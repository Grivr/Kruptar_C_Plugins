# --- Project details -------------------------------------------------

PROJ    := sample
TARGET  := $(PROJ)

OBJS    := $(PROJ).o

# --- Build defines ---------------------------------------------------

CC      := gcc
LD      := gcc

CFLAGS  := -O2 -s -Wall
#CFLAGS	:= -ggdb -O0 -Wall


.PHONY : build clean run

# --- Build -----------------------------------------------------------
# Build process starts here!

build: $(TARGET).dll


# Compile (step 1)

$(TARGET).dll: $(TARGET).o
	$(CC) $(CFLAGS) -shared -o $(TARGET).dll $(TARGET).o -Wl,--kill-at

$(TARGET).o: $(TARGET).c	
	$(CC)  -c $(CFLAGS) $(TARGET).c



# --- Clean -----------------------------------------------------------

clean : 

	@rm -fv *.dll
	@rm -fv *.o

#EOF
