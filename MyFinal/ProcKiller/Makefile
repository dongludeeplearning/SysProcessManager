COMPILER = gcc
CFLAGS = -Wall -std=gnu99 -g

../bin/ProcKiller: ProcKiller.c ../common/SysConfigReader.c
	$(COMPILER) $(CFLAGS) ProcKiller.c ../common/SysConfigReader.c -o ../bin/ProcKiller
	
clean:
	$(RM) -f *.o ../bin/ProcKiller