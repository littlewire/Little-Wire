TRG = bootloader

SRC = $(TRG).c com.c
OBJ = $(SRC:.c=.o)

CCFLAGS = -Wall -g

all : $(TRG)

%.o : %.c
	gcc $(CCFLAGS) -c $< -o $@

$(TRG) : $(OBJ)    
	gcc $(CCFLAGS) $(OBJ) -o $@

clean:
	rm -f $(OBJ)
	rm -f $(TRG)