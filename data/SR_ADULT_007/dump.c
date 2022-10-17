#include <stdint.h>
#include <stdio.h>

struct header {
  uint32_t v1;
  uint32_t nints;
  uint32_t v4;
  uint32_t nfloats;
  uint32_t v6;
  uint32_t nstrings;
  uint32_t v8;
  uint32_t numparams;
};

void print_header(struct header *h) {
  printf("%u %u %u %u %u %u %u %u\n", h->v1, h->nints, h->v4, h->nfloats, h->v6,
         h->nstrings, h->v8, h->numparams);
}

int main(int argc, char *argv[]) {
  if (argc < 2)
    return 1;
  const char *filename = argv[1];

  FILE *stream = fopen(filename, "rb");
  struct header header;
  size_t ret = fread(&header, sizeof header, 1, stream);
  print_header(&header);
  fclose(stream);
  return 0;
}
