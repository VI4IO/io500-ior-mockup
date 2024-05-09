objects = libaiori.o ior.o mdtest.o mdworkbench.o
all: libaiori.a
FLAGS = "-ggdb"

libaiori.a: $(objects) 
	ar cr libaiori.a $(objects)

$(objects): %.o: %.c
	mpicc -c $(FLAGS) -Wall -Werror -O3 $< 

clean:
	rm $(objects) *.a
