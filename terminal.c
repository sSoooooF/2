#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <signal.h>

pid_t childPID; // pid of child process

// execute command
void execute_command(char* command, char *arg)
{
    printf(" Executing command: %s\n", command);
    printf(" Executing argument: %s\n", arg);
    pid_t pid = fork();
    char *args[3] = { command, arg, NULL };
    if (pid == -1)
    {
        fprintf(stderr, "Error creating child process\n");
        exit(1);
    }
    else if (pid == 0) // child process
    {
        printf(" Child process: %s\n", args[1]);
        execvp(args[0], args);
        fprintf(stderr, "Error executing command\n");
        exit(1);
    }
    else // parent process
    {
        childPID = pid; // pid of child process = pid of parent process
        int status; // status of child process
        waitpid(pid, &status, 0); // wait the final of child process
    }

}

void signal_processing(int signal)
{
    if (signal == SIGINT)
    {
        printf("SIGINT (Ctrl+C) signal received\n");
        if (childPID > 0)
        {
            kill(childPID, SIGKILL);
        }
    }
}

int main()
{
    char input[100];

    signal(SIGINT, signal_processing);

    while (1)
    {
        printf("~$ ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;

        if (strstr(input, "ls"))
        {
            char *arg = strtok(input, " ");
            arg = strtok(NULL, " ");

            execute_command("ls", arg);
        } 
        else if (strstr(input, "cat")) 
        {
            char *arg = strtok(input, " ");
            arg = strtok(NULL, " ");

            execute_command("cat", arg);
        } 
        else if (strstr(input, "nice")) 
        {
            // nice -n 19 gnome-software

            char *arg = strtok(input, " ");
            arg = strtok(NULL, "\n");
            execute_command("nice", arg);
        } 
        else if (strstr(input, "killall")) 
        {
            execute_command("/usr/bin/killall", "");
        } 
        else if (strstr(input, "browser")) 
        {
            execute_command("/snap/bin/brave", "");
        } 
        else if (strstr(input, "exit")|| strstr(input, "q")) 
        {
            printf("Program complete.\n");
            break;
        } 
        else 
        {
            printf("Unknown command.\n");
        }
    }

    return 0;
}