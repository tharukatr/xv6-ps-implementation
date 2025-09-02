#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// Student Name: Tharuka Rehan Rodrigo
// Student ID: 23201288
// This is my ps command program for xv6

int
main(int argc, char *argv[])
{
  // First I need to check how many arguments the user gave me
  if(argc == 1) {
    // User just typed "ps" with no extra stuff
    // So I'll show all processes that are running or sleeping
    ps();
  } else if(argc == 2) {
    // User gave me one argument, let me check what it is
    if(strcmp(argv[1], "-r") == 0) {
      // They want to see only running processes
      psr();
    } else {
      // They gave me something else that I don't understand
      printf("Wrong command option\n");
      exit(1);
    }
  } else {
    // They gave me too many arguments, that's not right
    printf("Usage: ps [-r]\n");
    exit(1);
  }
  
  // All done, exit normally
  exit(0);
}