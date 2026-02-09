#include <stdio.h>

const char* usage = "yes [STRING...]\noutputs STRING repeatedly";

int main(int argc, char** argv){
  for(;;){
    if(argc < 2) putchar('y');
    for(int c = 1; c < argc; c++) {
      printf("%s", argv[c]);
      if(c < argc-1) putchar(' ');
    }
    putchar('\n');
  }
}
