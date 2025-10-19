CC = gcc -std=c11 -Wall -Werror -Wextra -pedantic -D_POSIX_C_SOURCE=200809L


all: s21_cat

s21_cat: s21_cat.o file_proc.o flags.o open_file.o
	$(CC) s21_cat.o file_proc.o flags.o open_file.o -o s21_cat

s21_cat.o: s21_cat.c open_file.h file_proc.h flags.h
	$(CC) -c s21_cat.c

file_proc.o: file_proc.c file_proc.h
	$(CC) -c file_proc.c

flags.o: flags.c flags.h
	$(CC) -c flags.c

open_file.o: open_file.c open_file.h
	$(CC) -c open_file.c


clean:
	rm -rf *.o
	rm s21_cat

rebuild: clean all

format:
	clang-format -n *.c *.h
	clang-format -i *.c *.h

test: 
	bash test_cat.sh

	