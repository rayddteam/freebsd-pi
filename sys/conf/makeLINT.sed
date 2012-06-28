#!/usr/bin/sed -E -n -f
# $FreeBSD: projects/armv6/sys/conf/makeLINT.sed 226747 2011-10-25 19:47:28Z cognet $

/^(machine|files|ident|(no)?device|(no)?makeoption(s)?|(no)?option(s)?|profile|cpu|maxusers)[[:space:]]/ {
    s/[[:space:]]*#.*$//
    p
}
