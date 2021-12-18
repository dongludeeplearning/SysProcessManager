COMPILER = gcc
CFLAGS = -Wall -std=gnu99 -g

../bin/SysManager: SysManager.c ../common/SysConfigReader.c
	$(COMPILER) $(CFLAGS) SysManager.c ../common/SysConfigReader.c -o ../bin/SysManager
	
clean:
	$(RM) -f *.o ../bin/SysManager