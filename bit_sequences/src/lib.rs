//! Implementation of the paper "The Evaluation of Finite Bit
//! Sequences".
//!
//! This crate implements the contents of the paper in Rust. The
//! modules follow the structure of the paper:
//!
//! | Module            | Paper section                                           |
//! |-------------------|---------------------------------------------------------|
//! | [`bit_sequence`]  | 3: Foundations, runs and sub-runs                       |
//! | [`one_step_test`] | 4: The one-step test in both variants                   |
//! | [`classes`]       | 6: Cumulated/base distribution, classes `B_N`           |
//! | [`generators`]    | 7: Iterative and recursive generator                    |
//! | [`composition`]   | 8: Composite frequency distributions                   |
//!
//! The command-line tool `evaluate` (Section 5 of the paper)
//! evaluates a data set of collected files.
//!
//! The documentation strategy: rustdoc is the single source of the
//! reference documentation (`make doc` builds it as HTML); every
//! public item carries a doc comment and at least one executable
//! example (doctest, verified by `make test`); the `missing_docs`
//! lint enforces completeness on every build.

// Enforces doc comments on all public items of the crate.
#![warn(missing_docs)]
// Enforces that all rustdoc links point to existing items.
#![warn(rustdoc::broken_intra_doc_links)]

/// Foundations: bit sequences, runs, run-length sequences (paper Section 3).
pub mod bit_sequence;

/// The one-step test in both variants including the lookup table
/// (paper Section 4).
pub mod one_step_test;

/// Cumulated and base distribution, classes `B_N`, membership
/// (paper Section 6).
pub mod classes;

/// Iterative and recursive generator for class representatives
/// (paper Section 7).
pub mod generators;

/// Composite frequencies from the binary representation of natural
/// numbers (paper Section 8).
pub mod composition;
