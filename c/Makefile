DEBUG=0
CFLAGS=-g -DDEBUG=$(DEBUG)


all: solver
force: clean solver

debug: DEBUG=4
debug: force

release: DEBUG=0
release: force

clean:
	rm -rf out
	rm -f solver

solver: out/main.o out/sudoku.o
	gcc -o solver $^

out/%.o: %.c
	mkdir -p out
	gcc $(CFLAGS) -c $? -o $@

.PHONY: clean force
