 
# ------------------ Compilation options  ------------------------

INSTDIR= ../bin
TARGET= hlfe_detector

# Loads math library.  
INC=-I
LIB=-L
OCVDIR=/usr/include/opencv
LINK1=-L$(OCVDIR)/lib -L.
LINK2=-lcxcore -lcv -lhighgui
OUT=-o 

OBJFILES = matrice.o erreur.o match.o Probability.o BinomialProbability.o exception.o filetools.o

# LINUX
SIFT=sift
MAKEKEYFILE=makekeyfile
KEYMAKEFILE=Makefile

#windows


# Flags for the C compiler: 
#   -Wall for strict gcc warnings (requires prototypes for all functions).
#   -g to produce debug data for gdb
#   -O for optimization
CFLAGS = -g -Wall 
CXXFLAGS = $(CXXFLAGS0) $(CFLAGS) -I$(OCVDIR) -I$(OCVDIR)/otherlibs/highgui -I$(OCVDIR)/cvaux/include
CC=g++
C=gcc

all: $(TARGET)

$(TARGET): main.o libhlfe.a
	${CC} $(CXXFLAGS) ${CFLAGS} $(LINK1) ${LIBS} main.o  -o $(TARGET) $(LINK2) -lhlfe

libhlfe.a: $(OBJFILES)
	ar rcs libhlfe.a $(OBJFILES)

main.o: main.cpp
	${CC} $(CXXFLAGS) $(LINK1) ${CFLAGS} ${LIBS} -c main.cpp
#	-$(CXX) $(CXXFLAGS) $(LINK1) $(OUT)main.o maincam.cpp $(LINK2)

camexe: maincam.cpp
	${CC} $(CXXFLAGS) $(LINK1) ${CFLAGS} ${LIBS} maincam.cpp -o camexe
##	-$(CXX) $(CXXFLAGS) $(LINK1) $(OUT)cam.exe maincam.cpp $(LINK2)

match.o: match.c
	${CC} ${CFLAGS} -c match.c

matrice.o: matrice.cpp
	${CC} ${CFLAGS} -c matrice.cpp
	
erreur.o: erreur.cpp
	${CC} ${CFLAGS} -c erreur.cpp
	
Probability.o: classprob/Probability.cpp
	${CC} $(CFLAGS) -c classprob/Probability.cpp
	
BinomialProbability.o: classprob/BinomialProbability.cpp
	${CC} $(CFLAGS) -c classprob/BinomialProbability.cpp
	
exception.o: inifile/exception.c
	${CC} $(CFLAGS) -c inifile/exception.c

filetools.o: inifile/filetools.c
	${CC} $(CFLAGS) -c inifile/filetools.c
	
	
clean: 
	rm -f $(TARGET) *.o *.bak *~

install: $(TARGET)
	cp $(TARGET) util/$(SIFT) util/$(MAKEKEYFILE) util/$(KEYMAKEFILE) $(INSTDIR)

# How to make .key files
XXX: key/XXX.key

key/XXX.key: pgm/XXX.pgm jpg/XXX.jpg
	$(SIFT) <pgm/XXX.pgm >key/XXX.key

pgm/XXX.pgm: jpg/XXX.jpg
	convert "jpg/XXX.jpg" "pgm/XXX.pgm"

# LAST LINE
    
