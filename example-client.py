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
HOST = "192.168.1.24"
PORT = 80


import fileinput
import socket

print("Reading from STDIN. Press CTRL+D to exit.")

# Create a socket object
s = socket.socket()

# Connect to the server
s.connect((HOST, PORT))

max_width = 24

char_table = {
    "→": b"\x1b", # right arrow
    "←": b"\x1c", # left arrow
    "↑": b"\x1d", # up arrow
    "↓": b"\x1e", # down arrow

    "▒": b"\x7f", # block

    "♪": b"\x8d", # music note

    "ä": b"\xe0", # a umlaut
    "ë": b"\xe1", # e umlaut
    "ü": b"\xe2", # u umlaut
    "ï": b"\xe3", # i umlaut
    "ö": b"\xe4", # o umlaut
    "à": b"\xe5", # a grave
    "è": b"\xe6", # e grave
    "ù": b"\xe7", # u grave
    "â": b"\xe8", # a circumflex
    "ê": b"\xe9", # e circumflex
    "û": b"\xea", # u circumflex
    "î": b"\xeb", # i circumflex
    "ô": b"\xec", # o circumflex
    "é": b"\xed", # e acute
    "ç": b"\xee", # c cedilla
    "ß": b"\xef", # eszett

    "0̷": b"\xf0", # slashed zero
    
    "♠": b"\xf5", # spade
    "♥": b"\xf6", # heart
    "♦": b"\xf7", # diamond
    "♣": b"\xf8", # club

    "α": b"\xf9", # alpha
    "β": b"\xfa", # beta
    "γ": b"\xfb", # gamma
    "σ": b"\xfc", # sigma
    "λ": b"\xfd", # lambda
    "δ": b"\xfe", # delta
    "£": b"\xff", # pound
}

def encode(text):
    # See https://en.wikipedia.org/wiki/Sharp_pocket_computer_character_sets (CE-126 table seems to be the same as CE-125)
    encoded = b""
    for c in text:
        # TODO: check for both lower and upper case version of the character
        if c in char_table:
            encoded += char_table[c]
        elif c.lower() in char_table:
            encoded += char_table[c.lower()]
        else:
            encoded += c.encode('ascii')
    return encoded

def printLine(line):
    line = encode(line)
    s.send(line)
    # Check that the line was sent back (i.e. received by the ESP32)
    received_line = recvline()
    if received_line != line:
        print("Error: line not printed correctly")
        print("Sent:", line)
        print("Received:", received_line)

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
