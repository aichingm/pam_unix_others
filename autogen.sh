#!/bin/sh -x

mkdir tools
touch tools/install.sh
mkdir tools/m4
autoreconf -v
