#!/usr/bin/env python3

# This is an example usage of the ESP32 controlled thermal printer API.
# It will simply print text from STDIN to the printer.
#
# The communication protocol doesn't use HTTP or any other standard
# protocol to make the ESP32 code as simple as possible.
# This is why we are not using a simple REST API here.
#
# The protocol is quite simple: the client sends a string of bytes which will be
# printed on the sharp CE-125. The string is terminated by a newline character
# and can be any length, but giving the printer too much text at once will cause
# it to word wrap to 24 characters per line.
#
# The sharp CE-125 printer also doesn't seem to use any standard modern
# encoding. I'll have to do some more research to figure out what it is.
# For now, it is possible to print a character table and use that to figure out
# what bytes to send to the printer.
# Here is an example to print a right arrow, 27 being its character code in
# decimal:
#   rightarrow = 27.to_bytes(1, byteorder='big')
#   s.send(rightarrow + b'\n')
#
# The ESP32 should reply with the string it received (i.e. the string that was
# printed). This is useful for making sure the entire string was received by the
# ESP32 (though it doesn't provide any guarantee over whether the string was
# printed or not).

# The IP address or hostname of the ESP32
HOST = "192.168.1.13"
PORT = 80


import fileinput
import socket

print("Reading from STDIN. Press CTRL+D to exit.")

# Create a socket object
s = socket.socket()

# Connect to the server
s.connect((HOST, PORT))

max_width = 24

def printLine(line):
    line = line.encode()
    s.send(line)
    # Check that the line was sent back (i.e. received by the ESP32)
    if recvline() != line:
        print("Error: line not printed correctly")

def recvline():
    line = b''
    while True:
        char = s.recv(1)
        line += char
        if char == b'\n':
            return line


for line in fileinput.input():
    printLine(line)

s.close()
