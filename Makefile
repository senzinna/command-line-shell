all:
	gcc -Wall  myshell.c utility.c -o myshell
clean:
	rm myshell
