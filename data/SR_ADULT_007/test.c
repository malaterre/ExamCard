#include <stdio.h>
#include <stdlib.h>

static const char *ParamTypes[] = {
    "float",   // 0
    "integer", // 1
    "string",  // 2
    NULL,      // 3
    "enum",    // 4
};

static const char **foo() { return ParamTypes; }

int main(int argc, char *argv[]) {
  //  FILE *stream = fopen("test.raw", "wb");
  //  fwrite(ParamTypes, sizeof(ParamTypes), 1, stream);
  //  fclose(stream);
  const char **pt = foo();
  puts(pt[0]);
  puts(pt[1]);
  puts(pt[2]);
  //	puts(pt[3]);
  puts(pt[4]);
  printf("%d\n", pt[1] - pt[0]);
  printf("%d\n", pt[2] - pt[1]);
  printf("%d\n", pt[4] - pt[2]);
  return 0;
}
