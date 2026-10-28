//! The entropy extractor (paper Sections 6 and 8): constructs a bit
//! sequence with exactly the paper's prescribed distribution from an
//! arbitrary entropy source.
//!
//! The target distribution is the composite frequency distribution
//! $_B H_n$ from Section 8, determined by the target length $B$. The
//! entropy source is read as a bit sequence, decomposed into runs,
//! and each run decrements the corresponding counter in the target
//! distribution. Runs whose counter is already zero are silently
//! discarded. When all counters reach zero, the accepted run lengths
//! are assembled into a bit sequence with alternating bit values.
//!
//! The result has by construction exactly the paper's prescribed
//! distribution; the ordering of the runs originates from the
//! entropy source and is therefore unpredictable.

use crate::bit_sequence::Bit;
use crate::bit_sequence::BitSequence;
use crate::composition::composite_frequency;

/// The target distribution for the extractor: how many runs of each
/// length are still needed. Index $k-1$ corresponds to run length $k$.
///
/// The counters start at the composite frequencies $_B H_n$ and are
/// decremented by the extractor as runs are accepted from the
/// entropy source. A counter at zero means runs of that length are
/// no longer needed and further runs of that length are discarded.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct TargetDistribution {
    /// The remaining number of runs needed per length; index $k-1$ is length $k$.
    remaining: Vec<u64>,
    /// The target length $B$ in bits (the sum $\sum n \cdot H_n$).
    target_length: u64,
}

impl TargetDistribution {
    /// Creates the target distribution for a given target length
    /// from the composite frequencies of the paper (Section 8).
    ///
    /// * `target_length` - the desired length $B$ of the output
    ///   sequence in bits, with $B \ge 1$.
    /// * Returns: the target distribution with all counters set to
    ///   the composite frequencies $_B H_n$.
    ///
    /// # Example
    ///
    /// ```
    /// use bit_sequences::extractor::TargetDistribution;
    /// let target = TargetDistribution::from_target_length(341);
    /// // The composite distribution for 341 is (86, 42, ..., 1).
    /// assert_eq!(target.remaining()[0], 86);
    /// assert_eq!(target.target_length(), 341);
    /// ```
    pub fn from_target_length(target_length: u64) -> Self {
        // Determine the composite frequencies for the target length.
        let frequencies = composite_frequency(target_length);
        // Assemble the target distribution from the frequencies.
        Self {
            remaining: frequencies,
            target_length,
        }
    }

    /// The remaining number of runs needed at each length.
    pub fn remaining(&self) -> &[u64] {
        // Return a view of the internal counter vector.
        &self.remaining
    }

    /// The target length $B$ in bits.
    pub fn target_length(&self) -> u64 {
        // Return the stored target length.
        self.target_length
    }

    /// Attempts to accept a run of the given length: decrements the
    /// counter if runs of this length are still needed.
    ///
    /// * `run_length` - the length of the run to accept.
    /// * Returns: `true` if the run was accepted (counter was > 0
    ///   and is now decremented), `false` if the run was discarded.
    pub fn accept(&mut self, run_length: u64) -> bool {
        // Early exit: runs beyond the target distribution are discarded.
        if run_length == 0 || run_length > self.remaining.len() as u64 {
            return false;
        }
        // The remaining count for this length.
        let count = &mut self.remaining[(run_length - 1) as usize];
        // Early exit: no more runs of this length needed.
        if *count == 0 {
            return false;
        }
        // Decrement the counter and accept the run.
        *count -= 1;
        // Signal acceptance.
        true
    }

    /// Checks whether the entire target distribution is fulfilled:
    /// all counters are zero.
    ///
    /// * Returns: `true` if and only if all counters are zero.
    pub fn is_fulfilled(&self) -> bool {
        // Check whether any counter is still positive.
        self.remaining.iter().all(|&count| count == 0)
    }
}

/// The result of an extraction run.
#[derive(Debug)]
pub enum ExtractionResult {
    /// The extraction succeeded: the accepted run lengths in the
    /// order they were found in the entropy source.
    Success(Vec<u64>),
    /// The entropy source was exhausted before the target
    /// distribution was fulfilled: the remaining counters are given.
    SourceExhausted(TargetDistribution),
}

/// Iterates over the run lengths of a bit sequence given as raw
/// bytes (most significant bit first per byte), yielding each run
/// length in order. The iterator tracks the open run across byte
/// boundaries.
///
/// * `bytes` - the raw bytes of the entropy source.
/// * Returns: an iterator over the run lengths.
pub fn run_length_iterator(bytes: &[u8]) -> impl Iterator<Item = u64> + '_ {
    // The run lengths of all bytes, flattened into one sequence.
    let mut all_runs: Vec<u64> = Vec::new();
    // The length of the currently open run (not yet yielded).
    let mut open_run: u64 = 0;
    // The last bit seen; `None` means nothing processed yet.
    let mut last_bit: Option<Bit> = None;

    // Process each byte of the source.
    for &byte_value in bytes.iter() {
        // The eight bits of this byte, most significant first.
        for bit_position in 0..8 {
            // The bit at the current position.
            let bit = (byte_value >> (7 - bit_position)) & 1;
            // Check whether this bit continues or ends the open run.
            match last_bit {
                // The first bit begins the first run.
                None => {
                    open_run = 1;
                }
                // The same bit extends the open run.
                Some(previous) if previous == bit => {
                    open_run += 1;
                }
                // A bit change yields the open run and begins a new one.
                Some(_) => {
                    all_runs.push(open_run);
                    open_run = 1;
                }
            }
            // The current bit is the predecessor of the next step.
            last_bit = Some(bit);
        }
    }

    // Yield the final open run if one exists.
    if open_run > 0 {
        all_runs.push(open_run);
    }

    // Return the collected run lengths as an iterator.
    all_runs.into_iter()
}

/// The main extraction algorithm: reads runs from the entropy
/// source, accepts each run that the target distribution still
/// needs, and silently discards the rest, until the target
/// distribution is fulfilled.
///
/// * `target` - the target distribution to fulfill.
/// * `entropy_bytes` - the raw bytes of the entropy source.
/// * Returns: `Success` with the accepted run lengths in order, or
///   `SourceExhausted` if the source ran out before the target was
///   fulfilled.
///
/// # Example
///
/// ```
/// use bit_sequences::extractor::{extract, TargetDistribution};
/// // A target distribution that needs exactly one run of length 1
/// // and one run of length 3 (= target length 4).
/// let target = TargetDistribution::from_target_length(4);
/// // An entropy source with runs 2, 1, 1, 3, 1 (encoded as raw bytes).
/// let source_bytes = [0b0110_1100, 0b1110_0000];
/// let result = extract(target, &source_bytes);
/// // The source must suffice.
/// assert!(matches!(result, bit_sequences::extractor::ExtractionResult::Success(_)));
/// ```
pub fn extract(mut target: TargetDistribution, entropy_bytes: &[u8]) -> ExtractionResult {
    // The accepted run lengths in the order they were found.
    let mut accepted_runs: Vec<u64> = Vec::new();
    // Iterate over the runs of the entropy source in order.
    for run_length in run_length_iterator(entropy_bytes) {
        // Attempt to accept this run: decrement if still needed.
        if target.accept(run_length) {
            // Record the accepted run length.
            accepted_runs.push(run_length);
            // Early exit: stop reading once the target is fulfilled.
            if target.is_fulfilled() {
                // Return success with the accepted runs.
                return ExtractionResult::Success(accepted_runs);
            }
        }
        // Otherwise the run is silently discarded; continue.
    }
    // The source was exhausted; report the unfulfilled target.
    ExtractionResult::SourceExhausted(target)
}

/// Assembles the accepted run lengths into a bit sequence with
/// alternating bit values, starting with bit 0.
///
/// * `accepted_runs` - the run lengths in the order they were
///   accepted from the entropy source.
/// * Returns: the realized bit sequence.
pub fn assemble(accepted_runs: &[u64]) -> BitSequence {
    // Realize the run lengths as a bit sequence (alternating, start bit 0).
    BitSequence::from_run_lengths(accepted_runs)
}
