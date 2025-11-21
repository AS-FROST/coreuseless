#include <stdio.h>
#include <stdlib.h>
const char* usage = "whoami\nsee who you are";

int main() {
  
  char* user = getenv("USERNAME");
  if(user == NULL) {
    puts("USERNAME: USERNAME env not set");
    return 1;
  }
  puts(user);
}
