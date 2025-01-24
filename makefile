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

export OPTIMIZE=-O2 -g

all:
	bear -- clang $(WARNINGS) $(OPTIMIZE) $(SANITIZER) -std=c23 -fPIC -ferror-limit=0 -o object-files/logger.o -c logger.c
	clang $(WARNINGS) $(OPTIMIZE) $(SANITIZER) -std=c23 -fPIC -ferror-limit=0 -shared -o build/liblogger.so object-files/logger.o
