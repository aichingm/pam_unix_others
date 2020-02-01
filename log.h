
#ifndef LOG_H
#define LOG_H

#undef D
#ifdef DEBUGGING
#define D(X) fprintf(stderr, "%s:%d (%s): ", __FILE__, __LINE__, __FUNCTION__); debug X ;
#else
#define D(X) ;
#endif
void debug(const char* format, ...);

void log_init(const char* prog_name);
void log_msg(int err, const char *format, ...);
void log_rt_debug(int condition, const char *format, ...);
void log_close(void);

#endif
