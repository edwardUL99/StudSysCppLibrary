# StudSysCppLibrary
The library for a Student Management System coded in C++

This repository provides the library for the code behind the Student Management System. It provides a means for separation of backend implementation from ui implementation. This has been done so the library can be used with any UI, whether it is CLI or GUI, rather than duplicating code

# How to compile
Change directory into the root of project you cloned and open a terminal there and type make.
You can then copy the libstudsys.so file to /usr/lib/studsys or in a separate directory and specify the path to it with -L<path> flag

# How to use
You can copy all the headers into a studsys folder in /usr/include or into a different directory and then include the headers required
When compiling the UI code make sure you specify -lstudsys and if necessary -L<path> where path is the location of the libstudsys.so
