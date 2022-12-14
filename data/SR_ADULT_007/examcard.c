#include <stdbool.h> /* bool */
#include <stddef.h>  /* offsetof */
#include <stdint.h>  /* uint32_t */
#include <stdio.h>   /* FILE */
#include <stdlib.h>  /* malloc */
#include <string.h>  /* memcpy */

#include <assert.h>

// implementation details:
#if 1
static inline bool is_aligned(const void *restrict pointer, size_t byte_count) {
  // https://stackoverflow.com/questions/1898153/how-to-determine-if-memory-is-aligned
  return (uintptr_t)pointer % byte_count == 0;
}
#else
#define is_aligned(POINTER, BYTE_COUNT)                                        \
  (((uintptr_t)(const void *)(POINTER)) % (BYTE_COUNT) == 0)
#endif

// all offset are related, for absolute offset, the prefix abs_ is used in the
// code.
struct header {
  uint32_t ints_offset; //  offset to values (right after header)
  uint32_t num_ints;
  uint32_t floats_offset;
  uint32_t num_floats;
  uint32_t other_data_offset;
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
#undef PACK

struct examcard_data {
  const char *data;
  size_t len;
};

static void print_header(const struct header *h) {
  printf("0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n", h->ints_offset,
         h->num_ints, h->floats_offset, h->num_floats, h->other_data_offset,
         h->other_data_len, h->v8, h->numparams);
  printf("%u %u %u %u %u %u %u %u\n", h->ints_offset, h->num_ints,
         h->floats_offset, h->num_floats, h->other_data_offset,
         h->other_data_len, h->v8, h->numparams);
}

// populate header
#if 0
static void get_header(struct header *header,
                       const struct examcard_data *examcard_data) {
  memcpy(header, examcard_data->data, sizeof *header);
}
#endif

static const struct header *
get_header2(const struct examcard_data *examcard_data) {
  assert(is_aligned(examcard_data->data, 4));
  return (const struct header *)examcard_data->data;
}

static const void *get_param_ptr(const struct examcard_data *examcard_data,
                                 uint32_t param_index) {
  const uint32_t header_size = 32;
  return examcard_data->data + param_index * 50 + header_size;
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

static void print_param(const struct param *param) {
  assert(param->type <= 4);
  const char *type_str = ParamTypes[param->type];
  assert(type_str);
  const char *name = param->name;
  // wotsit ?
  if (strcmp(name, "IF_info_seperator") != 0) {
    assert(param->for_disp == 0 || param->for_disp == 1);
  }
  printf("%-32s\tdisp:%u\ttype:%s\tsize:%u\toffset1:0x%04x\toffset2:0x%"
         "04x\n",
         name, param->for_disp, type_str, param->dim, param->eoffset,
         param->offset);
}

enum LocationType {
  Header = 0,
  Parameters = 1,
  Integers = 2,
  Floats = 3,
  OtherData = 4
};

static enum LocationType
get_location_from_abs_offset(const struct examcard_data *examcard_data,
                             uint32_t abs_offset) {
  const struct header *header = get_header2(examcard_data);
  const uint32_t header_size = 32;
  // ranges:
  const uint32_t header_beg = 0;
  const uint32_t header_end = header_size;
  assert(header_beg == 0);
  // param
  const uint32_t param_beg = header_size;
  const uint32_t param_end = param_beg + header->numparams * 50;
  assert(param_beg == header_end);
  // int
  assert(offsetof(struct header, ints_offset) == 0);
  const uint32_t int_beg = header->ints_offset + 0;
  const uint32_t int_end = int_beg + header->num_ints * 4;
  assert(int_beg == param_end);
  // float
  assert(offsetof(struct header, floats_offset) == 8);
  const uint32_t float_beg = header->floats_offset + 8;
  const uint32_t float_end = float_beg + header->num_floats * 4;
  assert(float_beg == int_end);
  // other data
  assert(offsetof(struct header, other_data_offset) == 16);
  const uint32_t other_data_beg = header->other_data_offset + 16;
  const uint32_t other_data_end = other_data_beg + header->other_data_len;
  assert(other_data_beg == float_end);
  assert(other_data_end == examcard_data->len);

  if (abs_offset >= header_beg && abs_offset < header_end) {
    return Header;
  } else if (abs_offset >= param_beg && abs_offset < param_end) {
    return Parameters;
  } else if (abs_offset >= int_beg && abs_offset < int_end) {
    return Integers;
  } else if (abs_offset >= float_beg && abs_offset < float_end) {
    return Floats;
  } else if (abs_offset >= other_data_beg && abs_offset < other_data_end) {
    return OtherData;
  } else {
    assert(0);
  }
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
    // printf("ERROR: %s %zd\n", output, to_copy);
    *buffer = 0;
    return false;
  }
  return true;
}

static void process_examcard_data2(const struct examcard_data *examcard_data) {
#if 0
  struct header header;
  get_header(&header, examcard_data);
  for (uint32_t p = 0; p < header.numparams; ++p) {
	 }
#else
  const struct header *header = get_header2(examcard_data);
  print_header(header);
  struct param param;
  assert(offsetof(struct param, eoffset) == 42);
  assert(offsetof(struct param, offset) == 46);
  for (uint32_t p = 0; p < header->numparams; ++p) {
    const char *param_ptr = get_param_ptr(examcard_data, p);
    memcpy(&param, param_ptr, sizeof param);
    print_param(&param);
    assert(param_ptr - examcard_data->data == p * 50 + 32);
    const uint32_t offset1 = param.eoffset + p * 50 + 42 + 32;
    const uint32_t offset2 = param.offset + p * 50 + 46 + 32;
    const enum ParamType type = param.type;
    const uint32_t num = param.dim;
    enum LocationType location;
    switch (type) {
    case Float:
      location = get_location_from_abs_offset(examcard_data, offset2);
      assert(location == Floats);
      break;
    case Integer:
      location = get_location_from_abs_offset(examcard_data, offset2);
      assert(location == Integers);
      break;
    case String:
      location = get_location_from_abs_offset(examcard_data, offset2);
      assert(location == OtherData);
      break;
    case Enum:
      location = get_location_from_abs_offset(examcard_data, offset1);
      assert(location == OtherData);
      {
        const char *enum_str = examcard_data->data + offset1;
        const size_t enum_len = strlen(enum_str);
        assert(enum_len < 4096);
        for (uint32_t k = 0; k < num; ++k) {
          uint32_t f;
          memcpy(&f, examcard_data->data + offset2 + k * 4, sizeof f);
          char buffer[17 + 1]; // PSEUDOGOLDENANGLE
          bool res = get_enum(buffer, sizeof buffer, enum_str, f);
          assert(res);
        }
      }
      break;
    }
  }
#endif
}

// exported:
void process_examcard_data(const void *data, size_t len) {
  struct examcard_data examcard_data;
  examcard_data.data = data;
  examcard_data.len = len;
  process_examcard_data2(&examcard_data);
}
