# COMP 2401 - Christine Laurendau - Assignment 4

Name:           Clint Galvez [SN: 101196386]
Date:           24 November 2021
Assignment:     COMP 2401-B Assignment 4
Purpose:        To write a multi-process program in C, in the Ubuntu Linux environment of the
				course VM, that manages and regulates access to the books in a library

**Note: when referring to terminal, it is assumed you are in the assignment4 directory**

## SOURCE AND HEADER FILES

- defs.h
- server.c
- lib.c
- books.c
- client.c
- connect.c
- booksData.txt
- README.txt
- Makefile

## COMPILATION COMMAND

- make

## LAUNCHING AND OPERATING INSTRUCTIONS

1. launch server command:  
	- REGULAR: ./server& 		**Note: the '&' symbol just tells it to run in the background**
	- MEMORY LEAK TEST: valgrind --leak-check=yes ./server
	- enter `ps` into the terminal to see if the server is running
2. launch client command (on a separate terminal):  
	- REGULAR: ./client
	- MEMORY LEAK TEST: valgrind --leak-check=yes ./client
2. Press 0-4 then Return depending on your desired action:
	0. Exit
	1. Print books
	2. Check out a book
		- Enter a book id
	3. Check in a book
		- Enter a book id