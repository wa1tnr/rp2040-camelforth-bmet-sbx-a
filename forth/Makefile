ARMGNU	= arm-none-eabi
XCPU	= -mcpu=cortex-m0
AOPS	= --warn --fatal-warnings $(XCPU)
COPS	= -Wall -Wextra -O2 -ffreestanding $(XCPU) -fpic -mthumb
LOPS	= -nostartfiles
UOPS	= #-f -n

MKUF2	= mkuf2
LINKER	= memory.ld
TARGET	= forth-picobin-aa
DEST_DIR = ./local-RPI_RP2
BUILT_UF2 = $(TARGET).uf2

ASM		= $(wildcard src/*.s)
SRC 	= $(wildcard src/*.c)
INC		= $(wildcard inc/*.h)
OBJA	= $(ASM:src/%.s=build/%.o)
OBJC	= $(SRC:src/%.c=build/%.o)


all: build $(TARGET).bin $(TARGET).list $(TARGET).uf2

run: build $(TARGET).bin $(TARGET).list $(TARGET).uf2
	pirun.sh

xupload: clean all
	for index in $(BUILT_UF2) ; do echo $${index} ; \
	cp -p $${index} $(DEST_DIR) ; \
	echo $${index} was copied to $(DEST_DIR) ; done

upload: all
	@sleep 1
	@echo ' running target' '[0;1;34mmake upload[0m' now
	@sleep 1
	@echo -n " [0;1;33;40msleeping 2 seconds.."
	@sleep 2
	@echo    ".  Done.[0m"
	@for index in $(BUILT_UF2) ; do echo -n ' ' ; \
	cp -p $${index} $(DEST_DIR) ; \
	echo $${index} was copied to $(DEST_DIR) ; done

clean :
		@echo " Clean"
		@rm -rf build
		@rm -f $(TARGET).*


$(TARGET).uf2 : $(TARGET).bin
	@echo " Create\t\t" $@
	@./$(MKUF2) -i $< -o $@ $(UOPS)

$(TARGET).list: $(TARGET).elf
	@echo " Create\t\t" $@
	@$(ARMGNU)-objdump -D $< > $@

$(TARGET).bin: $(TARGET).elf
	@echo " Create\t\t" $@
	@$(ARMGNU)-objcopy -O binary $< $@

$(TARGET).elf : $(LINKER) $(OBJA) $(OBJC)
	@echo " Create\t\t" $@
	@$(ARMGNU)-gcc $(COPS) $(LOPS) -T $(LINKER) $(OBJA) $(OBJC) -o $@

$(OBJA): build/%.o : src/%.s $(INC)
	@echo " Compile\t" $<
	@$(ARMGNU)-as $(AOP) $< -o $@

$(OBJC): build/%.o : src/%.c $(INC)
	@echo " Compile\t" $<
	@$(ARMGNU)-gcc $(COPS) -Iinc -c $< -o $@

build:
	@echo " Create\t\t" $@
	@mkdir build
