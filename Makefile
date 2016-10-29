CFLAGS = -Wall -std=c99 -g

a3: a3.c a3.o ship.o timer.o intersect.o Makefile  #curses.o
	gcc $(CFLAGS) ship.o a3.o timer.o intersect.o -lm -lncurses -o lander

a3.o: a3.c
	gcc $(CFLAGS) -c a3.c

ship.o: ship.c ship.h 
	gcc $(CFLAGS) -c ship.c -lm -lncurses

timer.o: timer.c timer.h
	gcc $(CFLAGS) -c timer.c 

#curses.o: curses.c curses.h
#	gcc $(CFLAGS) -c curses.c -lncurses

intersect.o: intersect.c intersect.h
	gcc $(CFLAGS) -c intersect.c -lm

test: test.o
	gcc $(CFLAGS) test.o -o test
test.o:
	gcc $(CFLAGS) -c test.c

lengths: 
	wc -L *.c *.h README Makefile

clean:
	rm -f -v a3 test *.o submit.tar core

tar:
	tar -cvf submit.tar a3.c a3.h ship.c ship.h intersect.c intersect.h timer.c timer.h Makefile README landscape.txt\


