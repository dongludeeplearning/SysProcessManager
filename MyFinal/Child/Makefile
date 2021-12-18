COMPILER = gcc
CFLAGS = -Wall -std=gnu99 -g

../bin/Child: Child.c ../common/SysConfigReader.c
	$(COMPILER) $(CFLAGS) Child.c ../common/SysConfigReader.c -o ../bin/Child
	
clean:
	$(RM) -f *.o ../bin/Child