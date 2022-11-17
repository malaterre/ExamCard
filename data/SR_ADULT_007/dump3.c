#include <stdbool.h> /* bool */
#include <stddef.h>  /* offsetof */
#include <stdint.h>  /* uint32_t */
#include <stdio.h>   /* FILE */
#include <stdlib.h>  /* malloc */
#include <string.h>  /* memcpy */

#include <assert.h>

void process_examcard_data(const void *data, size_t len);

static size_t file_size(const char *filename) {
  FILE *f = fopen(filename, "rb");
  fseek(f, 0, SEEK_END);
  long fsize = ftell(f);
  fclose(f);
  return fsize;
}

int main(int argc, char *argv[]) {
  if (argc < 2)
    return 1;
  const char *filename = argv[1];
  size_t len = file_size(filename);

  int err = 1;
  void *data = aligned_alloc(4, len);
  if (data) {
    FILE *stream = fopen(filename, "rb");
    const size_t ret = fread(data, 1, len, stream);
    assert(ret == len);
    fclose(stream);
    process_examcard_data(data, len);
    err = 0;
  }
  free(data);
  return err;
}
