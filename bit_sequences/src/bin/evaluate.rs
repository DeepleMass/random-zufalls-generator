//! The `evaluate` command-line tool (paper Section 5): evaluates a
//! data set of collected files with the one-step test - the files,
//! sorted by name, correspond to the chronological order of the
//! timestamp file names.
//!
//! Usage: `evaluate <directory>`
//!
//! The output comprises the number of bits $M$, the table of the
//! frequencies $l_n$ with logarithm and expectation
//! $M \cdot 2^{1-n}$, the chi-square goodness-of-fit test of the run
//! lengths against the geometric distribution, and the estimated
//! transition probability.

use std::env;
use std::fs;
use std::io::Read;
use std::path::PathBuf;
use std::process::ExitCode;

use bit_sequences::one_step_test::cumulated_distribution_from_histogram;
use bit_sequences::one_step_test::RunFrequencyCounter;

/// Entry point of the program; expects the data directory as the
/// first argument.
fn main() -> ExitCode {
    // Consider the command-line arguments without the program name.
    let arguments: Vec<String> = env::args().skip(1).collect();
    // Early exit: without an argument no directory can be read.
    if arguments.is_empty() {
        // Give a hint about the expected usage.
        eprintln!("Usage: evaluate <directory>");
        // Terminate the program with a failure code.
        return ExitCode::FAILURE;
    }
    // Take over the data directory from the first argument.
    let directory = PathBuf::from(&arguments[0]);
    // Create the counter for the entire, concatenated bit sequence.
    let mut counter = RunFrequencyCounter::new();
    // Determine all collected files of the directory sorted by name.
    let file_names = match collect_file_names(&directory) {
        // The successful case yields the sorted list of names.
        Ok(file_names) => file_names,
        // Read errors are reported in plain text.
        Err(error_message) => {
            // Write the error to the standard error output.
            eprintln!("Error reading the directory: {error_message}");
            // Terminate the program with a failure code.
            return ExitCode::FAILURE;
        }
    };
    // Early exit: an empty directory has nothing to evaluate.
    if file_names.is_empty() {
        // State the situation on the standard error output.
        eprintln!("No files found in {}.", directory.display());
        // Terminate the program with a failure code.
        return ExitCode::FAILURE;
    }
    // Feed every collected file into the counter, chronologically sorted.
    for file_name in file_names.iter() {
        // Assemble the full path of the file.
        let file_path = directory.join(file_name);
        // Feed the file into the counter; report errors and abort.
        if let Err(error_message) = process_file(&mut counter, &file_path) {
            // Write the error with the file name to the error output.
            eprintln!("Error on {}: {error_message}", file_path.display());
            // Terminate the program with a failure code.
            return ExitCode::FAILURE;
        }
    }
    // Finalize the sequence: the last open run is booked.
    counter.finalize();
    // The number of bits M is eight times the number of bytes.
    let bit_count = counter.byte_count() * 8;
    // Compute the cumulated distribution from the run histogram.
    let cumulated_distribution = cumulated_distribution_from_histogram(counter.histogram());
    // Consistency check of the paper: $l_1$ must be exactly M.
    assert!(
        cumulated_distribution[0] == bit_count,
        "consistency violation l_1 = M"
    );
    // Print the header of the output with the collection totals.
    println!("Files:           {}", file_names.len());
    // Print the number of bytes of the data set.
    println!("Bytes:           {}", counter.byte_count());
    // Print the number of bits M of the data set.
    println!("Bits M:          {bit_count}");
    // Print the length P of the longest run.
    println!("Longest run P:   {}", cumulated_distribution.len());
    // Output a blank line before the table.
    println!();
    // Output the table header with the column labels.
    println!(" n |      l_n |  log2(l_n) |  M*2^(1-n)");
    // Output the separating line below the table header.
    println!("---+----------+------------+----------");
    // Limit the table to the first 25 rows (the rest is in the chi-square).
    for (run_position, &frequency) in cumulated_distribution.iter().enumerate().take(25) {
        // The run length is the table position plus 1.
        let position = run_position as u64 + 1;
        // The base-2 logarithm of the frequency as a floating point number.
        let logarithm = (frequency as f64).log2();
        // The theoretical expectation $M \cdot 2^{1-n}$.
        let expectation = bit_count as f64 * 2f64.powi(1 - position as i32);
        // Output the table row of the length n.
        println!("{position:2} | {frequency:8} | {logarithm:10.2} | {expectation:8.0}");
    }
    // Output a blank line after the table.
    println!();
    // Run the chi-square goodness-of-fit test and print its result.
    print_chi_square(counter.histogram());
    // Determine and print the estimated transition probability.
    print_transition_probability(counter.histogram(), bit_count);
    // Terminate the program successfully.
    ExitCode::SUCCESS
}

/// Determines the name-sorted list of all files of a directory
/// (sub-directories are skipped).
///
/// * `directory` - the directory to scan.
/// * Returns: sorted file names or an error message.
fn collect_file_names(directory: &PathBuf) -> Result<Vec<String>, String> {
    // Open the directory for reading; provide the cause of an error.
    let read_handle = fs::read_dir(directory).map_err(|error| error.to_string())?;
    // Create the list of names with sufficient capacity.
    let mut file_names: Vec<String> = Vec::new();
    // Examine every directory entry individually.
    for entry in read_handle {
        // Unwrap the entry; pass errors on as strings.
        let entry = entry.map_err(|error| error.to_string())?;
        // Determine the file attributes of the entry.
        let attributes = fs::metadata(entry.path()).map_err(|error| error.to_string())?;
        // Early exit: sub-directories are not collected files.
        if !attributes.is_file() {
            continue;
        }
        // Take over the file name as text; pass errors on as strings.
        let file_name = entry.file_name().into_string().map_err(|name| {
            format!("invalid file name: {name:?}")
        })?;
        // Add the file name to the list.
        file_names.push(file_name);
    }
    // Sort the list of names lexicographically (= chronologically).
    file_names.sort();
    // Return the sorted list.
    Ok(file_names)
}

/// Reads a file block by block and feeds it into the counter; the
/// runs continue across block and file boundaries.
///
/// * `counter` - the continuing run frequency counter.
/// * `file_path` - the path of the file to process.
/// * Returns: empty on success, otherwise an error message.
fn process_file(counter: &mut RunFrequencyCounter, file_path: &PathBuf) -> Result<(), String> {
    // Open the file for reading; pass errors on as strings.
    let mut file_handle = fs::File::open(file_path).map_err(|error| error.to_string())?;
    // The buffer of one block; 1 MiB balances syscalls against memory.
    let mut block = vec![0u8; 1 << 20];
    // Process blocks as long as they are being read.
    loop {
        // Fill the block; pass errors on as plain text.
        let read_count = file_handle
            .read(&mut block)
            .map_err(|error| error.to_string())?;
        // Early exit: zero bytes read means the end of the file is reached.
        if read_count == 0 {
            break;
        }
        // Feed the read portion of the block into the counter.
        counter.process_bytes(&block[..read_count]);
    }
    // Report success without an error message.
    Ok(())
}

/// Runs the chi-square goodness-of-fit test of the run histogram
/// against the geometric distribution $g_n = R \cdot 2^{-n}$ and
/// prints the statistic, the degrees of freedom and the ratio.
///
/// * `histogram` - the run frequencies per length.
fn print_chi_square(histogram: &[u64]) {
    // The total number R of runs as the sum of the histogram.
    let run_count: u64 = histogram.iter().sum();
    // The chi-square statistic beginning at zero.
    let mut chi_square = 0f64;
    // The number of degrees of freedom beginning at zero.
    let mut degrees_of_freedom = 0u32;
    // Include every run length with a sufficient expectation in the statistic.
    for (run_position, &frequency) in histogram.iter().enumerate() {
        // The run length is the table position plus 1.
        let position = run_position as f64 + 1.0;
        // The expected frequency under the geometric distribution.
        let expectation = run_count as f64 * 2f64.powf(-position);
        // Early exit: cells below 10 are unusable for the approximation.
        if expectation < 10.0 {
            break;
        }
        // The squared contribution of this cell to the statistic.
        let contribution = (frequency as f64 - expectation).powi(2) / expectation;
        // Add the contribution to the statistic.
        chi_square += contribution;
        // Note one more degree of freedom.
        degrees_of_freedom += 1;
    }
    // Print the statistic with degrees of freedom and classification.
    println!("Chi-square:     {chi_square:.1} at {degrees_of_freedom} degrees of freedom");
    // Print the ratio to the number of degrees of freedom as the size of the deviation.
    println!("Ratio:          {:.2} (values around 1 speak for the geometric distribution)",
        chi_square / degrees_of_freedom as f64);
}

/// Determines and prints the estimated transition probability
/// $\hat p$ (share of bit changes among all bit pairs) together with
/// the standard deviation and the deviation from $1/2$ in sigma
/// units.
///
/// * `histogram` - the run frequencies per length.
/// * `bit_count` - the total number M of bits.
fn print_transition_probability(histogram: &[u64], bit_count: u64) {
    // The total number R of runs as the sum of the histogram.
    let run_count: u64 = histogram.iter().sum();
    // Early exit: without pairs no estimation is possible.
    if bit_count < 2 {
        return;
    }
    // The estimate p = (R-1)/(M-1) as the share of changes per pair.
    let estimate = (run_count - 1) as f64 / (bit_count - 1) as f64;
    // The standard deviation of the estimate under p = 1/2.
    let sigma = (0.25 / (bit_count - 1) as f64).sqrt();
    // The deviation of the estimate from 1/2 in sigma units.
    let deviation_in_sigma = (estimate - 0.5) / sigma;
    // Print the estimate with its uncertainty.
    println!("Transition rate: {estimate:.5} +- {sigma:.5} (expected 0.5)");
    // Print the deviation in sigma units with its classification.
    println!("Deviation:       {deviation_in_sigma:.2} sigma");
}
