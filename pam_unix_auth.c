/*
 * Copyright Alexander O. Yuriev, 1996.  All rights reserved.
 * NIS+ support by Thorsten Kukuk <kukuk@weber.uni-paderborn.de>
 * Copyright Jan Rękorajski, 1999.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, and the entire permission notice in its entirety,
 *    including the disclaimer of warranties.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * ALTERNATIVELY, this product may be distributed under the terms of
 * the GNU Public License, in which case the provisions of the GPL are
 * required INSTEAD OF the above restrictions.  (This clause is
 * necessary due to a potential bad interaction between the GPL and
 * the restrictions contained in a BSD-style copyright.)
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>

/* indicate the following groups are defined */

#define PAM_SM_AUTH

#define _PAM_EXTERN_FUNCTIONS
#include <security/_pam_macros.h>
#include <security/pam_modules.h>
#include <security/pam_ext.h>

#include "support.h"
#include "log.h"

/*
 * PAM framework looks for these entry-points to pass control to the
 * authentication module.
 */

/* Fun starts here :)

 * pam_sm_authenticate() performs UNIX/shadow authentication
 *
 *      First, if shadow support is available, attempt to perform
 *      authentication using shadow passwords. If shadow is not
 *      available, or user does not have a shadow password, fallback
 *      onto a normal UNIX authentication
 */

#define AUTH_RETURN(X)						\
do {									\
  int _retval = X; \
  name = p = NULL; \
	D(("recording return code for next time [%d]", _retval));			\
	*ret_data = _retval;					\
	pam_set_data(pamh, "unix_setcred_return",		\
			 (void *) ret_data, setcred_free);	\
	D(("done. [%s]", pam_strerror(pamh, _retval)));		\
	return _retval;						\
} while (0)


static void
setcred_free (pam_handle_t *pamh UNUSED, void *ptr, int err UNUSED)
{
  if (ptr) {
		free (ptr);
  }
}

PAM_EXTERN int
pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
	unsigned long long ctrl;
	int retval, *ret_data = NULL;
	const char *name;
	const char *p;

  log_init(PACKAGE_NAME);

	D(("called."));

	ctrl = _set_ctrl(pamh, flags, NULL, NULL, NULL, argc, argv);

	/* Get a few bytes so we can pass our return value to pam_sm_setcred() */
	ret_data = malloc(sizeof(int));
	if (!ret_data) {
		log_msg(LOG_CRIT, "pam_unix_auth: cannot allocate ret_data");
		return PAM_BUF_ERR;
	}

	/* get the user'name' */
	retval = pam_get_user(pamh, &name, NULL);
	if (retval != PAM_SUCCESS) {
		if (retval == PAM_CONV_AGAIN) {
			log_rt_debug(on(UNIX_DEBUG, ctrl), "pam_get_user/conv() function is not ready yet");
			/* it is safe to resume this function so we translate this
			 * retval to the value that indicates we're happy to resume.
			 */
      AUTH_RETURN(PAM_INCOMPLETE);
		}
    log_rt_debug(on(UNIX_DEBUG, ctrl), "could not obtain username");
		AUTH_RETURN(retval);
	}

  /*
    * Various libraries at various times have had bugs related to
    * '+' or '-' as the first character of a user name. Don't
    * allow this characters here.
    */
  if (name == NULL || name[0] == '-' || name[0] == '+') {
    log_msg(LOG_NOTICE, "bad username [%s]", name);
    AUTH_RETURN(PAM_USER_UNKNOWN);
  }
  log_rt_debug(on(UNIX_DEBUG, ctrl), "username [%s] obtained", name);

	/* if blank passwords are allowed check if this user does not have a password... */
  if (off(UNIX__NONULL, ctrl) && try_blankpassword(pamh, ctrl, name)) {
		log_msg(LOG_DEBUG, "user [%s] has blank password; authenticated without it", name);
		AUTH_RETURN(PAM_SUCCESS);
	}

	/* get this user's authentication token */
	retval = pam_get_authtok(pamh, PAM_AUTHTOK, &p , NULL);
	if (retval != PAM_SUCCESS) {
		if (retval != PAM_CONV_AGAIN) {
			log_msg(LOG_CRIT, "auth could not identify password for [%s]", name);
      AUTH_RETURN(retval);
		}
		/*
		 * it is safe to resume this function so we translate this
		 * retval to the value that indicates we're happy to resume.
		 */
		log_rt_debug(on(UNIX_DEBUG, ctrl), "conversation function is not ready yet");
    AUTH_RETURN(PAM_INCOMPLETE);
	}
	D(("user=%s, password=[%s]", name, p));

	/* verify the password of this user */
  AUTH_RETURN(_unix_verify_password(pamh, name, p, ctrl));
}

PAM_EXTERN int
pam_sm_setcred (UNUSED pam_handle_t *pamh, UNUSED int flags, UNUSED int argc, UNUSED const char **argv) {
  D(("called"));
  log_msg(LOG_NOTICE, "pam_setcred is not supported in this module");
  return PAM_SUCCESS;
}
