#include <stdbool.h> /* bool */
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

static size_t file_size(const char *filename) {
  FILE *f = fopen(filename, "rb");
  fseek(f, 0, SEEK_END);
  long fsize = ftell(f);
  fclose(f);
  return fsize;
}

// all offset are related, for absolute offset, the prefix abs_ is used in the
// code.
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
#undef PACK

struct examcard_data {
  const char *data;
  size_t len;
};

static void print_header(struct header *h) {
  printf("0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n", h->ints_offset,
         h->num_ints, h->floats_offset, h->num_floats, h->strings_offset,
         h->other_data_len, h->v8, h->numparams);
  printf("%u %u %u %u %u %u %u %u\n", h->ints_offset, h->num_ints,
         h->floats_offset, h->num_floats, h->strings_offset, h->other_data_len,
         h->v8, h->numparams);
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
  const char *type_str = ParamTypes[param->type];
  assert(type_str);
  const char *name = param->name;
  printf("%-32s\tdisp:%u\ttype:%s\tsize:%u\toffset1:0x%04x\toffset2:0x%"
         "04x\n",
         name, param->for_disp, type_str, param->dim, param->eoffset,
         param->offset);
}

static void process_examcard_data2(const struct examcard_data *examcard_data) {
#if 0
  struct header header;
  get_header(&header, examcard_data);
  //print_header(&header);
  for (uint32_t p = 0; p < header.numparams; ++p) {
	 }
#else
  const struct header *header = get_header2(examcard_data);
  struct param param;
  for (uint32_t p = 0; p < header->numparams; ++p) {
    const void *param_ptr = get_param_ptr(examcard_data, p);
    memcpy(&param, param_ptr, sizeof param);
    print_param(&param);
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

int main(int argc, char *argv[]) {
  if (argc < 2)
    return 1;
  const char *filename = argv[1];
  size_t len = file_size(filename);

  int err = 1;
  //  void *data = malloc(len);
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
