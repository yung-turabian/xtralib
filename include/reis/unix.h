#ifndef __REIS_UNIX_H__
#define __REIS_UNIX_H__

#include <dirent.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <syslog.h>
#include <stdio.h>
#include <string.h>

#define BD_NO_CHDIR           01 // dont chdir("/")
#define BD_NO_CLOSE_FILES     02 // dont close all open files
#define BD_NO_REOPEN_STD_FDS  04 // dont reopen stdin/out/err

#define BD_NO_UMASK0          010 // dont do umask(0)
#define BD_MAX_CLOSE          8192 // max fds

// 0 on success; -1 on error
int Daemonize(const char *name, char* path, int flags);

#endif
