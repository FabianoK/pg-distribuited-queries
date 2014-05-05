REMOVE=artigo.aux artigo.log artigo.pdf artigo.bbl artigo.blg
all:
	pdflatex artigo.tex
	bibtex artigo.aux
	pdflatex artigo.tex
	pdflatex artigo.tex
	evince artigo.pdf &

clean:
	rm -f ${REMOVE}
	
