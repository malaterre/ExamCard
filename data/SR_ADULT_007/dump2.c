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
  uint32_t num_strings;
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

static void print_header(struct header *h) {
  assert(h->ints_offset == h->numparams * 50 + 32);
  assert(offsetof(struct header, floats_offset) == 8);
  assert(h->ints_offset + h->num_ints * 4 == h->floats_offset + 8);
  assert(offsetof(struct header, strings_offset) == 16);
  assert(h->floats_offset + h->num_floats * 4 == h->strings_offset + 8);
  assert(h->v8 == 0x8);
  printf("0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n", h->ints_offset,
         h->num_ints, h->floats_offset, h->num_floats, h->strings_offset,
         h->num_strings, h->v8, h->numparams);
  printf("%u %u %u %u %u %u %u %u\n", h->ints_offset, h->num_ints,
         h->floats_offset, h->num_floats, h->strings_offset, h->num_strings,
         h->v8, h->numparams);
}

int main(int argc, char *argv[]) {
  if (argc < 2)
    return 1;
  const char *filename = argv[1];

  FILE *stream = fopen(filename, "rb");
  struct header header;
  assert(sizeof header == 32);
  size_t ret = fread(&header, sizeof header, 1, stream);
  print_header(&header);

  fclose(stream);
  return 0;
}
