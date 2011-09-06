ndialog
=======
Display dialog boxes from shell. 

This is similar to the [dialog][1] utility, with the exception that this one is based on [Newt][2] library (while dialog is based on ncurses).

Prerequisites
--------------
1. libxml2
2. libcurses
3. libslang
4. libnewt
5. libtool
6. autoconf
7. automake
8. g++

Installation
------------
```
buildconf.sh
./configure
make install
```

Usage
-----
`./NDialog dialog.xml`

Look into TestDialog.xml for more information.

[1]: http://en.wikipedia.org/wiki/Dialog_(software)
[2]: http://en.wikipedia.org/wiki/Newt_(programming_library)
