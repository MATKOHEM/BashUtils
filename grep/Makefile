CC=gcc
CFLAGS=-Wall -Wextra -Werror
	
all: s21_grep

clean:
	rm -rf *.o
	rm -f s21_grep

rebuild: clean style all

s21_grep: s21_grep.c
	$(CC) $(CFLAGS) s21_grep.c -o s21_grep
	
style: s21_grep.c
	cp -f ../../materials/linters/.clang-format ./
	clang-format -i -style=Google *.c *.h
