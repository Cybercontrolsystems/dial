# Fairly generic cross-compliation makefile for simple programs
CC=$(CROSSTOOL)/$(ARM)/bin/gcc
NAME=dial
CCFLAGS=-lgd -lm

all: $(NAME)
	$(CROSSTOOL)/$(ARM)/bin/strip $(NAME)

$(NAME): $(NAME).c
	$(CROSSTOOL)/$(ARM)/bin/gcc $(NAME).c -o $(NAME) $(CCFLAGS)
