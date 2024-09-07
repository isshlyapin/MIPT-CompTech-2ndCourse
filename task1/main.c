#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "mydef.h"


#define MAX_LEN_CMD 2048

int input_cmd(char *cmd_ptr, size_t nbyte);
int run_cmd(const char *cmd_ptr);
char **parce_cmd(const char *cmd_ptr);

int main()
{
    while(1)
    {
        char str_cmd[MAX_LEN_CMD];
        memset(str_cmd, '\0', MAX_LEN_CMD);

        input_cmd(str_cmd, MAX_LEN_CMD);
        PRINT_LOG("Input: {%s}\n", str_cmd);
        
        run_cmd(str_cmd);
    }

    return 0;
}

int input_cmd(char *cmd_ptr, size_t nbyte)
{
    PRINT_LOG("Wait enter cmd...\n");

    fgets(cmd_ptr, nbyte, stdin);

    if (cmd_ptr[strlen(cmd_ptr) - 1] == '\n') { cmd_ptr[strlen(cmd_ptr) - 1] = '\0'; }
        
    return 0;
}

int run_cmd(const char *cmd_ptr)
{
    const pid_t pid = fork();
    if (pid < 0)
    {
        PRINT_ERROR("Unexpected behavior");
    }
    else if (pid > 0)
    {
        int status = 0;
        waitpid(pid, NULL, WEXITSTATUS(status));
        
        return status;
    }
    else
    {
        char **args = parce_cmd(cmd_ptr);
           
    }
    return 0;
}

char **parce_cmd(const char *cmd_ptr)
{
    const char sep[4] = "|";

    char *istr = strtok(cmd_ptr, sep);

    while(istr)
    {
        PRINT_LOG("arg: [%s]\n", istr);

        istr = strtok(NULL, sep);
    }

    return NULL;
}

