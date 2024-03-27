usr_app : usr_app.o auto_g.o hadlock.o stack.o
	gcc usr_app.o auto_g.o hadlock.o stack.o -o usr_app
	
usr_app.o : usr_app.c def.h
	gcc -c usr_app.c -g
	
auto_g.o : auto_g.c def.h
	gcc -c auto_g.c
	
hadlock.o : hadlock.c def.h
	gcc -c hadlock.c -g
	
stack.o : stack.c def.h
	gcc -c stack.c -g
	
clean :
	-rm usr_app usr_app.o auto_g.o hadlock.o stack.o
