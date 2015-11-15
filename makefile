project: projecttrial.o pieces.o main.o assist.o
	cc -c assist.c
	cc -c projecttrial.c
	cc -c pieces.c
	cc -c main.c
	cc projecttrial.o pieces.o main.o assist.o -lncurses -o project
assist.c: project.h
	cc -c assist.c
projecttrial.c: project.h
	cc -c projecttrial.c
pieces.h: project.h
	cc -c pieces.h
main.c: project.h
	cc -c main.c
project.h:
