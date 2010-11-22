# ------------------ Makefile  ------------------------ 

# NOTE : This Makefile is used at RUNTIME !!

#SIFT=siftWin32
SIFT=./sift

# How to make .key files
#XXX: key/XXX.key

#key/XXX.key: pgm/XXX.pgm jpg/XXX.jpg
#	$(SIFT) <pgm/XXX.pgm >key/XXX.key

#pgm/XXX.pgm: jpg/XXX.jpg
#	convert jpg/XXX.jpg pgm/XXX.pgm


key/%.key: pgm/%.pgm
	$(SIFT) < $< > $@

pgm/%.pgm: jpg/%.jpg
	convert $< $@

clean: 
	rm -f pgm/*.pgm key/*.key


