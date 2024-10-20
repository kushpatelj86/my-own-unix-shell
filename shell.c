#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/fcntl.h>

#define MAXNUM 100 
#define MAXLETTERS 1000 



void initialmessage() 
{
    printf("\033[H\033[J");
    printf("\n\n\n\n******************"
           "************************");
    printf("\n\n\n\t****Kush's SHELL****");
    printf("\n\n\n\n*******************"
           "***********************");
    char* username = getenv("USER");
    printf("\n\n\nUSER is: @%s", username);
    printf("\n");
    sleep(1);
    printf("\033[H\033[J");
}





int printSpace(char *str) 
{
    int i = 0;
    while (str[i] != '\0') 
    {
        if (str[i] == ' ') 
        {
            printf("SPACE ");
        }
        i++;
    }
    printf("\n");
    return 0;
}

int printPipe(char *str) 
{
    int i = 0;
    int count = 0;
    while (str[i] != '\0') 
    {
        if (str[i] == '|') 
        {
            printf("PIPE ");
            count++;
            
        }
        i++;
    }



    printf("\n");


    if(count > 0)
    {
        return 1;
    }
    return 0;
}





int takeInput(char* str)
{
    char* buf;

    buf = readline("\n>>> ");
    if (strlen(buf) > 0) 
    {
        add_history(buf);
        strcpy(str, buf);

        printPipe(str);
        printSpace(str);

        return 0;
    } 
    else 
    {
        return 1;
    }
}





int is_built_in(char *str[]) 
{
    int numcommands = 5;
    char* list[numcommands];
    char* username;
    int newAr = 0;
    list[0] = "exit";
    list[1] = "cd";
    list[2] = "help";
    list[3] = "hello";
    list[4] = "!!";

    static int prevAr = 0;

    int i = 0;
    while (i < numcommands) 
    {
        if (strcmp(str[0], list[i]) == 0) 
        {
            newAr = i + 1;
            break;
        }
        i++;
    }

    if (newAr == 1) 
    {
        prevAr = newAr;
        printf("\nYou are now leaving\n");
        exit(0);
        
    } 
    else if (newAr == 2) 
    {
        prevAr = newAr;

        if (str[1] != NULL) 
        {
            int change = chdir(str[1]);
            
            if (change != 0) 
            {
                fprintf(stderr, "chdir failed");
            }
        } 
        else 
        {
            printf("cd: missing argument\n");
        }
        return 1;
    } 
    else if (newAr == 3) 
    {
        prevAr = newAr;
        printf("Built-in commands:\n");
        printf("help  - Display this help message\n");
        printf("cd    - Change the current directory\n");
        printf("mkdir - Create a new directory\n");
        printf("exit  - Exit the shell\n");
        printf("!!    - Repeat the last command\n");
        return 1;
    } 
    else if (newAr == 4) 
    {
        prevAr = newAr;

        username = getenv("USER");
        if (username == NULL) 
        {
            username = "User";
        }
        printf("\nHello %s.\nUse help to know more..\n", username);
        return 1;
    } 



     else if (newAr == 5) 
    {
        printf("\nPrev arg %d\n", prevAr);
        if (prevAr == 1) 
        {
            printf("\nYou are now leaving\n");
            exit(0);
        
        } 
        else if (prevAr == 2) 
        {
            prevAr = newAr;

            if (str[1] != NULL) 
            {
                int change = chdir(str[1]);
                
                if (change != 0) 
                {
                    fprintf(stderr, "chdir failed");
                }
            } 
            else 
            {
                printf("cd: missing argument\n");
            }
            return 1;
        } 
        else if (prevAr == 3) 
        {
            prevAr = newAr;
            printf("Built-in commands:\n");
            printf("help  - Display this help message\n");
            printf("cd    - Change the current directory\n");
            printf("mkdir - Create a new directory\n");
            printf("exit  - Exit the shell\n");
            printf("!!    - Repeat the last command\n");
            return 1;
        } 
        else if (prevAr == 4) 
        {

            username = getenv("USER");
            if (username == NULL) 
            {
                username = "User";
            }
            printf("\nHello %s.\nUse help to know more..\n", username);
            return 1;
        } 











        return 1;
    } 







    else 
    {
        return 0;
    }
}

void printDirectory()
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\nDir: %s", cwd);
}



void execArgsWithoutCommandLineRedirection(char **command) 
{
    pid_t fork1 = fork();
    if (fork1 < 0) 
    {
        fprintf(stderr, "fork failed");
    } 
    else if (fork1 == 0) 
    {
        int exe = execvp(command[0], command);
        if (exe < 0) 
        {
            printf("\n execArgsWithoutCommandLineRedirection called\n");
            fprintf(stderr, "\nexecvp failed..");
        }
        exit(0);
    } 
    else 
    {
        wait(NULL);
    }
}


void execArgsWithCommandLineRedirection(char **command, const char *output_file) {
    pid_t fork1 = fork();
    if (fork1 < 0) 
    {
        fprintf(stderr, "Fork failed\n");
        exit(1);
    } 
    else if (fork1 == 0) 
    {
        int output = open(output_file, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
        if (output < 0) 
        {
            fprintf(stderr, "Failed to open output file %s\n", output_file);
            exit(1);
        }

        if (dup2(output, STDOUT_FILENO) < 0) 
        {
            fprintf(stderr, "Failed to redirect output\n");
            close(output);
            exit(1);
        }

        close(output);

        if (execvp(command[0], command) < 0) 
        {
            fprintf(stderr, "execvp failed for %s\n", command[0]);
            exit(1);
        }
    }
    else 
    {
        wait(NULL);
    }
}

















int parsePipe(char* str, char** strpiped)
{
    int count = 0;

    while (count < 2) 
    {
        strpiped[count] = strsep(&str, "|");
        if (strpiped[count] == NULL)
        {
            break;
        }
        count++;
    }

    if (strpiped[1] == NULL)
    {
        return 0; 
    }   
    else 
    {
        return 1;
    }
}

void parseSpace(char* str, char** parsed)
{
    int i = 0;

    while (i < MAXNUM) 
    {
        parsed[i] = strsep(&str, " ");

        if (parsed[i] == NULL) 
        {            
            break;
        }        
        if (strlen(parsed[i]) == 0) 
        { 
            continue;
        }

        i++;
    }

}


int parseRedirection(char* str, char** strredirected)
{
    for (int i = 0; i < 2; i++) 
    {
        strredirected[i] = strsep(&str, ">");
        if (strredirected[i] == NULL)
        {
            break;

        }
    }

    if (strredirected[1] == NULL)
    {
        return 0;
    }
    else 
    {
        strredirected[1] = strtok(strredirected[1], " ");
        return 1;
    }
}



void handleEcho(char* str) 
{
    char* token = strtok(str, " ");
    while (token != NULL) 
    {
        printf("%s\n", token);
        token = strtok(NULL, " ");
    }
}


int processString(char* str, char** parsed, char** parsedpipe)
{

    char* strpiped[2];
    char* strredirected[2];
    int piped = 0;
    int redirected = 0;


     char* lastWord = strrchr(str, ' ');
    if (lastWord != NULL && strcmp(lastWord + 1, "ECHO") == 0) 
    {
        handleEcho(str);
        return 0; 
    }




    piped = parsePipe(str, strpiped);

   
    if (piped) 
    {
        parseSpace(strpiped[0], parsed);
        parseSpace(strpiped[1], parsedpipe);
    } 
    else 
    {
        redirected = parseRedirection(str, strredirected);

        if (redirected) 
        {
            parseSpace(strredirected[0], parsed);
        } 
        else 
        {
            parseSpace(str, parsed);
        }
    }

     if (redirected) 
     {
        execArgsWithCommandLineRedirection(parsed, strredirected[1]);
        printSpace(str);
        printPipe(str);

        return 0;
    }

    int inbult = is_built_in(parsed);
    if (inbult)
    {

        return 0;
    }
    else
    {
        return 1 + piped;
    }
}

void execTwoPipes(char **cmd1, char **cmd2)
{
    int pipes[2]; 
    pid_t fork1, fork2;

    if (pipe(pipes) < 0) 
    {
        printf("\nPipe could not be initialized");
        return;
    }
    fork1 = fork();
    if (fork1 < 0) 
    {
        printf("\nCould not fork");
        return;
    }

    else if (fork1 == 0) {
        close(pipes[0]);
        fflush(stdout);
        dup2(pipes[1], STDOUT_FILENO);
        close(pipes[1]);

            int exe = execvp(cmd1[0], cmd1);

        if (exe < 0) 
        {
            printf("\nCould not execute command 1..");
            exit(0);
        }
    } 
    else 
    {
        fork2 = fork();

        if (fork2 < 0) 
        {
            printf("\nCould not fork");
        }

        
        if (fork2 == 0) 
        {
            close(pipes[1]);
            fflush(stdin);
            dup2(pipes[0], STDIN_FILENO);
            close(pipes[0]);
            int exe = execvp(cmd2[0], cmd2);
            if (exe < 0) 
            {
                printf("\nCould not execute command 2..");
                exit(0);
            }
        } 
        
        wait(NULL);
        wait(NULL);
        
    }
}












int main()
{
    char inputString[MAXLETTERS];
    char *cm1[MAXNUM];
    char* cm2[MAXNUM];
    int execFlag = 0;
    initialmessage();

    while (1) 
    {
        printDirectory();
        if (takeInput(inputString))
        {            
            continue;
        }       
        execFlag = processString(inputString, cm1, cm2);
        

        if (execFlag == 1)
        {
            execArgsWithoutCommandLineRedirection(cm1);
            printSpace(inputString);
            printPipe(inputString);

        }

        if (execFlag == 2)
        {
            execTwoPipes(cm1, cm2);
            printSpace(inputString);
            printPipe(inputString);

        }

           printSpace(inputString);
            printPipe(inputString);
    }
    return 0;
}




