CC=g++
CFLAGS=-I.

def := definitions.cpp

parser: Mabano_Lada_PE01.cpp $(def)
	$(CC) -o Mabano_Lada_PE01.exe Mabano_Lada_PE01.cpp $(def)

test: Test.cpp definitinos.cpp
	$(CC) -o Test.cpp $(def)

clean:
	rm *.exe
