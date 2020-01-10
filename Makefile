#
# Copyright (c) 2020 Mario Aichinger <aichingm@gmail.com>
#


LIBCRYPT = -lcrypt
LIBAUDIT = -laudit
LIBSELINUX = #-lselinux

CC ?= gcc
CFLAGS = 
LDFLAGS = $(LIBCRYPT) $(LIBAUDIT) $(LIBSELINUX)
DEFS = "-DHAVE_LIBAUDIT=1"

ARTEFACTS = pam_unix_others.so unix_others_chkpwd

PAM_UNIX_OTHERS_os = bigcrypt.o pam_unix_auth.o  support.o passverify.o yppasswd_xdr.o md5_good.o md5_broken.o
CKPWD_os =  md5_good.hlpr_o md5_broken.hlpr_o bigcrypt.hlpr_o passverify.hlpr_o unix_chkpwd.hlpr_o


.PHONY: all clean nice defchklist deflist 

all: clean artefacts

clean:
	rm -rf *.o **/*.o *.hlpr_o **/*.hlpr_o $(ARTEFACTS)

nice:
	#clang-format -i -style=Chromium *.c **/*.c *.h **/*.h
	
artefacts: $(ARTEFACTS)

unix_others_chkpwd: $(CKPWD_os)
	$(CC) $(CFLAGS) $(LDFLAGS) $(DEFS) -o $@ $^

pam_unix_others.so: $(PAM_UNIX_OTHERS_os)
	$(CC) -shared $(CFLAGS) $(LDFLAGS) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -DCHKPWD_HELPER=\"unix_others_chkpwd\" -c -o $@ $<

%.hlpr_o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -DHELPER_COMPILE=\"unix_others_chkpwd\" -c -o $@ $<

	
GREP_CH_NOFILE = --no-filename --include="*.c" --include="*.h"
defchklist:
	@grep "#ifdef" .        -R $(GREP_CH_NOFILE) |  sort | uniq
	@grep "#if defined" .   -R $(GREP_CH_NOFILE) |  sort | uniq
	@grep "#elif defined" . -R $(GREP_CH_NOFILE) |  sort | uniq
	@grep "#elifdef" .      -R $(GREP_CH_NOFILE) |  sort | uniq
	@grep "#ifndef" .       -R $(GREP_CH_NOFILE) |  sort | uniq
	@grep "#if not def" .   -R $(GREP_CH_NOFILE) |  sort | uniq
	@grep "#elifndef" .     -R $(GREP_CH_NOFILE) |  sort | uniq
	@grep "#elif not def" . -R $(GREP_CH_NOFILE) |  sort | uniq
	
deflist:
	@grep "#define" .        -R $(GREP_CH_NOFILE) |  sort | uniq
