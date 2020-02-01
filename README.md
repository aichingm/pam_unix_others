# Pam unix others

This project provides a linux-pam authentication module very similar to the pam_unix module (in fact this was originally a fork) but it provides only the auth module and in the auth module only the `authenticate` method. The `pam_setcred` has no effect on the system and only returns `PAM_SUCCESS`.

The main difference between pam_unix and pam_unix_others is that pam_unix_others allows programs, which are ran with a user which is in the same group as owning group of the helper (`unix_others_chkpwd`)  to authenticate *any* user via the /etc/passwd/ and /etc/shadow files **without** being ran as root. In addition to the limitation for for the user under which the program is ran also the helper only allows to authenticate users which are in the `puo_auth` group are.

## Why? What should this be used for?

My decision to  work on this project was that I wanted to authenticate users for a web based git interface via the local Linux user database. The application in question had pam support already built in but stated on there website:

> To configure PAM, set the 'PAM Service Name' to a filename in `/etc/pam.d/`. To work with normal Linux passwords, the user running [application] must have read access to `/etc/shadow`.

Since only root should have read access to `/etc/shadow` either the protection of the shadow file has to be lowered or the application has to run as root. Neither of both seams reasonable. The solution to this is to allow a pam module to authenticate certain users for certain users/processes.

### How would such a setup look like?

`cat /etc/pam.d/unix_others`

```
#%PAM-1.0

auth      required  pam_unix_others.so try_first_pass nullok  audit
```

`ls -l /usr/bin/unix_others_chkpwd`

```
-rwsr-sr-x 1 root can_puo 45048 Feb  2 00:24 /usr/local/bin/unix_others_chkpwd
```

`ls -l /usr/lib/security/pam_unix_others.so`

```
-rwxr-xr-x 1 root root 53984 Feb  2 00:24 /usr/lib/security/pam_unix_others.so
```

## Build
```bash
./autogen.sh
./configure
make artifacts
```

The built artifacts are `pam_unix_others.so` and `unix_others_chkpwd` unless otherwise set by `configure` or `make`. 

## Install

```shell
make clean install
```

The module name for the service file is `pam_unix_others.so` unless otherwise set by `configure` or `make`. 

The shared library `pam_unix_others.so` will be copied to `/usr/lib/security` and the helper program will be copied to `/usr/local/bin` this can be adjusted by setting the `PREFIX_BIN` variable via make. 

You must be **root** to install the module. 

## Development

### State of development

~So far the module is buildable, installable and runable but works exactly like pam_unix's authenticate method.~
The module and the helper work as intended. The calling process has to be in the group `ca_pou` (unless otherwise configured) and the user which shall be authenticated has to  be in the group `allow_pou` (unless otherwise configured).

### A shared code base

If any decisions in the code are dependent on the build target, the two definitions `BUILDING_MODULE` and `BUILDING_HELPER` should be used to guard such code.

### Debugging

The first place to check for errors is `journalctl`.  Also you might want to add `audit` to the parameter for the module in the service file.

To turn on debugging output and other debugging options configure can enable such options `./configure --enable-debugging`. Turning on debugging in this way will define `DEBUGGING` which should be used to in or exclude functionality in the code base. A similar named variable is available in the `Makefile`  which is either `yes` or `no`. 

## Further reading

The Linux-PAM Module Writers' Guide: http://www.linux-pam.org/Linux-PAM-html/Linux-PAM_MWG.html

An example pam module and client project with a lot of useful links: https://github.com/beatgammit/simple-pam

The linux-pam repository: https://github.com/linux-pam/linux-pam

