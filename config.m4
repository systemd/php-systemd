
PHP_ARG_WITH(systemd, enable support for systemd,
[  --with-systemd[=DIR]      Enable systemd])

if test "$PHP_SYSTEMD" != "no"; then

  SEARCH_PATH="/usr /usr/local"
  SEARCH_FOR="/include/systemd/sd-journal.h"

  SYSTEMD_DIR=

  if test "$PHP_SYSTEMD" = "yes"; then
    AC_MSG_CHECKING([for systemd headers in default path])
    for i in $SEARCH_PATH ; do
      if test -r $i/$SEARCH_FOR; then
        SYSTEMD_DIR=$i
        AC_MSG_RESULT(found in $i)
      fi
    done
  else
    AC_MSG_CHECKING([for systemd headers in $PHP_SYSTEMD])
    if test -r $PHP_SYSTEMD/$SEARCH_FOR; then
      SYSTEMD_DIR=$PHP_SYSTEMD
      AC_MSG_RESULT([found])
    fi
  fi

  if test -z "$PHP_SYSTEMD"; then
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Cannot find systemd headers])
  fi

  PHP_ADD_INCLUDE($SYSTEMD_DIR/include)

  LIBNAME=systemd
  LIBSYMBOL=sd_journal_sendv

  if test "x$PHP_LIBDIR" = "x"; then
    PHP_LIBDIR=lib
  fi

  PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  [
    PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $SYSTEMD_DIR/$PHP_LIBDIR, SYSTEMD_SHARED_LIBADD)
  ],[
    AC_MSG_ERROR([wrong systemd version {44.+ is required} or lib not found])
  ],[
    -L$SYSTEMD_DIR/$PHP_LIBDIR 
  ])

  PHP_ADD_EXTENSION_DEP(systemd, sockets, true)
  PHP_SUBST(SYSTEMD_SHARED_LIBADD)
  PHP_NEW_EXTENSION(systemd, systemd.c, $ext_shared)
fi
