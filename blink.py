#!/usr/bin/python3

import time, os, sys

os.chdir("/sys/class/gpio/")
os.system("echo {} > export".format(sys.argv[1]))
os.chdir("gpio{}".format(sys.argv[1]) )
os.system("echo out > direction")
for i in range(20):
  os.system("echo 1 > value")
  time.sleep(1)
  os.system("echo 0 > value")
  time.sleep(0.2)

