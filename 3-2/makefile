CFLAGS=-I/usr/X11R6/include
LDLIBS=-L/usr/X11R6/lib -lglut -lGL -lGLU -lcglm -lm
a.out: 3-2.c
	$(CC) $(CFLAGS) 3-2.c ../rot_qua/rot_qua.c $(LDLIBS)
	./a.out
	rm -f a.out 