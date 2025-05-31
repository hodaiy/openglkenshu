CFLAGS=-I/usr/X11R6/include
LDLIBS=-L/usr/X11R6/lib -lglut -lGL -lGLU -lcglm -lm
a.out: normalobj.c
	$(CC) $(CFLAGS) normalobj.c $(LDLIBS)
	./a.out
	rm -f a.out 