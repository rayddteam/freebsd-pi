#!/bin/sh
# $FreeBSD: projects/armv6/tools/regression/lib/libc/stdio/test-printbasic.t 232120 2012-02-24 18:39:55Z cognet $

cd `dirname $0`

executable=`basename $0 .t`

make $executable 2>&1 > /dev/null

exec ./$executable
