#!/bin/sh
# $FreeBSD: projects/armv6/tools/regression/lib/msun/test-ctrig.t 226747 2011-10-25 19:47:28Z cognet $

cd `dirname $0`

executable=`basename $0 .t`

make $executable 2>&1 > /dev/null

exec ./$executable
