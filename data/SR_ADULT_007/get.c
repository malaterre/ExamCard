#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char all[] =
    "NOT_USED1,SHORT,SHORTEST,USER_DEF,NOT_USED3,INPHASE,OUTPHASE";

bool get_enum(char *buffer, size_t buf_len, const char *input, int j) {
  if (j < 0) {
    *buffer = 0;
    return false;
  }
  const size_t len = strlen(input);
  const char *output = input;
  for (int i = 0; i < j; ++i) {
    assert(output != NULL);
    const char *tmp = strchr(output, ',');
    if (tmp == NULL) {
      *buffer = 0;
      return false;
    }
    output = tmp + 1;
  }
  const char *tmp = strchr(output, ',');
  const size_t to_copy =
      tmp == NULL ? len - (output - input) : (size_t)(tmp - output);
  // it is an error is buffer too small to contains trailing null byte:
  if (to_copy < buf_len) {
    strncpy(buffer, output, to_copy);
    buffer[to_copy] = 0;
  } else {
    *buffer = 0;
    return false;
  }
  return true;
}

int main(int argc, char *argv[]) {
  int i = atoi(argv[1]);
  char buffer[8 * 2];
  printf("%s\n", all);
  if (get_enum(buffer, sizeof buffer, all, i)) {
    printf("%s\n", buffer);
  } else {
    assert(*buffer == 0);
  }
  return 0;
}
