# ------------------ Compilation options  ------------------------ 

# Loads math library.  
LIBS =

# Flags for the C compiler: 
#   -Wall for strict gcc warnings (requires prototypes for all functions).
#   -g to produce debug data for gdb
#   -O for optimization
CFLAGS = -Wall -g 
CC= gcc
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


