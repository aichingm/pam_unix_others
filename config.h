
// TODO find the place where crypt is defined in the linux-pam repo probably from configure
#define crypt bigcrypt

#define UNUSED 

// usually defined by config.h via configure.ac
#define _(msgid) (msgid)

// usually defined by linux-pam/libpam/pam_modutil_searchkey.c as BUF_SIZE 8192
#define BUF_SIZE 8192


// TODO find all uses of those and set them
/*
#ifdef HAVE_CRYPT_GENSALT_R
#ifdef HAVE_CRYPT_R
#ifdef HAVE_LIBAUDIT
#ifdef HAVE_LIBXCRYPT
#ifdef HAVE_NIS
#ifdef HAVE_PAM_FAIL_DELAY
#ifdef HAVE_RPCB_GETADDR
#ifdef HAVE_RPCSVC_YPCLNT_H
#ifdef HAVE_RPC_RPC_H
#ifdef HAVE_YP_GET_DEFAULT_DOMAIN
#ifdef HELPER_COMPILE
#ifdef PAM_DEBUG
#ifdef PAM_PATH_RANDOMDEV
#ifdef SA_RESETHAND
#ifdef USE_LCKPWDF
#ifdef WITH_SELINUX
#ifdef USE_ECONF

#if defined HAVE_DNGETTEXT && defined ENABLE_NLS
#if defined(CRYPT_CHECKSALT_AVAILABLE) && CRYPT_CHECKSALT_AVAILABLE
#if defined(CRYPT_GENSALT_IMPLEMENTS_AUTO_ENTROPY) && CRYPT_GENSALT_IMPLEMENTS_AUTO_ENTROPY
#if defined(CRYPT_GENSALT_OUTPUT_SIZE) && CRYPT_GENSALT_OUTPUT_SIZE > 64
#if defined(HAVE_RPCB_GETADDR)
#if defined(HAVE_YP_GET_DEFAULT_DOMAIN) && defined (HAVE_YP_BIND) && defined (HAVE_YP_MATCH) && defined (HAVE_YP_UNBIND)
#if defined(USE_LCKPWDF) && !defined(HAVE_LCKPWDF)
#if defined(HAVE_GETLINE)


#elif defined (HAVE_GETDELIM)
#elif defined(HAVE_CRYPT_H)
#elif defined(HAVE_GETDOMAINNAME)



 
#ifndef ASM_MD5
#ifndef HELPER_COMPILE
#ifndef HIGHFIRST
#ifndef MD5_H
#ifndef PAM_DATA_REPLACE
#ifndef SA_RESETHAND
#ifndef _PAM_UNIX_SUPPORT_H
#ifndef _YPPASSWD_H_
#ifndef __sgi


usage CHKPWD_HELPER
usage UNUSED
usage HELPER_COMPILE
usage _
usage BUF_SIZE

*/

