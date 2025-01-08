#include <reis/unix.h>

static int CloseNonStdFds(void)
{
    unsigned int i;

    struct rlimit rlim;
    int num_of_fds = getrlimit(RLIMIT_NOFILE, &rlim);

    if(num_of_fds == -1)
        return 1;

    for(i = 3; i < num_of_fds; i++)
        close(i);

    return 0;
}

static int ResetSignalHandlers(void)
{
#if defined _NSIG

    for(unsigned int i = 1; i < _NSIG; i++) {
        if(i != SIGKILL && i != SIGSTOP)
            signal(i, SIG_DFL);
    }
#endif

    return 0;
}

static int ClearSignalMask(void)
{
    sigset_t sset;

    return ((sigemptyset(&sset) == 0)
            && (sigprocmask(SIG_SETMASK, &sset, NULL) == 0));
}

static int CreatePIDFile(const char *pid_file)
{
    pid_t my_pid = getpid();
    char my_pid_str[10];
    int fd;

    sprintf(my_pid_str, "%d", my_pid);

    if((fd = open(pid_file, O_CREAT | O_EXCL | O_WRONLY, S_IRUSR | S_IWUSR)) == -1)
        return -1;

    if(write(fd, my_pid_str, strlen(my_pid_str)) == -1)
        return -1;

    close(fd);

    return 0;
}

int Daemonize(const char *name, int flags)
{
    int rc;
    int maxfd, fd;
    pid_t pid;

    CloseNonStdFds();

    ResetSignalHandlers();

    ClearSignalMask();


    // santize enviroment block

    pid = fork();

    if(pid == -1) //error
        exit(EXIT_FAILURE);
    else if(pid != 0) //parent terminate
        exit(EXIT_SUCCESS);

    if(setsid() == -1) // become leader of new session
        exit(EXIT_FAILURE);

    pid = fork();

    if(pid < 0) //error
        exit(EXIT_FAILURE);

    if(pid > 0) //parent terminate
        exit(EXIT_SUCCESS);

    // sets daemon name, as opposed to exe name
    rc = prctl(PR_SET_NAME, name, 0, 0, 0);
    if(rc != 0) {
        perror("prctl()");
        exit(1);
    }


    if(!(flags & BD_NO_UMASK0))
        umask(0);

    if(!(flags & BD_NO_CHDIR))
        chdir("/");

    char pid_name[BUFSIZ];

    snprintf(pid_name, strlen(name) + 10, "/run/%s.pid", name);

    CreatePIDFile(pid_name);


    if(!(flags & BD_NO_CLOSE_FILES)) {
        maxfd = sysconf(_SC_OPEN_MAX);
        if(maxfd == -1)
            maxfd = BD_MAX_CLOSE;
        for(fd = 0; fd < maxfd; fd++)
            close(fd);
    }

    if(!(flags & BD_NO_REOPEN_STD_FDS)) {
        //going dark
        close(STDIN_FILENO);

        fd = open("/dev/null", O_RDWR);
        if(fd != STDIN_FILENO)
            return -1;
        if(dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO)
            return -2;
        if(dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO)
            return -3;
    }

    return 0;
}
