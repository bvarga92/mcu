# projekt nev
TARGET = walkingrobot

# .c forrasfajlok szokozzel elvalasztva
SRC = $(TARGET).c servo.c remote.c gait.c

# .S kiterjesztesu Assembly forrasfajlok szokozzel elvalasztva
ASRC = 

# header konyvtarak
EXTRAINCDIRS = 

# mikrokontroller tipusa
MCU = atmega328p

# kimeneti formatum (srec/ihex/binary)
FORMAT = ihex

# optimalizacios szint (0/1/2/3/s)
OPT = s

# C fordito flagek
CFLAGS = -g -O$(OPT) -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -Wall -Wstrict-prototypes -Wa,-adhlns=$(<:.c=.lst) $(patsubst %,-I%,$(EXTRAINCDIRS)) -std=gnu99

# assembler flagek
ASFLAGS = -Wa,-adhlns=$(<:.S=.lst),-gstabs 

# linker flagek
LDFLAGS = -Wl,-Map=$(TARGET).map,--cref

# minimal printf
#LDFLAGS += -Wl,-u,vfprintf -lprintf_min

# float printf (-lm is kell!)
#LDFLAGS += -Wl,-u,vfprintf -lprintf_flt

# math.h
#LDFLAGS += -lm



########################################################



SHELL = sh
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size
REMOVE = rm -f
COPY = cp
HEXSIZE = $(SIZE) --target=$(FORMAT) $(TARGET).hex
ELFSIZE = $(SIZE) -A $(TARGET).elf
OBJ = $(SRC:.c=.o) $(ASRC:.S=.o)
LST = $(ASRC:.S=.lst) $(SRC:.c=.lst)
ALL_CFLAGS = -mmcu=$(MCU) -I. $(CFLAGS)
ALL_ASFLAGS = -mmcu=$(MCU) -I. -x assembler-with-cpp $(ASFLAGS)

all: begin gccversion sizebefore $(TARGET).elf $(TARGET).hex $(TARGET).eep $(TARGET).lss $(TARGET).sym sizeafter finished end

begin:
	@echo
	@echo -------------------------

finished:
	@echo Finished successfully.

end:
	@echo -------------------------
	@echo

sizebefore:
	@if [ -f $(TARGET).elf ]; then echo; echo Size before: ; $(ELFSIZE); echo; fi

sizeafter:
	@if [ -f $(TARGET).elf ]; then echo; echo Size after: ; $(ELFSIZE); echo; fi

gccversion:
	@$(CC) --version

COFFCONVERT=$(OBJCOPY) --debugging \
	--change-section-address .data-0x800000 \
	--change-section-address .bss-0x800000 \
	--change-section-address .noinit-0x800000 \
	--change-section-address .eeprom-0x810000 

coff: $(TARGET).elf
	@echo
	@echo Converting to AVR COFF: $(TARGET).cof
	$(COFFCONVERT) -O coff-avr $< $(TARGET).cof

extcoff: $(TARGET).elf
	@echo
	@echo Converting to AVR Extended COFF: $(TARGET).cof
	$(COFFCONVERT) -O coff-ext-avr $< $(TARGET).cof

%.hex: %.elf
	@echo
	@echo Creating load file for Flash: $@
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

%.eep: %.elf
	@echo
	@echo Creating load file for EEPROM: $@
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 -O $(FORMAT) $< $@

%.lss: %.elf
	@echo
	@echo Creating Extended Listing: $@
	$(OBJDUMP) -h -S $< > $@

%.sym: %.elf
	@echo
	@echo Creating Symbol Table: $@
	avr-nm -n $< > $@

.SECONDARY : $(TARGET).elf
.PRECIOUS : $(OBJ)
%.elf: $(OBJ)
	@echo
	@echo Linking: $@
	$(CC) $(ALL_CFLAGS) $(OBJ) --output $@ $(LDFLAGS)

%.o : %.c
	@echo
	@echo Compiling: $<
	$(CC) -c $(ALL_CFLAGS) $< -o $@

%.s : %.c
	$(CC) -S $(ALL_CFLAGS) $< -o $@

%.o : %.S
	@echo
	@echo Assembling: $<
	$(CC) -c $(ALL_ASFLAGS) $< -o $@

clean: begin clean_list finished end

clean_list :
	@echo
	@echo Cleaning project:
	$(REMOVE) $(TARGET).hex
	$(REMOVE) $(TARGET).eep
	$(REMOVE) $(TARGET).obj
	$(REMOVE) $(TARGET).cof
	$(REMOVE) $(TARGET).elf
	$(REMOVE) $(TARGET).map
	$(REMOVE) $(TARGET).obj
	$(REMOVE) $(TARGET).a90
	$(REMOVE) $(TARGET).sym
	$(REMOVE) $(TARGET).lnk
	$(REMOVE) $(TARGET).lss
	$(REMOVE) $(OBJ)
	$(REMOVE) $(LST)
	$(REMOVE) $(SRC:.c=.s)
	$(REMOVE) $(SRC:.c=.d)

%.d: %.c
	set -e; $(CC) -MM $(ALL_CFLAGS) $< | sed 's,\(.*\)\.o[ :]*,\1.o \1.d : ,g' > $@; [ -s $@ ] || rm -f $@

-include $(SRC:.c=.d)

.PHONY : all begin finish end sizebefore sizeafter gccversion coff extcoff clean clean_list
