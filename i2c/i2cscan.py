import smbus

s=smbus.SMBus(1)
for i in range(3,127):
  try:
    s.read_byte(i)
    print("\nI2C detected: "+hex(i))
  except:
    print("skip: "+hex(i)+', ',end='',flush=True)


