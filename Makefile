CC=gcc
FLAGS=-g -Wall -lpng

OBJDIR=objs
C_SOURCE=$(shell find src/* -name \*.c)
DIR=$(dir $(C_SOURCE))
OBJ_FILE=$(addprefix $(OBJDIR)/, $(patsubst %.c, %.o, $(C_SOURCE)))
BINDIR=$(dir $(OBJ_FILE))

PRJC=vlook

default:
	@[ -d $(OBJDR) ]  | mkdir -vp $(OBJDIR)
	@[ -d "$(BINDIR)" ] | mkdir -pv $(BINDIR)
	@echo $(C_SOURCE) $(OBJ_FILE)
	@make $(PRJC)

$(PRJC):$(OBJ_FILE)
	$(CC) -o $@ $^ $(FLAGS)

$(OBJDIR)/%.o:%.c
	$(CC) -o $@ -c $< $(FLAGS)

clean:
	rm $(PRJC) -r $(OBJDIR)

all:
	@make clean
	make
