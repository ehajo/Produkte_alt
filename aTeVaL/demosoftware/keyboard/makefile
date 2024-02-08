# CPU-Typ:
MCU = atmega8

# CPU-Frequenz
F_CPU =  8000000

# Ausgabeformat
FORMAT = ihex

# Zielname
TARGET = keyboard

# C-Datein hier auflisten:
SRC = $(TARGET).c

# Assembler-Dateien hier auflisten:
ASRC = 

# Optimierung
OPT = s

# Debugmodus
DEBUG = 	#dwarf-2

# Zusaetzliche Verzeichnisse
#EXTRAINCDIRS += verzeichnis/


# C-Standard
# c89   - "ANSI" C
# gnu89 - c89 plus GCC Erweiterungen
# c99   - ISO C99 standard
# gnu99 - c99 plus GCC Erweiterungen
CSTANDARD = -std=gnu99

# Compilerdefinitionen
CDEFS = -DF_CPU=$(F_CPU)UL

# Compilerincludes
CINCS =


# Compiler flags.
#  -g*:          Debug-Informationen
#  -O*:          Optimierungslevel
#  -f...:        Tuning
#  -Wall...:     Warnlevel
#  -Wa,...:      Assembler
#    -adhlns...: Erzeuge Assembler-listening
CFLAGS = -g$(DEBUG)
CFLAGS += $(CDEFS) $(CINCS)
CFLAGS += -O$(OPT)
CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -Wall -Wstrict-prototypes
#CFLAGS += -Wchar-subscripts -Wsign-compare
## Zusatz Flags
CFLAGS += -fno-inline-small-functions
#CFLAGS += -finline-small-functions
CFLAGS += -fno-split-wide-types
CFLAGS += -fno-tree-scev-cprop
CFLAGS += -fno-move-loop-invariants
CFLAGS += -ffunction-sections -fdata-sections -Wl,--gc-sections
CFLAGS += -Wl,--relax
CFLAGS += -Wa,-adhlns=$(<:.c=.lst)
CFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))
CFLAGS += $(CSTANDARD)

# Assemblerflags
ASFLAGS = -Wa,-adhlns=$(<:.S=.lst),-gstabs 

# Externe Speicheroptionen
EXTMEMOPTS =

# Linker flags
#  -Wl,...:     Weiterreichen an den Linker
#    -Map:      Erzeuge map-Datei
#    --cref:    Querverweise im map-File erzeugen
LDFLAGS = -Wl,-Map=$(TARGET).map,--cref
LDFLAGS += $(EXTMEMOPTS)
LDFLAGS += $(PRINTF_LIB) $(SCANF_LIB) $(MATH_LIB)

# Programmiersupport mit avrdude
# Gardware: alf avr910 avrisp bascom bsd 
# dt006 pavr picoweb pony-stk200 sp12 stk200 stk500

AVRDUDE_PROGRAMMER = avrispmkII

# Port wo der Programmer dranhaengt
AVRDUDE_PORT = usb

# Schreibbefehl
AVRDUDE_WRITE_FLASH = -U flash:w:$(TARGET).hex:i
#AVRDUDE_WRITE_EEPROM = -U eeprom:w:$(TARGET).eep

AVRDUDE_FLAGS = -p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER)
AVRDUDE_FLAGS += $(AVRDUDE_NO_VERIFY)
AVRDUDE_FLAGS += $(AVRDUDE_VERBOSE)
AVRDUDE_FLAGS += $(AVRDUDE_ERASE_COUNTER)



# ---------------------------------------------------------------------------

# Define programs and commands.
SHELL = sh
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size
NM = avr-nm
AVRDUDE = avrdude
REMOVE = rm -f
COPY = cp

# Define Messages
# English
MSG_ERRORS_NONE = Fehler: keine
MSG_BEGIN = -------- Start --------
MSG_END = --------  Ende  --------
MSG_SIZE_BEFORE = Speicherbedarf davor: 
MSG_SIZE_AFTER = Speicherbedarf danach:
MSG_COFF = Konvertiere zu AVR COFF:
MSG_EXTENDED_COFF = Konvertiere zu AVR Extended COFF:
MSG_FLASH = Erstelle Flash-Datei:
MSG_EEPROM = Erstelle EEPROM-Datei:
MSG_EXTENDED_LISTING = Erstelle erweitertes Listing:
MSG_SYMBOL_TABLE = Erstelle Symboltabelle:
MSG_LINKING = Verlinke:
MSG_COMPILING = Kompiliere:
MSG_ASSEMBLING = Assembliere:
MSG_CLEANING = Projekt aufraeumen:

# Define all object files.
OBJ = $(SRC:.c=.o) $(ASRC:.S=.o) 

# Define all listing files.
LST = $(ASRC:.S=.lst) $(SRC:.c=.lst)

# Compiler flags to generate dependency files.
GENDEPFLAGS = -Wp,-M,-MP,-MT,$(*F).o,-MF,.dep/$(@F).d

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS = -mmcu=$(MCU) -I. $(CFLAGS) $(GENDEPFLAGS)
ALL_ASFLAGS = -mmcu=$(MCU) -I. -x assembler-with-cpp $(ASFLAGS)

# Default target.
all: begin gccversion sizebefore build sizeafter finished end

build: elf hex eep lss sym

elf: $(TARGET).elf
hex: $(TARGET).hex
eep: $(TARGET).eep
lss: $(TARGET).lss 
sym: $(TARGET).sym

# Damits schoen aussieht:
begin:
	@echo
	@echo $(MSG_BEGIN)

end:
	@echo $(MSG_END)
	@echo


# Zeige Dateigroesse:
HEXSIZE = $(SIZE) --target=$(FORMAT) $(TARGET).hex
ELFSIZE = $(SIZE) --target=elf32-avr $(TARGET).elf # --mcu=$(MCU)

sizebefore:
	@if test -f $(TARGET).elf; then echo; echo $(MSG_SIZE_BEFORE); $(ELFSIZE); \
	2>/dev/null; echo; fi

sizeafter:
	@if test -f $(TARGET).elf; then echo; echo $(MSG_SIZE_AFTER); $(ELFSIZE); \
	2>/dev/null; echo; fi



# Zeige compiler version informationen.
gccversion : 
	@$(CC) --version

# Programmiere Chip
program: $(TARGET).hex $(TARGET).eep
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH) $(AVRDUDE_WRITE_EEPROM)

# Konvertiere ELF zu COFF fuer debugging/simulieren im AVR-Studio
COFFCONVERT=$(OBJCOPY) --debugging \
--change-section-address .data-0x800000 \
--change-section-address .bss-0x800000 \
--change-section-address .noinit-0x800000 \
--change-section-address .eeprom-0x810000 

coff: $(TARGET).elf
	@echo
	@echo $(MSG_COFF) $(TARGET).cof
	$(COFFCONVERT) -O coff-avr $< $(TARGET).cof

extcoff: $(TARGET).elf
	@echo
	@echo $(MSG_EXTENDED_COFF) $(TARGET).cof
	$(COFFCONVERT) -O coff-ext-avr $< $(TARGET).cof

# Erzeuge Ausgabedateien (.hex, .eep) von der ELF-Datei.
%.hex: %.elf
	@echo
	@echo $(MSG_FLASH) $@
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

%.eep: %.elf
	@echo
	@echo $(MSG_EEPROM) $@
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 -O $(FORMAT) $< $@

# Erstelle erweitertes Listing von der ELF-Datei.
%.lss: %.elf
	@echo
	@echo $(MSG_EXTENDED_LISTING) $@
	$(OBJDUMP) -h -S $< > $@

# Erstelle Symboltabelle von der ELF-Datei
%.sym: %.elf
	@echo
	@echo $(MSG_SYMBOL_TABLE) $@
	$(NM) --size-sort --print-size $< > $@


# Link:  Erstelle ELF-Datei von Object-Dateien.
.SECONDARY : $(TARGET).elf
.PRECIOUS : $(OBJ)
%.elf: $(OBJ)
	@echo
	@echo $(MSG_LINKING) $@
	$(CC) $(ALL_CFLAGS) $(OBJ) --output $@ $(LDFLAGS)


# Kompiliere: Erstelle Objektdateien von den C-Codes.
%.o : %.c
	@echo
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(ALL_CFLAGS) $< -o $@ 

# Compile: Erstelle Assemblercode von den C-Codes.
%.s : %.c
	$(CC) -S $(ALL_CFLAGS) $< -o $@


# Assemble: Erstelle Objekte vom Assembler-Code.
%.o : %.S
	@echo
	@echo $(MSG_ASSEMBLING) $<
	$(CC) -c $(ALL_ASFLAGS) $< -o $@

# Target: Aufraeumen.
clean: begin clean_list finished end

clean_list :
	@echo
	@echo $(MSG_CLEANING)
#	$(REMOVE) $(TARGET).hex
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
	$(REMOVE) .dep/*

# Includiere Abhaengigkeiten.
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

# Listing PHONY.
.PHONY : all begin finish end sizebefore sizeafter gccversion \
build elf hex eep lss sym coff extcoff \
clean clean_list program

