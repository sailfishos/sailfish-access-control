prefix=@PREFIX@
libdir=@LIBDIR@
includedir=@INCDIR@

Name: Sailfish Access Control
Description: Sailfish Access Control library
Version: @VERSION@
Requires: glib-2.0
Requires.private:
Cflags: -I@INCDIR@
Libs: -L@LIBDIR@ -lsailfishaccesscontrol
