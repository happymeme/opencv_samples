#!/bin/sh

usage() {
	echo "Usage: $0 filename [obj_filename]"
}

# arg check
if [ $# -lt 1 ]; then
	usage
	exit 1
fi

# obj filename
if [ -n $2 ]; then
	# 拡張子を取り除く
	OBJ=`echo $1 | sed -e "s/\.[^.]*$//"`
else
	OBJ=$2
fi

# compile
g++ -o $OBJ $1 -I/opt/local/include/opencv  -L/opt/local/lib -lcv -lhighgui -lcvaux -lml -lcxcore  
