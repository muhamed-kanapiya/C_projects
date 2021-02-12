#-*- MakeFile -*-

all: my_ls
	
my_ls: my_ls.c myHeader.h
	gcc -g -fsanitize=address my_ls.c myHeader.h -o my_ls

clean:
	rm *.o *.h.gch my_ls my_safe test core.* a.out