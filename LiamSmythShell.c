/* Liam Smyth's implementation of a bash shell
   program will read in commands from stdin and execute the commands
   program will end on EOF (Ctrl + D)
*/
#include "LiamSmythShell.h"

int main(void){
  signal(SIGINT, sigint_handler); // to handle Ctrl+C
  size_t n = 10;
  char *lineptr = NULL;
  size_t read;
  pid_t child_pid;
  int child_status;

  printPrompt(); // print time (day/month hours:minutes) and prompt character

  while((read = getline(&lineptr, &n, stdin)) != EOF){ // EOF == (Ctrl + D)
    char **arguments = splitLine(lineptr); // split the line into an array of arguments, the first being the command
    bool commandFound = false, fileOutPut = false;

    int i = 0;
    while(arguments[i] != NULL){
      // check all arguments to see if the user want to redirect stdout to a file
      if(strcmp(arguments[i], ">") == 0){
        fileOutPut = true;
        break;
      }
      i++;
    }

    /* check all required arguments to see if the user has entered a correct argument
       print an error message otherwise */
    if(strcmp(arguments[0], "ls") == 0){
      // list directory contents
      commandFound = true;
    }else if(strcmp(arguments[0], "pwd") == 0){
      // print working directory
      commandFound = true;
    }else if(strcmp(arguments[0], "sleep") == 0){
      // suspend execution for an interval of time (seconds)
      commandFound = true;
    }else if(strcmp(arguments[0], "mkdir") == 0){
      // make a new directory
      commandFound = true;
    }else if(strcmp(arguments[0], "cd") == 0){
      // change directory
      // execvp will not execute cd correctly so chdir must be used to implement the command
      int success;
      success = chdir(arguments[1]);
      if(success != 0){
        perror(strerror(errno));
      }

    }else if(strcmp(arguments[0], "rmdir") == 0){
      // remove directory
      commandFound = true;
    }else if(strcmp(arguments[0], "echo") == 0){
      // write arguments to the standard output
      commandFound = true;
    }else if(strcmp(arguments[0], "whoami") == 0){
      // display effective user ID
      commandFound = true;
    }else if(strcmp(arguments[0], "cat") == 0){
      // concatenate and print files
      commandFound = true;
    }else{
      // command is not valid
      printf("Command not found\n");
    }

    if(commandFound){
      int f;
      if(fileOutPut){
        f = open(arguments[i + 1], O_WRONLY|O_CREAT, 0666);
      }
      child_pid = fork();
      if(child_pid == 0){
        // child process will execute the command
        if(fileOutPut){
          dup2(f, 1); // change the standard output to a file
          arguments[i] = NULL;
        }
        execvp(arguments[0], arguments);
        printf("Unkown command\n");
        exit(0);
      }else{
          // parent process will wait for child process to finish before receiving more commands
          // also handle Ctrl+c
          signal(SIGINT, SIG_IGN); // tell the parent process to ignore SIGINT while child process is running
          wait(&child_status);
          if(fileOutPut){
            close(f);
          }
          signal(SIGINT, sigint_handler); // tell the parent process to handle SIGINT manually once child process is finished
      }
    }

    printPrompt(); // print out the time and prompt character
  }
  printf("\n");

  return 0;
}
