#!/bin/sh

aclocal -I acinclude
autoheader
automake --foreign --include-deps --add-missing --copy
autoconf

echo "Now you are ready to run ./configure"
