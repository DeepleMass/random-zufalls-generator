//! Generators for class representatives of $B_N$ (paper Section 7).
//!
//! The iterative generator produces the run-length sequence of a
//! representative directly from the closed form of the base
//! distribution theorem; the recursive transition describes the
//! construction from $B_{N-1}$ to $B_N$ (construction lemma): every
//! run is extended by one bit and $2^{N-3}$ runs of length 1 are
//! added.

use crate::bit_sequence::BitSequence;
use crate::classes::longest_run;

/// The iterative generator (Algorithm 3 of the paper): produces the
/// run-length sequence of a class representative of $B_N$ from the
/// closed form of the base distribution - one run of length $N$,
/// then for $i = N-2$ down to 1 exactly $2^{N-i-2}$ runs of length
/// $i$ each.
///
/// * `class_index` - the class index $N$ with $N \ge 1$.
/// * Returns: the run-length sequence of a representative of $B_N$.
///
/// # Example
///
/// ```
/// use bit_sequences::generators::iterative_generator;
/// assert_eq!(iterative_generator(4), vec![4, 2, 1, 1]);
/// ```
pub fn iterative_generator(class_index: u64) -> Vec<u64> {
    // The run-length sequence begins with the single longest run of length N.
    let mut run_lengths: Vec<u64> = vec![class_index];
    // Early exit: for N <= 2 there are no further runs.
    if class_index < 3 {
        return run_lengths;
    }
    // Traverse the augmentation lengths from N-2 downwards to 1.
    for run_length in (1..=(class_index - 2)).rev() {
        // The frequency of this length according to the closed form.
        let frequency = 1u64 << (class_index - run_length - 2);
        // Append the length to the run-length sequence frequency many times.
        for _ in 0..frequency {
            run_lengths.push(run_length);
        }
    }
    // Return the completed run-length sequence of the class representative.
    run_lengths
}

/// The recursive transition (Algorithm 4 of the paper): converts the
/// run-length sequence of an element of $B_N$ into an element of
/// $B_{N+1}$ by extending every run by one bit and adding $2^{N-2}$
/// runs of length 1 (construction lemma with target class $N+1$).
///
/// * `run_lengths` - the run-length sequence of an element of $B_N$.
/// * Returns: the run-length sequence of an element of $B_{N+1}$.
///
/// # Example
///
/// ```
/// use bit_sequences::generators::recursive_transition;
/// assert_eq!(recursive_transition(&[3, 1]), vec![4, 2, 1, 1]);
/// ```
pub fn recursive_transition(run_lengths: &[u64]) -> Vec<u64> {
    // The longest run N of the input controls the augmentation.
    let class_index = longest_run(run_lengths);
    // Extend all run lengths by 1 (step 1 of the lemma).
    let mut new_run_lengths: Vec<u64> = run_lengths.iter().map(|l| l + 1).collect();
    // Early exit: for N < 2 no augmentation is necessary.
    if class_index < 2 {
        return new_run_lengths;
    }
    // The number of new runs of length 1 according to the lemma.
    let augmentation = 1u64 << (class_index - 2);
    // Append the new runs of length 1 (step 2 of the lemma).
    for _ in 0..augmentation {
        new_run_lengths.push(1);
    }
    // Return the run-length sequence of the element of B_{N+1}.
    new_run_lengths
}

/// Produces the bit sequence of a class representative of $B_N$: the
/// realization of the run-length sequence of the iterative generator.
///
/// * `class_index` - the class index $N$ with $N \ge 1$.
/// * Returns: a bit sequence lying in $B_N$.
///
/// # Example
///
/// ```
/// use bit_sequences::generators::class_representative_sequence;
/// let representative = class_representative_sequence(3);
/// assert_eq!(representative.length(), 4);
/// ```
pub fn class_representative_sequence(class_index: u64) -> BitSequence {
    // Obtain the run-length sequence of the representative from the iterative generator.
    let run_lengths = iterative_generator(class_index);
    // Realize the run-length sequence as a bit sequence (alternating, starting bit 0).
    BitSequence::from_run_lengths(&run_lengths)
}
