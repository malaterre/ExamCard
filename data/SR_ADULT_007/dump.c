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

/*
9282 505 11294 237 12234 7782 8 185
9382 499 11370 244 12338 7955 8 187
9432 499 11420 245 12392 7955 8 188
9632 516 11688 265 12740 8176 8 192
9732 518 11796 265 12848 8176 8 194
10082 530 12194 263 13238 8328 8 201
10182 531 12298 647 14878 8328 8 203
10482 613 12926 373 14410 8029 8 209
11582 628 14086 378 15590 8376 8 231
11632 629 14140 378 15644 8407 8 232
11682 626 14178 380 15690 8734 8 233
*/
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

static void print_param(struct header *h, struct param *p, long beg, long end) {
  assert(p->for_disp == 0x0 || p->for_disp == 0x1);
  assert(p->type == 0x0 || p->type == 0x1 || p->type == 0x2 || p->type == 0x4);
  const char *type_str = ParamTypes[p->type];
  assert(type_str);
  uint32_t ints_offset = h->ints_offset;
  switch (p->type) {
  case Float:
    assert(p->eoffset == 0x0);
    // assert(p->offset >= ints_offset );
    break;
  case Integer:
    assert(p->eoffset == 0x0);
    // assert(p->offset <= ints_offset );
    break;
  case String:
    assert(p->eoffset == 0x0);
    // assert(p->offset >= ints_offset );
    break;
  case Enum:
    assert(p->eoffset != 0x0);
    // assert(p->offset <= ints_offset );
    break;
  default:
    assert(0);
  }
  const char *name = p->name;
  assert(name[2] == '_' || name[3] == '_');
  assert(p->eoffset < 0xffff); // FIXME
  assert(p->offset < 0xffff);  // FIXME
  printf("%-32s\tdisp:%u\ttype:%s\tsize:%u\toffset1:0x%04x\toffset2:0x%"
         "04x\t\tbeg:0x%04x\tend:0x%04x\n",
         name, p->for_disp, type_str, p->dim, p->eoffset, p->offset, beg, end);
}

typedef char str80[80 + 1];

static void print_values(struct header *h, struct param *p, FILE *stream,
                         int i) {
  long start = ftell(stream);
  int pos0 = 32;
  long offset1 = p->eoffset + i * 50 + 42 + pos0;
  long offset2 = p->offset + i * 50 + 46 + pos0;
  uint32_t num = p->dim;
  switch (p->type) {
  case Float:
    fseek(stream, offset2, SEEK_SET);
    printf("\t%x=>", offset2);
    for (uint32_t k = 0; k < num; ++k) {
      float f;
      fread(&f, sizeof f, 1, stream);
      if (k)
        printf(",");
      printf("%g", f);
    }
    printf("\n");
    break;
  case Integer:
    fseek(stream, offset2, SEEK_SET);
    printf("\t%x=>", offset2);
    for (uint32_t k = 0; k < num; ++k) {
      int32_t f;
      fread(&f, sizeof f, 1, stream);
      if (k)
        printf(",");
      printf("%d", f);
    }
    printf("\n");
    break;
  case String:
    fseek(stream, offset2, SEEK_SET);
    printf("\t%x=>", offset2);
    for (uint32_t k = 0; k < num; ++k) {
      str80 f;
      fread(&f, sizeof f, 1, stream);
      if (k)
        printf(",");
      printf("%s", f);
    }
    printf("\n");
    break;
  case Enum:
    fseek(stream, offset1, SEEK_SET);
    {
      char b[64];
      fread(b, sizeof *b, 64, stream);
      b[63] = 0;
      printf("\t%x\t%s", offset1, b);
    }
    printf("\n");
    fseek(stream, offset2, SEEK_SET);
    printf("\t=>");
    for (uint32_t k = 0; k < num; ++k) {
      int32_t f;
      fread(&f, sizeof f, 1, stream);
      if (k)
        printf(",");
      printf("%d", f);
    }
    printf("\n");
    break;
  default:
    assert(0);
  }
  fseek(stream, start, SEEK_SET);
}

size_t read_enums(char *out, size_t len, FILE *stream) {
  size_t count = 0;
  int i;
  while (count < len && (i = fgetc(stream)) != 0x0 && i != EOF) {
    *out++ = i;
    ++count;
  }
  // assert( i != EOF );
  *out = 0;
  return count;
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
    long beg = ftell(stream);
    ret = fread(&param, sizeof param, 1, stream);
    long end = ftell(stream);
    print_param(&header, &param, beg, end);
    print_values(&header, &param, stream, p);
  }
  printf("\n0x%04x\n", ftell(stream));
  // print integers
  for (int i = 0; i < header.num_ints; ++i) {
    int32_t f;
    fread(&f, sizeof f, 1, stream);
  }
  printf("\n0x%04x\n", ftell(stream));
  // print floats
  for (int i = 0; i < header.num_floats; ++i) {
    float f;
    fread(&f, sizeof f, 1, stream);
  }
  printf("\n0x%04x\n", ftell(stream));
  fseek(stream, 3240, SEEK_CUR);
  printf("\n0x%04x\n", ftell(stream));
  // print strings
  for (int i = 0; i < 13; ++i) {
    str80 f;
    fread(&f, sizeof f, 1, stream);
  }
  long start_enum = ftell(stream);
  printf("\n0x%04x\n", start_enum /*ftell(stream)*/);
  // print enums
  size_t count;
  int e = 0;
  do {
    char buffer[512 * 4];
    count = read_enums(buffer, sizeof buffer, stream);
    assert(strlen(buffer) == count);
    if (count)
      printf("E%d: %s\n", e++, buffer);
  } while (count != 0);
  long end_enum = ftell(stream);

  fseek(stream, start_enum, SEEK_SET);

  fclose(stream);
  return 0;
}
