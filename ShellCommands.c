#include "LiamSmythShell.h"

void sigint_handler(int sig_num){
  signal(SIGINT, sigint_handler);
  printf("\n");
  printPrompt();
  fflush(stdout);
}

void printPrompt(void){
  // function to print out the exact local time and prompt (#) character
  int day, month, hours, minutes;
  time_t now;
  time(&now);
  struct tm *time = localtime(&now);
  day = time->tm_mday;
  month = time->tm_mon + 1;
  hours = time->tm_hour;
  minutes = time->tm_min;

  printf("[%02d/%02d %02d:%02d]# ", day, month, hours, minutes);
}
