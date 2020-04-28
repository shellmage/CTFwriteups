# Vault

![alt challenge](./vault.png)

when we dm the bot on discord with "start" it asks for a 7 digit code composed of 1 and 0.

![alt lock](./lock.png)

the code is random and it keeps changing so we can't brutforce it, let's check the hint.

![alt hint](./hint.png)

the bot simply checks if the right code is in the input message, so we can just generate a long<br>
message with random values using the provided digits and send it, the discord message length is 2000 chars<br>
so it's long enough to guess the right code.<br>
there are 6 levels with with different code length and different digits<br>
here is a small script that generates the messages.<br>

![script.py](./script.py)
```
#!/usr/bin/python2
import random


def generateCode(number_list, length):
    code = ''
    for i in range(500):
        for j in range(length):
            code += random.choice(number_list)
    print code[:2000]

a = ['0', '1']
b = ['4', '5', '6']
c = ['5', '6', '7', '8', '9']
d = ['1', '2', '3', '4']
e = ['0', '1', '2', '5', '8', '9']


generateCode(a, 7)
generateCode(b, 6)
generateCode(c, 4)
generateCode(d, 5)
generateCode(e, 4)
generateCode(a, 11)
```
# Flag
![alt flag](./flag.png)
