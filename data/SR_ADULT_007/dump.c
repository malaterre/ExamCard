#include <assert.h>
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

#define PACK __attribute__((__packed__))
struct PACK param {
  char name[32 + 1];
  uint8_t for_disp;
  uint32_t type;
  uint32_t dim;
  uint32_t v4;
  uint32_t offset;
};

static void print_header(struct header *h) {
  printf("%u %u %u %u %u %u %u %u\n", h->v1, h->nints, h->v4, h->nfloats, h->v6,
         h->nstrings, h->v8, h->numparams);
}

enum ParamType {
  Float = 0x0,
  Integer = 0x1,
  String = 0x2,
  Enum = 0x4,
};

static const char *ParamTypes[] = {
    "float", "integer", "string",
    NULL, // 3
    "enum",
};

static void print_param(struct param *p) {
  assert(p->for_disp == 0x0 || p->for_disp == 0x1);
  assert(p->type == 0x0 || p->type == 0x1 || p->type == 0x2 || p->type == 0x4);
  const char *type_str = ParamTypes[p->type];
  printf("%-32s\tdisp:%u\ttype:%s\tsize:%u\toffset1:0x%04x\toffset2:0x%04x\n",
         p->name, p->for_disp, type_str, p->dim, p->v4, p->offset);
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
  for (int p = 0; p < header.numparams; ++p) {
    struct param param;
    assert(sizeof param == 50);
    ret = fread(&param, sizeof param, 1, stream);
    print_param(&param);
  }
  fclose(stream);
  return 0;
}
