all:	squark squark_unr squark_unr_g squark_g

CC              = gcc
CFLAGS		= -march=pentium -O3 -m32 -lrt

CC_G    	= i586-poky-linux-uclibc-gcc --sysroot=/intel/galileo/arduino-1.5.3-Intel.1.0.3/hardware/tools/sysroots/i586-poky-linux-uclibc
CFLAGS_G	= -O3 -m32 -march=pentium-mmx -mno-mmx -mtune=generic


squark_unr.o:	sources/squark.c
	$(CC) -c sources/squark.c -DUNROLED -o obj/squark_unr.o $(CFLAGS)

squark.o:	sources/squark.c
	$(CC) -c sources/squark.c -o obj/squark.o $(CFLAGS)
	
bench.o:	sources/bench.c
	$(CC) -c sources/bench.c -o obj/bench.o $(CFLAGS)

main.o:	sources/main.c
	$(CC) -c sources/main.c -o obj/main.o $(CFLAGS)	
	
squark_unr:	main.o squark_unr.o bench.o
	$(CC) -o bin/squark_unr obj/main.o obj/squark_unr.o obj/bench.o $(CFLAGS)
	
squark:		main.o squark.o bench.o
	$(CC) -o bin/squark obj/main.o obj/squark.o obj/bench.o $(CFLAGS)

squark_unr_g.o:	sources/squark.c
	$(CC_G) -c sources/squark.c -DUNROLED -o obj/squark_unr_g.o $(CFLAGS_G)

squark_g.o:	sources/squark.c
	$(CC_G) -c sources/squark.c -o obj/squark_g.o $(CFLAGS_G)
	
bench_g.o:	sources/bench.c
	$(CC_G) -c sources/bench.c -o obj/bench_g.o $(CFLAGS_G)

main_g.o:	sources/main.c
	$(CC_G) -c sources/main.c -o obj/main_g.o $(CFLAGS_G)	
	
squark_unr_g:	main_g.o squark_unr_g.o bench_g.o
	$(CC_G) -o bin/squark_unr_g obj/main_g.o obj/squark_unr_g.o obj/bench_g.o $(CFLAGS_G)
	
squark_g:	main_g.o squark_g.o bench_g.o
	$(CC_G) -o bin/squark_g obj/main_g.o obj/squark_g.o obj/bench_g.o $(CFLAGS_G)	

	
clean:
	rm -f bin/* obj/*
