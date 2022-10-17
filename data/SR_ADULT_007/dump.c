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
  uint32_t eoffset; // enum offset
  uint32_t offset;  // actual value offset
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
    "float",   // 0
    "integer", // 1
    "string",  // 2
    NULL,      // 3
    "enum",    // 4
};

static void print_param(struct param *p) {
  assert(p->for_disp == 0x0 || p->for_disp == 0x1);
  assert(p->type == 0x0 || p->type == 0x1 || p->type == 0x2 || p->type == 0x4);
  const char *type_str = ParamTypes[p->type];
  assert(type_str);
  switch (p->type) {
  case Enum:
    assert(p->eoffset != 0x0);
    break;
  default:
    assert(p->eoffset == 0x0);
  }
  const char *name = p->name;
  assert(name[2] == '_' || name[3] == '_');
  assert(p->eoffset < 0xffff); // FIXME
  assert(p->offset < 0xffff);  // FIXME
  printf("%-32s\tdisp:%u\ttype:%s\tsize:%u\toffset1:0x%04x\toffset2:0x%04x\n",
         name, p->for_disp, type_str, p->dim, p->eoffset, p->offset);
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
  printf("\n0x%04x\n", ftell(stream));

  fclose(stream);
  return 0;
}
