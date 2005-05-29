#!/bin/sh -x

aclocal
autoheader
automake --add-missing
autoconf

set +x
echo "Now you are ready to run ./configure"
