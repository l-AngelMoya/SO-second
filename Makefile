all: pagingdemand

pagingdemand: pagingdemand.c
	gcc -o pagingdemand pagingdemand.c

clean:
	rm ./pagingdemand