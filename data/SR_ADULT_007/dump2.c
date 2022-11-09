#include <assert.h>
#include <stddef.h> // offsetof
#include <stdint.h>
#include <stdio.h>
#include <string.h>

struct header {
  uint32_t ints_offset; //  offset to values (right after header)
  uint32_t num_ints;
  uint32_t floats_offset;
  uint32_t num_floats;
  uint32_t strings_offset;
  uint32_t other_data_len;
  uint32_t v8;
  uint32_t numparams;
};

#define PACK __attribute__((__packed__))
struct PACK param {
  char name[32 + 1];
  uint8_t for_disp;
  uint32_t type;
  uint32_t dim;
  uint32_t eoffset; // enum offset
  uint32_t offset;  // actual value offset
};

static size_t file_size(const char *filename) {
  FILE *f = fopen(filename, "rb");
  fseek(f, 0, SEEK_END);
  long fsize = ftell(f);
  fclose(f);
  return fsize;
}

static void print_header(struct header *h) {
  assert(h->ints_offset == h->numparams * 50 + 32);
  assert(offsetof(struct header, floats_offset) == 8);
  assert(h->ints_offset + h->num_ints * 4 == h->floats_offset + 8);
  assert(offsetof(struct header, strings_offset) == 16);
  assert(h->floats_offset + h->num_floats * 4 == h->strings_offset + 8);
  assert(h->v8 == 0x8);
  printf("0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n", h->ints_offset,
         h->num_ints, h->floats_offset, h->num_floats, h->strings_offset,
         h->other_data_len, h->v8, h->numparams);
  printf("%u %u %u %u %u %u %u %u\n", h->ints_offset, h->num_ints,
         h->floats_offset, h->num_floats, h->strings_offset, h->other_data_len,
         h->v8, h->numparams);
}

int main(int argc, char *argv[]) {
  if (argc < 2)
    return 1;
  const char *filename = argv[1];
  size_t len = file_size(filename);
  printf("0x%x %u\n", len, len);
  size_t data_len = len - 32;
  printf("data_len: 0x%x %u\n", data_len, data_len);

  FILE *stream = fopen(filename, "rb");
  struct header header;
  assert(sizeof header == 32);
  size_t ret = fread(&header, sizeof header, 1, stream);
  print_header(&header);

  assert(sizeof(struct param) == 50);
  size_t param_len = 50 * header.numparams;
  size_t int_len = 4 * header.num_ints;
  size_t float_len = 4 * header.num_floats;
  size_t computed_len = param_len + int_len + float_len + header.other_data_len;
  printf("computed_len: 0x%x %u\n", computed_len, computed_len);
  assert(computed_len == data_len);

  fclose(stream);
  return 0;
}
