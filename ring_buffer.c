#include "ring_buffer.h"
#include <string.h>

ring_buffer *init_ring_buffer(int use_hugepages) {
  ring_buffer *rb = (ring_buffer *)malloc(sizeof(ring_buffer));

  if (!rb) {
    printf("Unable to allocate memory for ringbuffer\n");
    return NULL;
  }

  // Setup flags depending on hugepages or not
  int page_size, mfd_flags, mmap_flags;
  if (use_hugepages) {
    mfd_flags = MFD_HUGETLB | MFD_HUGE_2MB;
    mmap_flags = MAP_HUGETLB | MAP_HUGE_2MB;
    page_size = (1 << 21); // 2MB
  } else {
    mfd_flags = 0;
    mmap_flags = 0;
    page_size = getpagesize();
  }

  rb->index = 0;

  // Create a filedescriptor that will be used to assign virtual memory
  int fd = memfd_create("page", mfd_flags);

  if (fd == -1) {
    perror("memfd_create failed");
    free(rb);
    return NULL;
  }

  // Modify size to fit exactly a page
  if (ftruncate(fd, page_size) == -1) {
    perror("ftruncate failed");
    free(rb);
    close(fd);
    return NULL;
  }

  // Map twice the memory in a contigious way
  rb->array = mmap(NULL, 2 * page_size, PROT_READ | PROT_WRITE,
                   MAP_SHARED | mmap_flags, fd, 0);

  if (rb->array == MAP_FAILED) {
    perror("mmap failed for first region");
    close(fd);
    free(rb);
    return NULL;
  }

  // Map the same array to the second half as well
  if (mmap((char *)rb->array + page_size, page_size, PROT_READ | PROT_WRITE,
           MAP_SHARED | MAP_FIXED | mmap_flags, fd, 0) == MAP_FAILED) {

    perror("mmap failed for second region");
    munmap(rb->array, page_size);
    close(fd);
    free(rb);
    return NULL;
  }

  rb->capacity = page_size;

  close(fd);
  return rb;
}
void free_ring_buffer(ring_buffer *rb) {
  munmap(rb->array, 2 * rb->capacity);
  free(rb);
}

void roll_ring_buffer(ring_buffer *rb, const size_t num_bytes) {
  rb->index = (rb->index + num_bytes) % rb->capacity;
}

void *get_ring_buffer(ring_buffer *rb, const size_t num_bytes_offset) {
  return (char *)(rb->array) + rb->index + num_bytes_offset;
}

void read_ring_buffer(ring_buffer *rb, void *out, const size_t num_bytes,
                      const size_t num_bytes_offset) {
  memcpy(out, get_ring_buffer(rb, num_bytes_offset), num_bytes);
}

void write_ring_buffer(ring_buffer *rb, void *in, const size_t num_bytes) {
  memcpy(get_ring_buffer(rb, 0), in, num_bytes);
  roll_ring_buffer(rb, num_bytes);
}

size_t length_ring_buffer(ring_buffer *rb, const size_t bytes) {
  return rb->capacity / bytes;
}
