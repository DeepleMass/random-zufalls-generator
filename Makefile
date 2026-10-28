# Makefile of the project: wraps the most common tasks around cargo and maturin.
# Usage: make <target>   (default target: release)

# The cargo command-line tool.
CARGO := cargo
# The build tool for the Python bindings.
MATURIN := maturin
# The Python interpreter in use.
PYTHON := python3
# The directory of the Python bindings.
BINDING_DIRECTORY := bindings/python
# The glob pattern of the produced wheels.
WHEEL_PATTERN := target/wheels/*.whl

# Default target: release build of all Rust members.
all: release

# Compiles all Rust members in debug mode (fast build, with checks).
build:
	$(CARGO) build --workspace

# Compiles all Rust members in release mode (optimized).
release:
	$(CARGO) build --release --workspace

# Runs all Rust tests (examples, lemmas and theorems of the paper).
test: release
	$(CARGO) test --workspace --release

# Checks warnings without producing artifacts.
check:
	$(CARGO) check --workspace
	$(CARGO) check --workspace --all-targets

# Builds the HTML reference documentation (rustdoc) of all Rust members into target/doc.
doc:
	$(CARGO) doc --no-deps --workspace

# Builds the documentation and opens it in the default browser.
doc-open: doc
	open target/doc/bit_sequences/index.html

# Builds the Python wheel of the bindings with maturin (requires maturin, see README).
wheel:
	cd $(BINDING_DIRECTORY) && $(MATURIN) build --release

# Installs the latest Python wheel user-wide into the interpreter (Homebrew Python).
python-install: wheel
	$(PYTHON) -m pip install --user --break-system-packages $(WHEEL_PATTERN)

# Builds and installs the bindings into a virtual environment (activated via VIRTUAL_ENV).
python-venv: wheel
	$(PYTHON) -m pip install $(WHEEL_PATTERN)

# Removes all build artifacts of cargo and maturin.
clean:
	$(CARGO) clean

# Lists the available targets as help.
help:
	@echo "Targets: all release build test check doc doc-open wheel python-install python-venv clean"

# Prevents files with the names of targets from being interpreted as targets.
.PHONY: all build release test check doc doc-open wheel python-install python-venv clean help
