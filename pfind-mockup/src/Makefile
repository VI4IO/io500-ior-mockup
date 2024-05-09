objects = pfind.o
all: pfind.a

pfind.a: $(objects)
	ar cr pfind.a $(objects)
	cp pfind.a ../

$(objects): %.o: %.c
	mpicc -c -Wall -Werror -O3 $<

clean:
	rm $(objects) *.a
