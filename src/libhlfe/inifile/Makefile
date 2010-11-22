TARGET=f_test
CC=gcc
CFLAGS=
SOURCES=${wildcard -r *.c}
OBJECTS=${SOURCES:%.c=%.o}

all: $(TARGET) 
# dep

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(CFLAGS)

%.o:%.c
	$(CC) -c $*.c $(CFLAGS)

clean:
	@rm -f $(TARGET) $(OBJECTS) *~

dep:
	makedepend -Y -- $(CFLAGS) -- $(SOURCES) 2>/dev/null

# DO NOT DELETE

filetools.o: filetools.h
test_filetools.o: filetools.h
