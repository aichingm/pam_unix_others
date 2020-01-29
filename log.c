
#include "log.h"
#include <syslog.h>
#include <stdarg.h>
#include <stdio.h>

void debug(const char* format, ...) {
  va_list args;
  va_start (args, format);
  vfprintf (stderr, format, args);
  va_end (args);
  fprintf (stderr, "\n");
}

void log_init(const char* prog_name) {
  openlog(prog_name, LOG_CONS | LOG_PID, LOG_AUTHPRIV);
}

void log_msg(int level, const char *format, ...) {
  va_list args;
  va_start(args, format);
  vsyslog(level, format, args);
  va_end(args);
}

void log_rt_debug(int condition, const char *format, ...) {
  if (condition) {
    va_list args;
    va_start(args, format);
    vsyslog(LOG_DEBUG, format, args);
    va_end(args);
  }
}

void log_close() {
  closelog();
}
