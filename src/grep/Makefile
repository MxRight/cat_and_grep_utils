CC = gcc -std=c11 -Wall -Werror -Wextra -pedantic -D_POSIX_C_SOURCE=200809L


all: s21_grep

s21_grep: s21_grep.o file_proc.o flags.o
	$(CC) s21_grep.o file_proc.o flags.o -o s21_grep

s21_grep.o: s21_grep.c file_proc.h flags.h
	$(CC) -c s21_grep.c

file_proc.o: file_proc.c file_proc.h
	$(CC) -c file_proc.c

flags.o: flags.c flags.h
	$(CC) -c flags.c



clean:
	rm -rf *.o
	rm s21_grep


rebuild: clean all

format:
	clang-format -n *.c *.h 
	clang-format -i *.c *.h

test:
	bash test_grep.sh

	