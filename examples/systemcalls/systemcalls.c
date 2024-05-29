
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "systemcalls.h"

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{
    int status;
    bool result;

    status = system(cmd);

    if (status == -1)
    {
        result = false;
    }
    else
    {
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
        {
            result = true;
        }
    }

    return result;
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count, ...)
{
    va_list args;
    char * command[count+1];
    int i;
    int childPid;
    int status;
    bool result = true;

    va_start(args, count);

    for (i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }

    command[count] = NULL;

    fflush(stdout);
    childPid = fork();

    if (childPid == -1)
    {
        result = false;
    }
    else if (childPid == 0)
    {
        int res = execv(command[0], &command[0]);
        exit(res);
    }
    else
    {
        do
        {
            if (wait(&status) < 0)
            {
                result = false;
                break;
            }
        } while(!WIFEXITED(status));

        result = (WEXITSTATUS(status) == 0) ? true : false;
    }

    va_end(args);

    return result;
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    char * command[count+1];
    int i;
    int fd;
    bool result = false;
    int childPid;
    int status;

    va_start(args, count);

    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    va_end(args);

    fflush(stdout);
    fd = open(outputfile, O_RDWR | O_CREAT | O_SYNC, S_IRWXU | S_IRGRP | S_IROTH);

    if (fd >= 0)
    {
        childPid = fork();
        if (childPid == 0)
        {
            if (dup2(fd, 1) >= 0)
            {
                execv(command[0], &command[0]);
            }
        }
        else
        {
            if (childPid != -1)
            {
                if (wait(&status) > 0)
                {
                    result = (WEXITSTATUS(status) == 0) ? true : false;
                }
            }
        }
        close(fd);
    }

    return result;
}
