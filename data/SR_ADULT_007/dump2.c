#include <assert.h>
#include <stdbool.h>
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
  char name[32 + 1]; // there is data stored after the trailing \0
  uint8_t for_disp;
  uint32_t type;
  uint32_t dim;
  uint32_t eoffset; // enum offset, the hardcoded strings
  uint32_t
      offset; // actual value offset, for enum this is an index in the vector
};

static size_t file_size(const char *filename) {
  FILE *f = fopen(filename, "rb");
  fseek(f, 0, SEEK_END);
  long fsize = ftell(f);
  fclose(f);
  return fsize;
}

static size_t get_data_len(const struct header *h) {
  size_t param_len = 50 * h->numparams;
  size_t int_len = 4 * h->num_ints;
  size_t float_len = 4 * h->num_floats;
  size_t computed_len = param_len + int_len + float_len + h->other_data_len;
  return computed_len;
}

static void check_header(struct header *h, const size_t data_len) {
  assert(h->ints_offset == h->numparams * 50 + 32);
  assert(offsetof(struct header, floats_offset) == 8);
  assert(h->ints_offset + h->num_ints * 4 == h->floats_offset + 8);
  assert(offsetof(struct header, strings_offset) == 16);
  assert(h->floats_offset + h->num_floats * 4 == h->strings_offset + 8);
  assert(h->v8 == 0x8);

  assert(sizeof(struct param) == 50);
  size_t computed_len = get_data_len(h);
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
  assert(p->eoffset < 0xffff); // FIXME
  assert(p->offset < 0xffff);  // FIXME

  size_t data_len = get_data_len(h);
  const uint32_t offset = p->offset;
  assert(offset > 32 && offset < data_len);
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
  // ??
  const char *name = p->name;
  assert(name[2] == '_' || name[3] == '_');
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

static bool get_enum(char *buffer, size_t buf_len, const char *input, int j) {
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
  // it is an error when buffer too small to contains trailing null byte:
  if (to_copy < buf_len) {
    strncpy(buffer, output, to_copy);
    buffer[to_copy] = 0;
  } else {
    *buffer = 0;
    return false;
  }
  return true;
}

typedef char str80[80 + 1];

enum LocationType {
  LUnknown = 0,
  LParam = 1,
  LInt = 2,
  LFloat = 3,
  LOther = 4
};

static enum LocationType get_location_from_offset(const struct header *h,
                                                  const char *data,
                                                  const size_t data_len,
                                                  uint32_t offset) {
	const uint32_t int_beg = h->numparams * 50;
	const uint32_t int_end = h->numparams * 50 + h->num_ints * 4;
	const uint32_t float_beg = int_end;
	const uint32_t float_end = int_end + h->num_floats * 4;
	assert(0);
  if (offset < h->numparams * 50) {
    assert(offset < h->ints_offset);
    return LParam;
  } else if (offset < h->ints_offset + h->num_ints * 4) {
    assert(offset < h->floats_offset);
    return LInt;
  } else if (offset < h->floats_offset + h->num_floats * 4) {
    return LFloat;
  } else {
    return LOther;
  }
}

void check_offset(const struct header *h, const struct param *p,
                  const uint32_t i, const char *data, const size_t data_len) {
  uint32_t offset1 = p->eoffset + i * 50 + 42 + 0;
  uint32_t offset2 = p->offset + i * 50 + 46 + 0;
  enum ParamType type = p->type;
  enum LocationType location;
  location = get_location_from_offset(h, data, data_len, offset2);
  switch (type) {
  case Float:
    assert(location == LFloat);
    break;
  }
}

static void print_values(const struct header *h, const struct param *p,
                         const uint32_t i, const char *data,
                         const size_t data_len) {
  assert(h);
  check_offset(h, p, i, data, data_len);
  (void)data_len;
  uint32_t pos0 = 0;
  uint32_t offset1 = p->eoffset + i * 50 + 42 + pos0;
  uint32_t offset2 = p->offset + i * 50 + 46 + pos0;
  uint32_t num = p->dim;
  switch (p->type) {
  case Float:
    printf("\t%x=>", offset2);
    for (uint32_t k = 0; k < num; ++k) {
      float f;
      memcpy(&f, data + offset2 + k * 4, sizeof f);
      if (k)
        printf(",");
      printf("%g", f);
    }
    printf("\n");
    break;
  case Integer:
    printf("\t%x=>", offset2);
    for (uint32_t k = 0; k < num; ++k) {
      int32_t f;
      memcpy(&f, data + offset2 + k * 4, sizeof f);
      if (k)
        printf(",");
      printf("%d", f);
    }
    printf("\n");
    break;
  case String:
    printf("\t%x=>", offset2);
    for (uint32_t k = 0; k < num; ++k) {
      str80 f;
      memcpy(&f, data + offset2 + k * sizeof f, sizeof f);
      if (k)
        printf(",");
      printf("%s", f);
    }
    printf("\n");
    break;
  case Enum:
    printf("\t=>");
    for (uint32_t k = 0; k < num; ++k) {
      int32_t f;
      memcpy(&f, data + offset2 + k * 4, sizeof f);
      if (k)
        printf(",");
      char buffer[8 * 2];
      const char *enum_str = data + offset1;
      if (get_enum(buffer, sizeof buffer, enum_str, f)) {
        printf("%s", buffer);
      } else {
        printf("%d", f);
        assert(0);
      }
    }
    printf("\n");
    break;
  default:
    assert(0);
  }
}

static void process_data(const struct header *h, const void *data,
                         size_t data_len) {
  const char *cur = data;
  (void)data_len;
  for (uint32_t p = 0; p < h->numparams; ++p) {
    struct param param;
    assert(sizeof param == 50);
    memcpy(&param, cur, sizeof param);
    check_param(h, &param);
    print_param(h, &param);
    print_values(h, &param, p, data, data_len);
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
