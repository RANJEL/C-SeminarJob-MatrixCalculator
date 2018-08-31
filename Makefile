#Author: Jan Lejnar
#lejnajan@fit.cvut.cz

CXX=g++
LD=g++
CFLAGS=-Wall -pedantic -std=c++11 -Wno-long-long -O0 -ggdb
BINARY=lejnajan
OBJECTS=\
        ./denseMatrix.o\
	./exceptionInOperation.o\
	./main.o\
	./matrix.o\
	./matrixDatabase.o\
	./sparseMatrix.o\
	./textUserInterface.o
	
all:
	make compile
	make doc

compile: $(BINARY)
	
run: $(BINARY)
	./$(BINARY)

doc:
	doxygen Doxyfile
	mv html doc

count:
	wc -l ./src/*.h ./src/*.cpp

clean:
	rm -rf $(BINARY) *.o backup doc

$(BINARY): $(OBJECTS)
	$(LD) -o $(BINARY) $(OBJECTS)

denseMatrix.o: src/denseMatrix.cpp src/denseMatrix.h src/matrix.h \
 src/exceptionInOperation.h src/sparseMatrix.h
	$(CXX) $(CFLAGS) -c -o $@ $<

exceptionInOperation.o: src/exceptionInOperation.cpp \
 src/exceptionInOperation.h
	$(CXX) $(CFLAGS) -c -o $@ $<

main.o: src/main.cpp src/textUserInterface.h src/denseMatrix.h \
 src/matrix.h src/exceptionInOperation.h src/sparseMatrix.h \
 src/matrixDatabase.h
	$(CXX) $(CFLAGS) -c -o $@ $<

matrix.o: src/matrix.cpp src/matrix.h
	$(CXX) $(CFLAGS) -c -o $@ $<

matrixDatabase.o: src/matrixDatabase.cpp src/matrixDatabase.h \
 src/denseMatrix.h src/matrix.h src/exceptionInOperation.h \
 src/sparseMatrix.h
	$(CXX) $(CFLAGS) -c -o $@ $<

sparseMatrix.o: src/sparseMatrix.cpp src/sparseMatrix.h src/matrix.h \
 src/exceptionInOperation.h src/denseMatrix.h
	$(CXX) $(CFLAGS) -c -o $@ $<

textUserInterface.o: src/textUserInterface.cpp src/textUserInterface.h \
 src/denseMatrix.h src/matrix.h src/exceptionInOperation.h \
 src/sparseMatrix.h src/matrixDatabase.h
	$(CXX) $(CFLAGS) -c -o $@ $<

