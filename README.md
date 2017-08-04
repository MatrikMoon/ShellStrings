# README #

This is a small project to make it easier to push strings on the stack for shellcode.
Has both 32 bit and 64 bit options.

A typical insall would look like this:
~~~~
$ git clone https://NetworkAuditor@bitbucket.org/NetworkAuditor/shellstrings.git
$ make
~~~~
A typical use would look like this:
~~~~
$ ./ss "hello world"
~~~~
or
~~~~
$ ./ss "hello again world!" -x64
~~~~
