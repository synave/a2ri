all : model util matrix vector point geometry vertex edge face conversion topology bounding_box overlap icp quality subdivision hashtable graph space_partition polyhedron triangulation

model :
	latex $@.tex
	dvipdf $@.dvi
	
hashtable :
	latex $@.tex
	dvipdf $@.dvi

space_partition :
	latex $@.tex
	dvipdf $@.dvi

triangulation :
	latex $@.tex
	dvipdf $@.dvi

polyhedron :
	latex $@.tex
	dvipdf $@.dvi

graph :
	latex $@.tex
	bibtex $@
	latex $@.tex
	latex $@.tex
	dvipdf $@.dvi

util :
	latex $@.tex
	dvipdf $@.dvi

matrix :
	latex $@.tex
	dvipdf $@.dvi

vector :
	latex $@.tex
	dvipdf $@.dvi

point : 
	latex $@.tex
	dvipdf $@.dvi

geometry :
	latex $@.tex
	dvipdf $@.dvi

vertex :	
	latex $@.tex
	dvipdf $@.dvi

edge :
	latex $@.tex
	dvipdf $@.dvi

face :
	latex $@.tex
	dvipdf $@.dvi

conversion :
	latex $@.tex
	dvipdf $@.dvi

topology :
	latex $@.tex
	dvipdf $@.dvi

bounding_box :
	latex $@.tex
	bibtex $@
	latex $@.tex
	latex $@.tex
	dvipdf $@.dvi

overlap :
	latex $@.tex
	dvipdf $@.dvi

icp :
	latex $@.tex
	bibtex $@
	latex $@.tex
	latex $@.tex
	dvipdf $@.dvi

quality :
	latex $@.tex
	bibtex $@
	latex $@.tex
	latex $@.tex
	dvipdf $@.dvi

subdivision :
	latex $@.tex
	bibtex $@
	latex $@.tex
	latex $@.tex
	dvipdf $@.dvi

clean :
	rm -f *~ *.aux *.log *.pdf *.dvi *.bbl *.blg
