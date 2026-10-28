//! The `deviation` command-line tool: examines arbitrary files as bit
//! sequences and measures how far their distribution deviates from
//! the paper's target (paper Section 5: $l_n = M \cdot 2^{1-n}$ for
//! independent, uniformly distributed bits).
//!
//! Usage: `deviation [--full] <file>...`
//!
//! For every file the tool prints the number of bits and the
//! deviation score (chi-square per degree of freedom over the
//! run-length distribution; ~ 1.0 matches the target, larger values
//! are structured). With `--full` the per-length deviation
//! distribution is printed as well. Finally, the files are ranked by
//! their score, closest to the target first.

use std::env;
use std::fs;
use std::io::Read;
use std::path::PathBuf;
use std::process::ExitCode;

use bit_sequences::deviation::{deviation_distribution, deviation_score};
use bit_sequences::one_step_test::cumulated_distribution_from_histogram;
use bit_sequences::one_step_test::RunFrequencyCounter;

/// Entry point of the program; expects at least one file path,
/// optionally preceded by the flag `--full`.
fn main() -> ExitCode {
    // Consider the command-line arguments without the program name.
    let arguments: Vec<String> = env::args().skip(1).collect();
    // Whether the per-length deviation distribution should be printed.
    let mut print_full_distribution = false;
    // The paths of the files to examine.
    let mut file_paths: Vec<PathBuf> = Vec::new();
    // Examine every argument in turn.
    for argument in arguments.iter() {
        // The flag enables the full deviation distribution output.
        if argument == "--full" {
            print_full_distribution = true;
            // Continue with the next argument without treating it as a path.
            continue;
        }
        // Everything else is taken over as a file path.
        file_paths.push(PathBuf::from(argument));
    }
    // Early exit: without a file path there is nothing to examine.
    if file_paths.is_empty() {
        // Give a hint about the expected usage.
        eprintln!("Usage: deviation [--full] <file>...");
        // Terminate the program with a failure code.
        return ExitCode::FAILURE;
    }
    // The results as pairs of score and path, for the ranking at the end.
    let mut results: Vec<(f64, String)> = Vec::new();
    // Examine every file in turn.
    for file_path in file_paths.iter() {
        // Read the file and determine its run histogram; report errors and continue.
        let histogram = match read_run_histogram(file_path) {
            // The successful case yields the run histogram.
            Ok(histogram) => histogram,
            // Read errors are reported in plain text.
            Err(error_message) => {
                // Write the error to the standard error output.
                eprintln!("Error on {}: {error_message}", file_path.display());
                // Continue with the next file.
                continue;
            }
        };
        // The number of bits M is eight times the number of bytes.
        let bit_count = 8 * byte_count_of(&histogram);
        // Early exit: an empty file has no distribution to examine.
        if bit_count == 0 {
            // State the situation on the standard error output.
            eprintln!("Skipping empty file {}.", file_path.display());
            // Continue with the next file.
            continue;
        }
        // Compute the cumulated distribution from the run histogram.
        let cumulated_distribution = cumulated_distribution_from_histogram(&histogram.0);
        // Consistency check of the paper: $l_1$ must be exactly M.
        assert!(
            cumulated_distribution[0] == bit_count,
            "consistency violation l_1 = M"
        );
        // The deviation score of this file against the paper's target.
        let score = deviation_score(&histogram.0);
        // Print the summary line of this file.
        println!(
            "{}: {} bits, deviation score = {score:.3}",
            file_path.display(),
            bit_count
        );
        // Print the full deviation distribution if the flag is set.
        if print_full_distribution {
            // Output a blank line before the table.
            println!();
            // Output the table header with the column labels.
            println!(" n |      l_n |  expected | relative | log2");
            // Output the separating line below the table header.
            println!("---+----------+-----------+----------+-------");
            // Consider every entry of the deviation distribution.
            for entry in deviation_distribution(&cumulated_distribution, bit_count) {
                // Output the table row of this length.
                println!(
                    "{:2} | {:8} | {:9.1} | {:8.3} | {:6.3}",
                    entry.run_length,
                    entry.measured_frequency,
                    entry.expected_frequency,
                    entry.relative_deviation,
                    entry.logarithmic_deviation
                );
            }
            // Output a blank line after the table.
            println!();
        }
        // Remember the result for the ranking.
        results.push((score, file_path.display().to_string()));
    }
    // Early exit: without results (all files failed) no ranking is possible.
    if results.is_empty() {
        // Terminate the program with a failure code.
        return ExitCode::FAILURE;
    }
    // Sort the results by score, closest to the target first (NaN sorts last).
    results.sort_by(|left, right| left.0.total_cmp(&right.0));
    // Output a blank line before the ranking.
    println!();
    // Output the heading of the ranking.
    println!("Ranking (closest to the paper's target first):");
    // Output every result of the ranking in order.
    for (rank, (score, path)) in results.iter().enumerate() {
        // Print rank, score and path of this file.
        println!("{:2}. deviation score = {score:8.3}  {path}", rank + 1);
    }
    // Terminate the program successfully.
    ExitCode::SUCCESS
}

/// Reads a file block by block and determines its run histogram;
/// runs continue across block boundaries.
///
/// * `file_path` - the path of the file to read.
/// * Returns: the run histogram (frequency per run length, index
///   $k-1$) together with the number of bytes processed, or an error
///   message.
fn read_run_histogram(file_path: &PathBuf) -> Result<(Vec<u64>, u64), String> {
    // Open the file for reading; pass errors on as strings.
    let mut file_handle = fs::File::open(file_path).map_err(|error| error.to_string())?;
    // A fresh counter for the entire file as one bit sequence.
    let mut counter = RunFrequencyCounter::new();
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
    // Finalize the sequence so the last open run is booked.
    counter.finalize();
    // Return the histogram as its own vector.
    Ok((counter.histogram().to_vec(), counter.byte_count()))
}

/// Determines the number of bytes processed from a histogram pair.
///
/// * `histogram_pair` - the pair of run histogram and byte count.
/// * Returns: the number of bytes.
fn byte_count_of(histogram_pair: &(Vec<u64>, u64)) -> u64 {
    // Return the co-counted number of bytes of the pair.
    histogram_pair.1
}
