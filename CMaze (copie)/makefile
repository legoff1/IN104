CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c99 -g
INC=-I CMaze-main/
SRC=CMaze-main/
EXEC=mazeEnv
all: $(EXEC)
main: $(SRC)mazeEnv.c $(SRC)article.o
$(CC) $(INC) -o $(SRC)$@ $^ $(CFLAGS)
$(SRC)%.o : $(SRC)%.c
$(CC) $(INC) -o $@ -c $< $(CFLAGS)
clean:
rm -rf $(SRC)*.o 
