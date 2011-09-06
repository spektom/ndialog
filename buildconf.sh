#! /bin/sh

touch install-sh
libtoolize --force
autoheader
aclocal
autoconf
