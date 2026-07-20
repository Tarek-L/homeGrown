# This is a copy of the known tool grep

## How it went

the developement of this started by me just implementing a dynamic array inspired by stb stretchy buffer.

after that i wanted to put it to good use, so i made this jrep program.

first step was that i created a vector of entries, stored each entry from readdir and printed to the console making somthing of a mini ls.

after that noticed that readdir use the same buffer for struct dirent* and i quote from the docs,

> The data returned by readdir() may be overwritten by subsequent calls to readdir() for the same directory stream.

so i just treated each entry in it's corresponding iteration.

after that i implemented jrep_dir which will extract each file name and format it and pass it to jrep_file.

things went smoothly when implementing jrep_file because it just readlines store them in a vect then send it to the main matching function that simply uses strstr.

an interesting thing i got out with is the stat and lstat functions and how does stat follows symlinks unlike lstat.

## TODO

* only thing in mind currently is making jrep multithreaded since it would be a good learning experience.
