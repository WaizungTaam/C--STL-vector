CC=g++
FLAG=-std=c++11 -g

vector_demo.o: include/iterator.h include/vector.h test/vector_demo.cc
	$(CC) $(FLAG) include/iterator.h include/vector.h test/vector_demo.cc -o vector_demo.o