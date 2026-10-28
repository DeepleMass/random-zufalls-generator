//! The `extract` command-line tool: constructs a bit sequence with
//! exactly the paper's prescribed distribution from an entropy
//! source file (paper Sections 6 and 8).
//!
//! Usage: `extract <target_length> <entropy_file> [output_file]`
//!
//! The target length determines the composite frequency
//! distribution (Section 8). The entropy source is read as a bit
//! sequence and decomposed into runs. Each run decrements the
//! corresponding counter in the target distribution; runs whose
//! counter is already zero are silently discarded. When all
//! counters reach zero, the accepted run lengths are assembled into
//! the output bit sequence. If the entropy source is exhausted
//! before the target is fulfilled, the tool exits with an error.

use std::env;
use std::fs;
use std::process::ExitCode;

use bit_sequences::composition::composite_frequency;
use bit_sequences::composition::composite_length;
use bit_sequences::extractor::assemble;
use bit_sequences::extractor::extract;
use bit_sequences::extractor::TargetDistribution;

/// Entry point: expects the target length (in bits) and the entropy
/// source file; optionally an output file for the resulting bit
/// sequence.
fn main() -> ExitCode {
    // The command-line arguments without the program name.
    let arguments: Vec<String> = env::args().skip(1).collect();
    // Early exit: at least target length and entropy file are required.
    if arguments.len() < 2 {
        // Give a hint about the expected usage.
        eprintln!("Usage: extract <target_length_in_bits> <entropy_file> [output_file]");
        // Terminate with a failure code.
        return ExitCode::FAILURE;
    }
    // Parse the target length from the first argument.
    let target_length: u64 = match arguments[0].parse() {
        // The successful case yields the number.
        Ok(value) => value,
        // A parse error is reported in plain text.
        Err(_) => {
            // Report the invalid argument.
            eprintln!("Invalid target length: {}", arguments[0]);
            // Terminate with a failure code.
            return ExitCode::FAILURE;
        }
    };
    // Early exit: the target length must be positive.
    if target_length == 0 {
        // State the requirement.
        eprintln!("Target length must be positive.");
        // Terminate with a failure code.
        return ExitCode::FAILURE;
    }
    // Read the entropy source file.
    let entropy_bytes = match fs::read(&arguments[1]) {
        // The successful case yields the bytes.
        Ok(bytes) => bytes,
        // A read error is reported in plain text.
        Err(error) => {
            // Report the error with the file name.
            eprintln!("Error reading {}: {error}", arguments[1]);
            // Terminate with a failure code.
            return ExitCode::FAILURE;
        }
    };
    // Create the target distribution from the target length.
    let target = TargetDistribution::from_target_length(target_length);
    // Print the target distribution.
    println!("Target length:    {target_length} bits");
    // Print the composite frequency distribution.
    let frequencies = composite_frequency(target_length);
    // The number of runs needed (= sum of all frequencies).
    let total_runs: u64 = frequencies.iter().sum();
    // Print the run count.
    println!("Runs needed:       {total_runs}");
    // Print the distribution (first 20 entries for readability).
    print!("Distribution:      [");
    // Print each frequency, separated by commas.
    for (position, &frequency) in frequencies.iter().enumerate().take(20) {
        // Print a comma before all but the first entry.
        if position > 0 {
            print!(", ");
        }
        // Print the frequency value.
        print!("{frequency}");
    }
    // Close the bracket.
    println!("]");
    // Print the entropy source size.
    println!("Entropy source:    {} bytes ({} bits)",
             entropy_bytes.len(), entropy_bytes.len() * 8);
    // Run the extraction.
    let result = extract(target, &entropy_bytes);
    // Unpack the extraction result.
    let accepted_runs = match result {
        // Success: take the accepted run lengths.
        bit_sequences::extractor::ExtractionResult::Success(runs) => {
            // Report the success.
            println!("Extraction:        success ({} runs accepted)", runs.len());
            // Continue with the runs.
            runs
        }
        // The source was exhausted: report the error and terminate.
        bit_sequences::extractor::ExtractionResult::SourceExhausted(remaining) => {
            // Report the failure.
            eprintln!("Entropy source exhausted before the target was fulfilled.");
            // Print the remaining unfulfilled counters.
            for (position, &count) in remaining.remaining().iter().enumerate() {
                // Only report lengths that still need runs.
                if count > 0 {
                    // Report the remaining count for this length.
                    eprintln!("  Length {}: {} runs still needed", position + 1, count);
                }
            }
            // Terminate with a failure code.
            return ExitCode::FAILURE;
        }
    };
    // Assemble the accepted run lengths into a bit sequence.
    let output_sequence = assemble(&accepted_runs);
    // Verify that the output has exactly the target length.
    assert_eq!(
        output_sequence.length(),
        target_length,
        "output length must equal target length"
    );
    // Verify that the composite length of the distribution equals the target.
    assert_eq!(
        composite_length(target_length),
        target_length,
        "composite length identity"
    );
    // Print the output length.
    println!("Output length:     {} bits", output_sequence.length());
    // If an output file is given, write the bit sequence.
    if arguments.len() >= 3 {
        // Write the bits as a string of 0s and 1s.
        let bit_string: String = output_sequence
            .bits()
            .iter()
            .map(|&bit| if bit == 0 { '0' } else { '1' })
            .collect();
        // Attempt to write the output file.
        if let Err(error) = fs::write(&arguments[2], bit_string) {
            // Report the error.
            eprintln!("Error writing {}: {error}", arguments[2]);
            // Terminate with a failure code.
            return ExitCode::FAILURE;
        }
        // Report the successful write.
        println!("Written to:        {}", arguments[2]);
    }
    // Terminate successfully.
    ExitCode::SUCCESS
}
