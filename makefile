kv : main.o kv.o hash.o
	g++ -o kv main.o kv.o hash.o

main.o : main.cpp
kv.o : kv.h kv.cpp
hash.o : hash.cpp

.PHONY : clean
clean : 
	-rm kv main.o kv.o hash.o
