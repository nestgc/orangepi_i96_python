#!/usr/bin/python3
from signal import signal, SIGTERM, SIGHUP, pause
from rpi_lcd import LCD
import subprocess
import time

lcd = LCD(address=0x27,bus=1)
##lcd = LCD(address=0x3f,bus=0)


while True:
  p = subprocess.run("ip -s link show dev wlan0 | tail -n 4",shell=True,stdout=subprocess.PIPE,universal_newlines=True)
  r = p.stdout.splitlines()
  r = [a.strip() for a in r]
  lcd.text(r[0][:20],1)
  lcd.text(r[1][:20],2)
  ##lcd.text(r[2][:20],3)
  ##lcd.text(r[3][:20],4)
  time.sleep(3)

lcd.clear()

