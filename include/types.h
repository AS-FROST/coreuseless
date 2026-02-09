#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#include <sys/types.h>

#define TYPE_STRING 0
#define TYPE_INT    1
#define TYPE_BOOL   2

#define TRUE true
#define FALSE false

struct value {
  int     type     ;
  void*   value    ;
  char*   flag_name;
};

struct option {
  char         small      ;
  char*        name       ;
  char*        description;
  int          flag       ;
  struct value value      ;
  bool         takes_value;
};

struct parsed {
  int           flags      ;
  struct value* value      ;
  size_t        value_count;
};

#endif
