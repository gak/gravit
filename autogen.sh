#!/bin/sh

# The GNU standard wants to see some extra file in the top-level directory
# (and if not found automake may report an error)
touch AUTHORS NEWS README ChangeLog

# build configure script
aclocal -I acinclude
autoheader
automake --foreign --include-deps --add-missing --copy
autoconf

echo "Now you are ready to run ./configure"
