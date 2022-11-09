#include <assert.h>
#include <stddef.h> // offsetof
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h> // malloc
#include <string.h>

struct header {
  uint32_t ints_offset; //  offset to values (right after header)
  uint32_t num_ints;
  uint32_t floats_offset;
  uint32_t num_floats;
  uint32_t strings_offset;
  uint32_t other_data_len;
  uint32_t v8; // always equals to 8 ?
  uint32_t numparams;
};

#define PACK __attribute__((__packed__))
struct PACK param {
  char name[32 + 0];
  uint8_t zero;
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

static void check_header(struct header *h, const size_t data_len) {
  assert(h->ints_offset == h->numparams * 50 + 32);
  assert(offsetof(struct header, floats_offset) == 8);
  assert(h->ints_offset + h->num_ints * 4 == h->floats_offset + 8);
  assert(offsetof(struct header, strings_offset) == 16);
  assert(h->floats_offset + h->num_floats * 4 == h->strings_offset + 8);
  assert(h->v8 == 0x8);

  assert(sizeof(struct param) == 50);
  size_t param_len = 50 * h->numparams;
  size_t int_len = 4 * h->num_ints;
  size_t float_len = 4 * h->num_floats;
  size_t computed_len = param_len + int_len + float_len + h->other_data_len;
  printf("computed_len: 0x%zx %zu\n", computed_len, computed_len);
  assert(computed_len == data_len);
  // can be odd number:
  // assert(computed_len % 4 == 0);
}

static void print_header(struct header *h) {
  printf("0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n", h->ints_offset,
         h->num_ints, h->floats_offset, h->num_floats, h->strings_offset,
         h->other_data_len, h->v8, h->numparams);
  printf("%u %u %u %u %u %u %u %u\n", h->ints_offset, h->num_ints,
         h->floats_offset, h->num_floats, h->strings_offset, h->other_data_len,
         h->v8, h->numparams);
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

static void check_param(const struct header *h, const struct param *p) {
  assert(h);
  assert(p->for_disp == 0x0 || p->for_disp == 0x1);
  assert(p->type == 0x0 || p->type == 0x1 || p->type == 0x2 || p->type == 0x4);
  switch (p->type) {
  case Float:
    assert(p->eoffset == 0x0);
    break;
  case Integer:
    assert(p->eoffset == 0x0);
    break;
  case String:
    assert(p->eoffset == 0x0);
    break;
  case Enum:
    assert(p->eoffset != 0x0);
    break;
  default:
    assert(0);
  }
  const char *name = p->name;
  assert(name[2] == '_' || name[3] == '_');
  assert(p->eoffset < 0xffff); // FIXME
  assert(p->offset < 0xffff);  // FIXME
}

static void print_param(const struct header *h, const struct param *p) {
  assert(h);
  const char *type_str = ParamTypes[p->type];
  assert(type_str);
  const char *name = p->name;
  printf("%-32s\tdisp:%u\ttype:%s\tsize:%u\toffset1:0x%04x\toffset2:0x%"
         "04x\n",
         name, p->for_disp, type_str, p->dim, p->eoffset, p->offset);
}

static void process_data(const struct header *h, void *data, size_t data_len) {
  const char *cur = data;
  (void)data_len;
  for (uint32_t p = 0; p < h->numparams; ++p) {
    struct param param;
    assert(sizeof param == 50);
    memcpy(&param, cur, sizeof param);
    check_param(h, &param);
    print_param(h, &param);
    cur += sizeof param;
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2)
    return 1;
  const char *filename = argv[1];
  size_t len = file_size(filename);
  printf("0x%zx %zu\n", len, len);
  size_t data_len = len - 32;
  printf("data_len: 0x%zx %zu\n", data_len, data_len);

  FILE *stream = fopen(filename, "rb");
  struct header header;
  assert(sizeof header == 32);
  size_t ret = fread(&header, sizeof header, 1, stream);
  check_header(&header, data_len);
  print_header(&header);

  void *data = malloc(data_len);
  int err = 1;
  if (data) {
    ret = fread(data, 1, data_len, stream);
    assert(ret == data_len);
    process_data(&header, data, data_len);
    err = 0;
  }
  free(data);

  fclose(stream);
  return err;
}