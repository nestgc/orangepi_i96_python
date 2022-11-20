#!/usr/bin/python3

import smbus, sys

bus = 1
''' you can call this app with arguments: either a single small digit (0,1,2) or 
    the device name /dev/i2c-1 '''
if len(sys.argv)>1:
  if sys.argv[1].isdigit():
    bus = int(sys.argv[1])
  if sys.argv[1].startswith("/dev/i2c"):
    bus = int(sys.argv[1][-1])

s=smbus.SMBus(bus)

print("Scan I2C bus /dev/i2c-{} for devices".format(bus))
print("     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f",end='')

def check_i2c(n):
  try:
    s.read_byte(n)
    return True
  except:
    return False

for i in range(3,120):
  if (i % 16 == 0) or (i == 3):
    print("\n"+"{:02x}:".format(i),end='',flush=True)
  if i == 3:
    print("         ",end='')
  print(" {:02x}".format(i) if check_i2c(i) else " --",end='',flush=True)
print()

