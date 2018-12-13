all: bench

bench: test
	./test

test: ../mlroot.c dummy.c test.ml 
	ocamlfind opt -o test -thread -package core,core_bench -linkpkg -I . -I ../ $^

clean:
	rm -f test test.cm* test.o dummy.o ../mlroot.o

.PHONY: all bench
