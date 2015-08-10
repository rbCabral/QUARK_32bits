all:	dquark 

CC              = gcc
CFLAGS		= -march=pentium -O3 -m32 -lrt

CC_G    	= i586-poky-linux-uclibc-gcc --sysroot=/intel/galileo/arduino-1.5.3-Intel.1.0.3/hardware/tools/sysroots/i586-poky-linux-uclibc
CFLAGS_G	= -O3 -m32 -march=pentium-mmx -mno-mmx 


dquark.o:	sources/dquark.c
	$(CC) -c sources/dquark.c -o obj/dquark.o $(CFLAGS)
	
bench.o:	sources/bench.c
	$(CC) -c sources/bench.c -o obj/bench.o $(CFLAGS)

main.o:	sources/main.c
	$(CC) -c sources/main.c -o obj/main.o $(CFLAGS)	
	
dquark:		main.o dquark.o bench.o
	$(CC) -o bin/dquark obj/main.o obj/dquark.o obj/bench.o $(CFLAGS)

dquark_g.o:	sources/dquark.c
	$(CC_G) -c sources/dquark.c -o obj/dquark_g.o $(CFLAGS_G)
	
bench_g.o:	sources/bench.c
	$(CC_G) -c sources/bench.c -o obj/bench_g.o $(CFLAGS_G)

main_g.o:	sources/main.c
	$(CC_G) -c sources/main.c -o obj/main_g.o $(CFLAGS_G)	
	
dquark_g:	main_g.o dquark_g.o bench_g.o
	$(CC_G) -o bin/dquark_g obj/main_g.o obj/dquark_g.o obj/bench_g.o $(CFLAGS_G)	

	
clean:
	rm -f bin/* obj/*
