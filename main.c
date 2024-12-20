#include "ring_buffer.h"

typedef long int dtype;

int main() {
    int use_huge_page = 0;
  ring_buffer *rb = init_ring_buffer(use_huge_page);

  if (!rb) {
    printf("Unable to create ring_buffer!\n");
    exit(1);
  }

  dtype *data = (dtype *)get_ring_buffer(rb, 0);

  for (int i = 0; i < length_ring_buffer(rb, sizeof(dtype)) + 5; i++) {
    data[i] = (dtype)i;
  }

  for (int i = 0; i < length_ring_buffer(rb, sizeof(dtype)) + 3; i++) {
    printf("%ld ", data[i]);
  }

  printf("\n");

  return 0;
}