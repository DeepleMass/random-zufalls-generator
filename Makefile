# Makefile des Projekts: Wrappt die haeufigsten Aufgaben um Cargo und maturin.
# Aufruf: make <ziel>   (Standardziel: release)

# Das Kommandozeilenwerkzeug von Cargo.
CARGO := cargo
# Das Build-Werkzeug fuer die Python-Bindings.
MATURIN := maturin
# Der verwendete Python-Interpreter.
PYTHON := python3
# Das Verzeichnis der Python-Bindings.
BINDING_VERZEICHNIS := bindings/python
# Der Mustername der erzeugten Raeder.
RAD_MUSTER := target/wheels/*.whl

# Standardziel: Release-Bau aller Rust-Glieder.
all: release

# Kompiliert alle Rust-Glieder im Debug-Modus (schneller Bau, mit Pruefungen).
build:
	$(CARGO) build --workspace

# Kompiliert alle Rust-Glieder im Release-Modus (optimiert).
release:
	$(CARGO) build --release --workspace

# Fuehrt alle Rust-Tests aus (Beispiele, Lemmata und Saetze des Papers).
test: release
	$(CARGO) test --workspace --release

# Prueft Formatierung und Warnungen ohne Artefakte zu erzeugen.
pruefen:
	$(CARGO) check --workspace
	$(CARGO) check --workspace --all-targets

# Baut die HTML-Referenzdokumentation (rustdoc) aller Rust-Glieder nach target/doc.
doc:
	$(CARGO) doc --no-deps --workspace

# Baut die Dokumentation und oeffnet sie im Standardbrowser.
doc-offen: doc
	open target/doc/bitfolgen/index.html

# Baut das Python-Rad der Bindings mit maturin (benoetigt maturin, siehe README).
python-rad:
	cd $(BINDING_VERZEICHNIS) && $(MATURIN) build --release

# Installiert das neueste Python-Rad nutzerweit in den Interpreter (Homebrew-Python).
python-install: python-rad
	$(PYTHON) -m pip install --user --break-system-packages $(RAD_MUSTER)

# Baut und installiert die Bindings in eine virtuelle Umgebung (aktiviert via VIRTUAL_ENV).
python-venv: python-rad
	$(PYTHON) -m pip install $(RAD_MUSTER)

# Entfernt alle Bauartefakte von Cargo und maturin.
clean:
	$(CARGO) clean

# Auflistung der verfuegbaren Ziele als Hilfe.
help:
	@echo "Ziele: all release build test pruefen doc doc-offen python-rad python-install python-venv clean"

# Verhindert, dass Dateien mit Zielnamen als Ziel interpretiert werden.
.PHONY: all build release test pruefen doc doc-offen python-rad python-install python-venv clean help
