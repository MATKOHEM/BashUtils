CC=gcc
CFLAGS=-Wall -Wextra -Werror

all : s21_cat

clean:
	rm -rf *.o
	rm -f s21_cat

rebuild: clean style all

s21_cat: s21_cat.c
	$(CC) $(CFLAGS) s21_cat.c -o s21_cat

style: s21_cat.c
	cp -f ../../materials/linters/.clang-format ./
	clang-format -i -style=Google *.c *.h
