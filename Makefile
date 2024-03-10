CC=g++-13
CFLAGS=-I.

def := definitions.cpp

parser: Mabano_Lada_PE01.cpp $(def)
	$(CC) -o Mabano_Lada_PE01.exe Mabano_Lada_PE01.cpp $(def)

test: Test.cpp definitions.cpp
	$(CC) -o Test.exe Test.cpp $(def)
	./Test.exe

clean:
	rm *.exe
