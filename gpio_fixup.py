#!/usr/bin/python3

# a program to correct the GPIO settings to match the values in the i96 bus spec
#
# Pat Beirne <patb@pbeirne.com> 2021
#
# this script applies only to the OrangePi-i96, booting from u-boot 2012.04.442

from mmap import mmap
import os, sys, time, struct

# assert the i96 GPIO pins into GPIO mode
PORTC_GPIO_MASK = 0           # no pins need changing
PORTA_GPIO_MASK = 0x7e508000  # rda gpioA 15,20,22,25-30 (i96 gpio group)
PORTB_GPIO_MASK = 0           # no pins need changing
PORTD_GPIO_MASK = 0xc         # rda gpioD 2,3 (i96 gpioi group)
PORTB_GPIO_MASK_NO_CTSRTS = 0x300  # assert B 8,9 to reuse uart2_cts,rts

# clear the i96 GPIO pins to special function mode
PORTC_SF_MASK = 0xfffffe3f  # clear bits C c6,7,8 (i96 uarts)
PORTA_SF_MASK = 0xffff91a0  # clear bits A 0-4,6,9-11,13,14 (i96 i2c, spi, i2s)
PORTB_SF_MASK = 0xfffffc3f  # clear bits B 6-9 (i96 i2c, uart)
PORTD_SF_MASK = 0xFFFFFFFF  # nothing to change

PORTC_IOMUX = 0x11a09008
PORTA_IOMUX = 0x11a0900c
PORTB_IOMUX = 0x11a09010
PORTD_IOMUX = 0x11a09014

PAGE_MASK   = ~0xFFF
PAGE_OFFSET = 0xFFF

# we must read and write 4 bytes at a time, little-endian
def get_word(mapfile, address):
  address &= PAGE_OFFSET
  return struct.unpack("<L", mapfile[address:address+4])[0]

def put_word(mapfile, address, data):
  address &= PAGE_OFFSET
  mapfile[address:address+4] = struct.pack("<L",data)

if __name__ == "__main__":
  print("OrangePi-i96 fixup GPIO pins")
  print("Version 1.0")

  try:
    with open("/dev/mem","r+b") as m:
      mem = mmap(m.fileno(), 32, offset = PORTC_IOMUX & PAGE_MASK)
      # set the GPIO pins up (to 1)
      put_word(mem, PORTC_IOMUX, get_word(mem, PORTC_IOMUX) | PORTC_GPIO_MASK)
      put_word(mem, PORTA_IOMUX, get_word(mem, PORTA_IOMUX) | PORTA_GPIO_MASK)
      put_word(mem, PORTB_IOMUX, get_word(mem, PORTB_IOMUX) | PORTB_GPIO_MASK)
      put_word(mem, PORTD_IOMUX, get_word(mem, PORTD_IOMUX) | PORTD_GPIO_MASK)

      # and set the special function bit down (to 0)
      put_word(mem, PORTC_IOMUX, get_word(mem, PORTC_IOMUX) & PORTC_SF_MASK)
      put_word(mem, PORTA_IOMUX, get_word(mem, PORTA_IOMUX) & PORTA_SF_MASK)
      put_word(mem, PORTB_IOMUX, get_word(mem, PORTB_IOMUX) & PORTB_SF_MASK)

      print("GPIO pins corrected to agree with the i96 bus spec")

  except PermissionError:
    print("failed to open /dev/mem......you must execute this script as root")
    exit(2)

