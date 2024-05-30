# MD2 Hash in C

Usage:
```
$ ./md2 input_str
```
where `input_str` is either one continuous string or a multiword string enclosed in quotations.

```
$ cat file | ./md2
```
to read in a file and
```
$ ./md2
```
where the file will accept input until EOF (i.e. ^D or ctrl+D) is detected.

sources: 
 - https://nickthecrypt.medium.com/cryptography-hash-method-md2-message-digest-2-step-by-step-explanation-made-easy-with-python-10faa2e35e85
 - https://datatracker.ietf.org/doc/rfc1319/

Python source code from nickthecrypt is included in md2.py and was then adapted into md2.c.
