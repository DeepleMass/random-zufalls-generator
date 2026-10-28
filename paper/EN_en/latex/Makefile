# Kompilation des Papers (XeLaTeX + biblatex/bibtex)
# Aufruf: make            (Vollversion inkl. Literatur)
#         make fast       (nur ein xelatex-Lauf)
#         make clean      (Hilfsdateien entfernen)

DOC = EvaluationOfFiniteBitSequences

.PHONY: all fast clean

all:
	xelatex -interaction=nonstopmode $(DOC).tex
	bibtex  $(DOC)
	xelatex -interaction=nonstopmode $(DOC).tex
	xelatex -interaction=nonstopmode $(DOC).tex

fast:
	xelatex -interaction=nonstopmode $(DOC).tex

clean:
	rm -f $(DOC).aux $(DOC).log $(DOC).bbl $(DOC).bcf $(DOC).blg \
	      $(DOC).out $(DOC).run.xml $(DOC).blg $(DOC).toc $(DOC).lof \
	      $(DOC).lot $(DOC)-blx.bib texput.log
