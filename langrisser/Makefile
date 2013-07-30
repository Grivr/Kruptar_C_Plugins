# --- Project details -------------------------------------------------

PROJ    := langrisser
TARGET  := $(PROJ)

OBJS    := $(PROJ).o

# --- Build defines ---------------------------------------------------

CC      := gcc
LD      := gcc

ifeq ($(MODE), DEBUG)
	CFLAGS	:= -ggdb -O0 -Wall
else
	CFLAGS  := -O2 -s -Wall
endif


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

# --- Run -----------------------------------------------------------
# Run app
run: 
	@echo "Do nothing for dll!"

lindent:
	lindent $(TARGET).c

#EOF
