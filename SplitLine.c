#include "LiamSmythShell.h"

char **splitLine(char *line){
  int buffer = BUFF_SIZE;
  int i = 0;
  char **args = malloc(buffer * sizeof(char*)); // allocate memory for the string array of arguments
  char *arg;

  if(!args){
    // exit the program if memory could not be allocated
    printf("memory allocation error\n");
    exit(EXIT_FAILURE);
  }

  arg = strtok(line, " \n"); // split strings on the basis of spaces and newline characters
  while(arg != NULL){
    args[i++] = arg;

    if(i >= buffer){
      // buffer size is reached, reallocate memory for string array
      buffer += BUFF_SIZE;
      args = realloc(args, buffer * sizeof(char*));
      if(!args){
        printf("memory allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    arg = strtok(NULL, " \n");
  }

  args[i] = NULL; // null terminate the string array

  return args;
}
