#-{ Project Relative Paths }----------------------------------------------------

BIN=./binary
SRC=./src
BHD=./include
LIB=./lib
ARC=./architecture

#-{ Compiler Definitions }------------------------------------------------------

# Compiler
CC=arm-none-eabi-gcc

# Device specific flags [1]
DFLAGS=-mcpu=cortex-m0 -mthumb -msoft-float
# Compiler flags
CFLAGS=$(DFLAGS) -g -c -Wall -Wextra

# Linker
LD=arm-none-eabi-gcc

# Path to linker script
LSCRIPT=$(ARC)/HC32F120x8.ld

# Linker flags
LFLAGS=-T $(LSCRIPT) --specs=nosys.specs

# Object copy (for converting formats)
OBJCOPY=arm-none-eabi-objcopy
OFLAGS=-O ihex

# Final binaries
HEX=$(BIN)/blink.hex
ELF=$(BIN)/blink.elf

# All intermediate object files
OBJ=$(BIN)/main.o $(BIN)/boot.o $(BIN)/init.o
#-- These rules for the finally binaries will usually not require modification

# Convert the ELF into intel hex format
$(HEX): $(ELF)
	$(OBJCOPY) $(OFLAGS) $(ELF) $(HEX)

# Link all intermediate objects into a single executable
$(ELF): $(OBJ)
	$(LD) $(LFLAGS) $(OBJ) -o $(ELF)

#-- These rules will vary depending on the program being built

# Compile the main file
$(BIN)/main.o: $(SRC)/main.c $(BHD)/hc32f1xx_conf.h
	$(CC) $(CFLAGS) $(SRC)/main.c -o $(BIN)/main.o

# Compile the reset handler
$(BIN)/boot.o: $(ARC)/startup_hc32f120.S
	$(CC) $(CFLAGS) $(ARC)/boot_stm32f0xx.S -o $(BIN)/boot.o

$(BIN)/init.o: $(SRC)/hc32_ll.c
	$(CC) $(CFLAGS) $(SRC)/hc32_ll.c -o $(BIN)/init.o
# Delete all intermediate object files
tidy:
	rm $(OBJ)
