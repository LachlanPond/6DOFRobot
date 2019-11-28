mainfile	= 6dofroboticarm
mcu			= atmega328p
clock		= 16000000
programmer	= usbtiny
fuses		= -U lfuse:w:0xff:m

flash_program: build_files
	sudo avrdude -c ${programmer} -p ${mcu} -U flash:w:${mainfile}.hex ${fuses}

build_files:
	avr-gcc -g -Os -mmcu=${mcu} -DF_CPU=${clock} functions.c ${mainfile}.c -o ${mainfile}.elf
	avr-objcopy -O ihex -R .eeprom ${mainfile}.elf ${mainfile}.hex