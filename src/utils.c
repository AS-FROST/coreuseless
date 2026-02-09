#include "utils.h"
#include "types.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern const char* usage;

const char* version = "FROST Coreuseless version 1.0\nhttps://github.com/AYn0nyme/coreuseless - Report issues: https://github.com/AYn0nyme/coreuseless/issues";

struct parsed parse_args(int argc, char **argv, struct option options[], size_t options_count){
  struct parsed output = {0};
  output.value = calloc(0, sizeof(struct value));
  for(int c = 1; c < argc; c++) {
    if(argv[c][0] != '-') continue;
    if(argv[c][1] == '-') {
      if(!strcmp(argv[c], "--help")) {
        puts(usage);
        for(size_t i = 0; i < options_count; i++) {
          printf("-%c, --%s  %s\n", options[i].small, options[i].name, options[i].description);
        }
        exit(0);
      } else if (!strcmp(argv[c], "--version")) {
        puts(version);
        exit(0);
      }
      for(size_t i = 0; i < options_count; i++) {
        if(!strcmp(argv[c] + 2, options[i].name)) {
          output.flags |= options[i].flag;
          if(options[i].takes_value) {
            if(c+1>=argc)break;
            char* val = strdup(argv[c+1]);

            size_t size = output.value_count;
            output.value = realloc(output.value, size+1 * sizeof(struct value));
            output.value[size].flag_name = options[i].name;
            output.value_count++;

            if(options[i].value.type == TYPE_BOOL) {
              output.value[size].value = (void*)(intptr_t)(!strcmp(val, "true"));
              output.value[size].type = TYPE_BOOL;
            } else if (options[i].value.type == TYPE_STRING) {
              output.value[size].value = (void*)strdup(val);
              output.value[size].type = TYPE_STRING;
            } else if (options[i].value.type == TYPE_INT) {
              int i_val = strtol(strdup(val), NULL, 0);
              output.value[size].value = (void*)(intptr_t)i_val;
              output.value[size].type = TYPE_INT;
            }
            break;
          }
        }
      }
    } else {
      if(argv[c][1] == 'h') {
        puts(usage);
        for(size_t i = 0; i < options_count; i++) {
          printf("-%c, --%s  %s\n", options[i].small, options[i].name, options[i].description);
        }
        exit(0);
      }
      for(size_t j = 1; j < strlen(argv[c]); j++) {
        for(size_t i = 0; i < options_count; i++) {
          if(argv[c][j] == options[i].small) {
            output.flags |= options[i].flag;
            if(options[i].takes_value) {
              if(c+1 >= argc) continue;
              char* val = strdup(argv[c+1]);

              size_t size = output.value_count;
              output.value = realloc(output.value, size+1 * sizeof(struct value));
              output.value[size].flag_name = options[i].name;
              output.value_count++;

              if(options[i].value.type == TYPE_BOOL) {
                output.value[size].value = (void*)(intptr_t)(!strcmp(val, "true"));
                output.value[size].type = TYPE_BOOL;
              } else if (options[i].value.type == TYPE_STRING) {
                output.value[size].value = (void*)strdup(val);
                output.value[size].type = TYPE_STRING;
              } else if (options[i].value.type == TYPE_INT) {
                int i_val = strtol(strdup(val), NULL, 0);
                output.value[size].value = (void*)(intptr_t)i_val;
                output.value[size].type = TYPE_INT;
              }
            }
            break;
          }
        }
      }
      
    }
  }
  return output;
}
