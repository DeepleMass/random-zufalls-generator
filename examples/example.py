"""Example: using the Rust library `bitsequences` from Python.

Prerequisite: installed bindings (see the Makefile, target
`python-install` or `python-venv`).

Run: python3 examples/example.py
"""

# Import the Rust implementation of the paper as a Python module.
import bitsequences

# --- Section 6: base distribution by offsetting ---------------------

# The cumulated distribution of the class B_5 (16, 8, 4, 2, 1).
cumulated_distribution = [16, 8, 4, 2, 1]
# Determine the base distribution by offsetting (paper example).
base_distribution = bitsequences.base_distribution_from_cumulated(cumulated_distribution)
# The paper expects the base distribution (4; 2; 1; 0; 1).
assert base_distribution == [4, 2, 1, 0, 1], base_distribution
# Print the result of the offsetting.
print("Base distribution of B_5:", base_distribution)

# --- Section 7: generators and membership ----------------------------

# Generate the run-length sequence of the class representative of B_7.
run_sequence = bitsequences.iterative_generator(7)
# The representative must be a member of the class.
assert bitsequences.is_member(run_sequence, 7)
# Apply the recursive transition to B_8 (construction lemma).
target_sequence = bitsequences.recursive_transition(run_sequence)
# The result must be a member of the successor class.
assert bitsequences.is_member(target_sequence, 8)
# Print the representative of B_7 (expected: 64 bits in total).
print("Representative of B_7:", run_sequence)

# --- Section 8: composition from binary representations --------------

# The paper's example number: 341 = 101010101_2.
example_number = 341
# Determine the composite distribution.
distribution = bitsequences.composite_frequency(example_number)
# The paper tabulates (86, 42, 22, 10, 6, 2, 2, 0, 1).
assert distribution == [86, 42, 22, 10, 6, 2, 2, 0, 1], distribution
# The weighted total length must again be the number itself.
assert bitsequences.composite_length(example_number) == example_number
# The cumulated frequency C_1 must be 171 (identity of the paper).
assert bitsequences.cumulated_composite_frequency(example_number, 1) == 171
# Print the composite distribution.
print("Composite distribution of B = 341:", distribution)

# --- Sections 4/5: one-step test on raw bytes -------------------------

# An example block of raw bytes (identical to the bit sequence evaluation).
example_bytes = bytes([0b01001100, 0b11000001, 0b11111111])
# Determine the run histogram over the bytes (fast path).
histogram = bitsequences.run_histogram_from_bytes(example_bytes)
# Determine the cumulated distribution from the histogram.
cumulated_from_bytes = bitsequences.cumulated_distribution_from_histogram(histogram)
# Determine the corresponding bit sequence for comparison (MSB first per byte).
bits = [b for byte in example_bytes for b in [(byte >> 7) & 1,
        (byte >> 6) & 1, (byte >> 5) & 1, (byte >> 4) & 1,
        (byte >> 3) & 1, (byte >> 2) & 1, (byte >> 1) & 1,
        byte & 1]]
# The classical one-step test on the same sequence.
cumulated_from_bits = bitsequences.one_step_test_bits(bits)
# Both paths must yield the same result (equivalence lemma).
assert cumulated_from_bytes == cumulated_from_bits
# The consistency condition l_1 = M must hold.
assert cumulated_from_bytes[0] == len(bits)
# Print the evaluation of the example block.
print("Cumulated distribution of the example bytes:", cumulated_from_bytes)

# Everything passed: print the closing message.
print("All examples passed.")
