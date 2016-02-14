dnl $Id$
dnl config.m4 for extension foolphp7

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(foolphp7, for foolphp7 support,
Make sure that the comment is aligned:
[  --with-foolphp7             Include foolphp7 support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(foolphp7, whether to enable foolphp7 support,
dnl Make sure that the comment is aligned:
dnl [  --enable-foolphp7           Enable foolphp7 support])

if test "$PHP_FOOLPHP7" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-foolphp7 -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/foolphp7.h"  # you most likely want to change this
  dnl if test -r $PHP_FOOLPHP7/$SEARCH_FOR; then # path given as parameter
  dnl   FOOLPHP7_DIR=$PHP_FOOLPHP7
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for foolphp7 files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       FOOLPHP7_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$FOOLPHP7_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the foolphp7 distribution])
  dnl fi

  dnl # --with-foolphp7 -> add include path
  dnl PHP_ADD_INCLUDE($FOOLPHP7_DIR/include)

  dnl # --with-foolphp7 -> check for lib and symbol presence
  dnl LIBNAME=foolphp7 # you may want to change this
  dnl LIBSYMBOL=foolphp7 # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $FOOLPHP7_DIR/$PHP_LIBDIR, FOOLPHP7_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_FOOLPHP7LIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong foolphp7 lib version or lib not found])
  dnl ],[
  dnl   -L$FOOLPHP7_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(FOOLPHP7_SHARED_LIBADD)

  PHP_NEW_EXTENSION(foolphp7, foolphp7.c fool_application.c fool_loader.c fool_request.c fool_register.c fool_dispatcher.c fool_controller.c fool_view.c fool_exception.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
