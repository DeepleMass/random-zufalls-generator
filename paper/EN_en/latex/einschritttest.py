# Reference implementation of the algorithms of the paper
# "The Evaluation of Finite Bit Sequences".
#
#  - one_step(bits):     one-step test, variant for bit sequences
#                        (Algorithm 2)
#  - one_step_runs(r):   one-step test, variant for sequences of runs
#                        (Algorithm 1)
#  - b_n_iterative(N):   iterative generator for B_N (Algorithm 3)
#  - b_n_recursive(r):   recursive passage B_N -> B_{N+1} (Algorithm 4)
#  - in_class(r, N):     membership test for B_N
#
# All functions operate on sequences of bits (0/1) or on sequences
# of run lengths. The return values are the cumulated distribution
# or the sequence of run lengths, respectively.


def one_step(bits):
    """Cumulated distribution of the runs and sub-runs of a bit sequence.

    For every bit of the sequence, all positions 1..z are counted,
    where z is the length of the run begun at that bit.
    """
    L, z, previous = [], 0, None
    for b in bits:
        if b == previous:
            z += 1
        else:
            z = 1
            previous = b
        for i in range(z):
            if i == len(L):
                L.append(0)
            L[i] += 1
    return L


def one_step_runs(runs):
    """Cumulated distribution from the sequence of run lengths.

    Every run of length l increases position i (1 <= i <= l)
    by l - i + 1.
    """
    L = []
    for l in runs:
        while len(L) < l:
            L.append(0)
        for i in range(1, l + 1):
            L[i - 1] += l - i + 1
    return L


def b_n_iterative(N):
    """Class representative of B_N as a sequence of run lengths
    (Algorithm 3): one run of length N, then for i = N-2, ..., 1
    exactly 2^(N-i-2) runs of length i each.
    """
    runs = [N]
    for i in range(N - 2, 0, -1):
        runs += [i] * (2 ** (N - i - 2))
    return runs


def b_n_recursive(runs):
    """Passage B_N -> B_{N+1} (Algorithm 4).

    Extends every run by one bit and appends 2^(N-2) runs of length 1
    (N = longest run of the input). The cases N = 1 and N = 2 require
    no augmentation.
    """
    N = max(runs)
    extra = 0 if N < 2 else 2 ** (N - 2)
    return [l + 1 for l in runs] + [1] * extra


def in_class(runs, N):
    """Membership test: does the sequence of run lengths lie in B_N?

    Checks whether the cumulated distribution falls off exactly
    exponentially: l_n = 2^(N-n) for all 1 <= n <= N.
    """
    L = one_step_runs(runs)
    return len(L) == N and all(
        L[n - 1] == 2 ** (N - n) for n in range(1, N + 1)
    )


def runs_to_bits(runs, start=0):
    """Realisation of a sequence of run lengths as a bit sequence."""
    bits, b = [], start
    for l in runs:
        bits += [b] * l
        b = 1 - b
    return bits


def runs_of(bits):
    """Run lengths of a bit sequence."""
    runs, z = [], 1
    for i in range(1, len(bits)):
        if bits[i] == bits[i - 1]:
            z += 1
        else:
            runs.append(z)
            z = 1
    runs.append(z)
    return runs


if __name__ == "__main__":
    # Self-test: the generators produce elements of B_N, and both
    # variants of the one-step test agree.
    import random

    for N in range(1, 17):
        r = b_n_iterative(N)
        assert in_class(r, N), N
        assert one_step(runs_to_bits(r)) == one_step_runs(r), N
        if N >= 2:
            assert in_class(b_n_recursive(r), N + 1), N

    random.seed(0)
    for _ in range(20):
        bits = [random.randint(0, 1) for _ in range(5000)]
        assert one_step(bits) == one_step_runs(runs_of(bits))

    print("All self-tests passed.")
