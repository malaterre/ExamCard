#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static const char all[] =
    "NOT_USED1,SHORT,SHORTEST,USER_DEF,NOT_USED3,INPHASE,OUTPHASE";
bool get_enum(char *buffer, size_t buf_len, const char *input, int j) {
  const char *start = input;
  size_t len = strlen(input);
  const char *output = start;
  for (int i = 0; i < j; ++i) {
    assert(start != NULL);
    const char *tmp = strchr(start, ',');
    if (tmp == NULL) {
      *buffer = 0;
      return false;
    }
    output = tmp + 1;
    start = output;
  }
  char *beg = buffer;
  const char *cur = output;
  while (cur < start + len && *cur != ',') {
    *buffer++ = *cur++;
    assert((size_t)(buffer - beg) < buf_len);
  }
  *buffer++ = 0;
  return true;
}

int main(int argc, char *argv[]) {
  int i = atoi(argv[1]);
  char buffer[512];
  printf("%s\n", all);
  if (get_enum(buffer, sizeof buffer, all, i)) {
    printf("%s\n", buffer);
  } else {
    assert(*buffer == 0);
  }
  return 0;
}
