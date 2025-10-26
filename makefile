##
# Logger
#
# @file
# @version 0.1

export WARNINGS+= -Wall
export WARNINGS+= -Wextra
export WARNINGS+= -Weverything
export WARNINGS+= -Wno-unsafe-buffer-usage
export WARNINGS+= -Wno-declaration-after-statement
export WARNINGS+= -Wno-missing-noreturn
export WARNINGS+= -Wno-padded
export WARNINGS+= -Wno-switch-default
export WARNINGS+= -Wno-double-promotion
export WARNINGS+= -Wno-pre-c23-compat

ifdef SANITIZE
export SANITIZER+= -fsanitize=address
export SANITIZER+= -fsanitize-address-use-after-return=always
export SANITIZER+= -fno-omit-frame-pointer
endif #SANITIZE

export OPTIMIZE=-O0 -g2

run: main.o
	./main.o

main.o: main.c logger.h makefile
	bear -- clang $(WARNINGS) $(OPTIMIZE) $(SANITIZER) -std=gnu23 -ferror-limit=0 -o main.o main.c
