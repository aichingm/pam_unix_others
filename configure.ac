dnl Process this file with autoconf to produce a configure script.
m4_define([package_def], [pam_unix_others])
m4_define([version_def], [1])
AC_INIT([package_def], [version_def])
AC_PREREQ([2.61])
AC_CONFIG_AUX_DIR(./tools)
AC_CONFIG_MACRO_DIR([tools/m4])
AC_CONFIG_HEADERS([config.h])

dnl This should be called before any macros that run the C compiler.
AC_USE_SYSTEM_EXTENSIONS


dnl
dnl OPTIONS
dnl
AC_ARG_ENABLE(econf,     AS_HELP_STRING([--enable-econf],     [libeconf]),            opt_econf=$enableval,     opt_econf=no     )
AC_ARG_ENABLE(debugging, AS_HELP_STRING([--enable-debugging], [debugging ouput]),     opt_debugging=$enableval, opt_debugging=no )
AC_ARG_ENABLE(audit,     AS_HELP_STRING([--disable-audit],    [audit support]),       WITH_LIBAUDIT=$enableval, WITH_LIBAUDIT=yes)
AC_ARG_ENABLE(nis,       AS_HELP_STRING([--disable-nis],      [NIS/YP support]),      WITH_NIS=$enableval,      WITH_NIS=yes     )
AC_ARG_ENABLE(selinux,   AS_HELP_STRING([--disable-selinux],  [SELinux support]),     WITH_SELINUX=$enableval,  WITH_SELINUX=yes )

AC_ARG_VAR([helper], [The name of the helper program] )
AC_ARG_VAR([module], [The name of the linkable module])
AC_ARG_VAR([process_group], [The name of group in which the calling process's user has to be])
AC_ARG_VAR([user_group], [The name group in which the authenticated user has to be])


dnl
dnl ARG FIXING
dnl


dnl helper
if test "$helper" = "" ; then
    helper="unix_others_chkpwd"
fi

dnl module
if test "$module" = "" ; then
    module="pam_unix_others.so"
fi

dnl process_group
if test "$process_group" = "" ; then
    process_group="can_puo"
fi

dnl user_group
if test "$user_group" = "" ; then
    user_group="allow_puo"
fi

dnl selinux
if test "$WITH_SELINUX" = "yes" ; then
  AC_CHECK_LIB([selinux], [getfilecon], LIBSELINUX="-lselinux", LIBSELINUX="")
else
  LIBSELINUX=""
fi

dnl crypt
AS_IF([test "x$ac_cv_header_xcrypt_h" = "xyes"], [crypt_libs="xcrypt crypt"], [crypt_libs="crypt"])

dnl audit
if test x"$WITH_LIBAUDIT" != xno ; then
  AC_CHECK_HEADER([libaudit.h], 
                  [AC_CHECK_LIB(audit, audit_log_acct_message, LIBAUDIT=-laudit, LIBAUDIT="")
                    AC_CHECK_TYPE([struct audit_tty_status],
                      [HAVE_AUDIT_TTY_STATUS=yes],
                      [HAVE_AUDIT_TTY_STATUS=""],
                      [#include <libaudit.h>]
                    )
                  ]
                 )
else
  LIBAUDIT=""
fi

dnl econf
if test "$opt_econf" = "yes" ; then
  LIBECONF="-leconf"
else
  LIBECONF=""
fi

dnl
dnl SUBSTITUTIONS
dnl

AC_SUBST([HELPER_NAME], $helper)
AC_SUBST([MODULE_NAME], $module)
AC_SUBST([PROCESS_GROUP], $process_group)
AC_SUBST([USER_GROUP], $user_group)
AC_SUBST([LIBSELINUX])
AC_SUBST([NIS_CFLAGS])
AC_SUBST([NIS_LIBS])
AC_SUBST([LIBCRYPT])
AC_SUBST([LIBAUDIT])
AC_SUBST([VERSION], [version_def])
AC_SUBST([PACKAGE], [package_def])
AC_SUBST([DEBUGGING], $opt_debugging)
AC_SUBST([LIBECONF])

dnl
dnl DEFINITIONS
dnl

AC_DEFINE_UNQUOTED([HELPER_NAME], ["$helper"], [Define the name of the helper programm])
AC_DEFINE_UNQUOTED([MODULE_NAME], ["$module"], [Define the name of the linkable module])
AC_DEFINE_UNQUOTED([PROCESS_GROUP], ["$process_group"], [Define the name of group in which the calling process's user has to be])
AC_DEFINE_UNQUOTED([USER_GROUP], ["$user_group"], [Define the name of the group in which the authenticated user has to be])

if test  "$opt_debugging" = "yes" ; then
  AC_DEFINE_UNQUOTED([DEBUGGING], [], [Define if debugging options should be enabled])
fi

if test ! -z "$LIBSELINUX" ; then
  AC_DEFINE([WITH_SELINUX], 1, [Defined if SE Linux support is compiled in])
fi

if test "$LIBCRYPT" = "-lxcrypt" -a "$ac_cv_header_xcrypt_h" = "yes" ; then
  AC_DEFINE([HAVE_LIBXCRYPT], 1, [Define to 1 if xcrypt support should be compiled in.])
fi

if test ! -z "$LIBAUDIT" -a "$ac_cv_header_libaudit_h" != "no" ; then
  AC_DEFINE([HAVE_LIBAUDIT], 1, [Define to 1 if audit support should be compiled in.])
fi

if test "$opt_econf" = "yes" ; then
  AC_DEFINE([USE_LIBAUDIT], [], [Define to force libeconf support])
fi

if test ! -z "$HAVE_AUDIT_TTY_STATUS" ; then
  AC_DEFINE([HAVE_AUDIT_TTY_STATUS], 1, [Define to 1 if struct audit_tty_status exists.])
fi

AC_DEFINE([UNUSED], __attribute__ ((unused)), [Define the UNUSED helper macro])


dnl
dnl CHECKS
dnl

if test ! -z "$LIBSELINUX" ; then
    BACKUP_LIBS=$LIBS
    LIBS="$LIBS $LIBSELINUX"
    AC_CHECK_FUNCS(setkeycreatecon)
    AC_CHECK_FUNCS(getseuser)
    LIBS=$BACKUP_LIBS
fi

AS_IF([test "x$enable_nis" != "xno"], [
  old_CFLAGS=$CFLAGS
  old_CPPFLAGS=$CPPFLAGS
  old_LIBS=$LIBS

  dnl if there's libtirpc available, prefer that over the system
  dnl implementation.
  PKG_CHECK_MODULES([TIRPC], [libtirpc], [
    CFLAGS="$CFLAGS $TIRPC_CFLAGS"
    CPPFLAGS="$CPPFLAGS $TIRPC_CFLAGS"
    LIBS="$LIBS $TIRPC_LIBS"
  ], [:;])

  PKG_CHECK_MODULES([NSL], [libnsl], [],
    [AC_CHECK_LIB([nsl],[yp_match],[NSL_LIBS="-lnsl"],[NSL_LIBS=""])])
  CFLAGS="$CFLAGS $NSL_CFLAGS"
  CPPFLAGS="$CPPFLAGS $NSL_CFLAGS"
  LIBS="$LIBS $NSL_LIBS"

  AC_CHECK_FUNCS([yp_get_default_domain yperr_string yp_master yp_bind yp_match yp_unbind])
  AC_CHECK_FUNCS([getrpcport rpcb_getaddr])
  AC_CHECK_HEADERS([rpc/rpc.h rpcsvc/ypclnt.h rpcsvc/yp_prot.h])
  AC_CHECK_DECLS([getrpcport], , , [
    #if HAVE_RPC_RPC_H
    # include <rpc/rpc.h>
    #endif
  ])

  CFLAGS="$old_CFLAGS"
  CPPFLAGS="$old_CPPFLAGS"
  LIBS="$old_LIBS"
])

BACKUP_LIBS=$LIBS
AC_SEARCH_LIBS([crypt],[$crypt_libs], LIBCRYPT="${ac_lib:+-l$ac_lib}", LIBCRYPT="")
AC_CHECK_FUNCS(crypt_r crypt_gensalt_r)
LIBS=$BACKUP_LIBS

AC_CHECK_HEADERS(xcrypt.h crypt.h)

if test ! -z "$HAVE_AUDIT_TTY_STATUS" ; then
  AC_CHECK_MEMBERS([struct audit_tty_status.log_passwd], [], 
                    AC_MSG_WARN([audit_tty_status.log_passwd is not available.  The log_passwd option is disabled.]),
                    [[#include <libaudit.h>]]
                  )
fi

dnl Checks for header files.
dnl generate this list with `make ac_headers_list`
AC_CHECK_HEADERS(crypt.h)
AC_CHECK_HEADERS(ctype.h)
AC_CHECK_HEADERS(errno.h)
AC_CHECK_HEADERS(fcntl.h)
AC_CHECK_HEADERS(libaudit.h)
AC_CHECK_HEADERS(libintl.h)
AC_CHECK_HEADERS(limits.h)
AC_CHECK_HEADERS(malloc.h)
AC_CHECK_HEADERS(pwd.h)
AC_CHECK_HEADERS(rpcsvc/ypclnt.h)
AC_CHECK_HEADERS(security/_pam_macros.h)
AC_CHECK_HEADERS(security/_pam_types.h)
AC_CHECK_HEADERS(security/pam_ext.h)
AC_CHECK_HEADERS(security/pam_modules.h)
AC_CHECK_HEADERS(security/pam_modutil.h)
AC_CHECK_HEADERS(selinux/selinux.h)
AC_CHECK_HEADERS(shadow.h)
AC_CHECK_HEADERS(signal.h)
AC_CHECK_HEADERS(stdarg.h)
AC_CHECK_HEADERS(stdio.h)
AC_CHECK_HEADERS(stdlib.h)
AC_CHECK_HEADERS(string.h)
AC_CHECK_HEADERS(sys/resource.h)
AC_CHECK_HEADERS(sys/stat.h)
AC_CHECK_HEADERS(sys/time.h)
AC_CHECK_HEADERS(sys/types.h)
AC_CHECK_HEADERS(sys/wait.h)
AC_CHECK_HEADERS(syslog.h)
AC_CHECK_HEADERS(time.h)
AC_CHECK_HEADERS(unistd.h)
AC_CHECK_HEADERS(utmp.h)
AC_CHECK_HEADERS(xcrypt.h)
AC_CHECK_HEADERS(libeconf.h)


dnl Checks for consts, typedefs, structures.
AC_C_BIGENDIAN
AC_C_CONST
AC_TYPE_UID_T
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_GETGROUPS
AC_STRUCT_TM


dnl Checks for functions.
AC_FUNC_MEMCMP
AC_FUNC_VPRINTF
AC_CHECK_FUNCS(gettimeofday)
AC_CHECK_FUNCS(strdup strtol)
AC_CHECK_FUNCS(getline getdelim)
AC_CHECK_FUNCS(dngettext)


AH_BOTTOM([#ifdef ENABLE_NLS
#include <libintl.h>
#define _(msgid) dgettext(PACKAGE_NAME, msgid)
#define N_(msgid) msgid
#else
#define _(msgid) (msgid)
#define N_(msgid) msgid
#endif
/* Target dependent definitions */
#ifdef BUILDING_MODULE
#ifdef DEBUGGING
#define PAM_DEBUG
#endif
#define CHKPWD_HELPER HELPER_NAME
#endif
#ifdef BUILDING_HELPER
#define HELPER_COMPILE HELPER_NAME
#endif
])


AC_CONFIG_FILES([Makefile])


AC_OUTPUT
