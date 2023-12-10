# Microscope RGB lamp driver
# Copyright (C) 2023 Vaclav Hanzl
# See https://github.com/vaclavhanzl/microscope


SIZE_REDUCE= -ffunction-sections -fdata-sections -Wl,--gc-sections

rgb:
	avr-gcc $(SIZE_REDUCE) -mmcu=attiny85 -O -DF_CPU=16000000 WS2812vh.c
	avr-objcopy -O ihex -R .eeprom a.out a.hex

prog:
	~/soft/micronucleus/commandline/micronucleus a.hex --run

ispprog:
	avrdude -c usbtiny -p t85 -U flash:w:a.hex

src:
	avr-objdump -D a.out
