#
# Filesystem related tools
#
# $FreeBSD: projects/armv6/tools/bsdbox/Makefile.fs 229675 2012-01-06 00:56:31Z adrian $

# mfs
CRUNCH_PROGS_sbin+=	mdmfs mdconfig newfs
CRUNCH_ALIAS_mdmfs=	mount_mfs

# UFS
CRUNCH_PROGS_sbin+=	fsck_ffs
CRUNCH_LIBS+= -lgeom
CRUNCH_LIBS+= -lufs

# msdos
# CRUNCH_PROGS_sbin+=	mount_msdosfs
# CRUNCH_LIBS+= -lkiconv
