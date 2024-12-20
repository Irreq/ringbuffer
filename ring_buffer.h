#ifndef RING_BUFFER_H
#define RING_BUFFER_H
#define _GNU_SOURCE
#include <linux/memfd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

/**
 * Add hugepage
 *
 * sudo sysctl -w vm.nr_hugepages=2
 *
 * List hugepage
 *
 * cat /proc/meminfo | grep Huge
 *
 */

typedef struct _ring_buffer {
  size_t index;    // Current position in array
  size_t capacity; // Half of used memory
  void *array;     // All the data
} ring_buffer;

/**
 * @brief Initialize virtual-memory ringbuffer
 *
 * WARNING: This will not fill the buffer initially, therefore it is recommended
 * to set/flush the buffer by the user before operating on real data.
 *
 * @param use_hugepages If the ringbuffer should contain hugepages (requires
 * existing pages)
 * @return pointer to ringbuffer
 */
ring_buffer *init_ring_buffer(int use_hugepages);
/**
 * @brief Release memory from ringbuffer to the system
 *
 * @param rb ringbuffer object
 */
void free_ring_buffer(ring_buffer *rb);
/**
 * @brief Move the current index of the ringbuffer
 *
 * @param rb ringbuffer object
 * @param num_bytes sizeof(type) * offset
 */
void roll_ring_buffer(ring_buffer *rb, const size_t num_bytes);
/**
 * @brief get the current (or offset) value in the ringbuffer
 *
 * @param rb ringbuffer object
 * @param num_bytes_offset offset in bytes (0 for current position)
 */
void *get_ring_buffer(ring_buffer *rb, const size_t num_bytes_offset);

/**
 * @brief Read data from the ringbuffer
 *
 * NOTICE: This method wraps memcpy
 *
 * @param rb ringbuffer object
 * @param out read data
 * @param num_bytes amount to read
 * @param num_bytes_offset offset from current position
 */
void read_ring_buffer(ring_buffer *rb, void *out, const size_t num_bytes,
                      const size_t num_bytes_offset);

/**
 * @brief Write data to the ringbuffer
 *
 * NOTICE: This method wraps memcpy + roll
 *
 * @param rb ringbfuffer oject
 * @param in data to write
 * @param num_bytes amount to write
 */
void write_ring_buffer(ring_buffer *rb, void *in, const size_t num_bytes);

/**
 * @brief Compute the number of possible items (length) of the array
 *
 * @param rb ringbuffer object
 * @param bytes sizeof items
 * @return number of possible items
 */
size_t length_ring_buffer(ring_buffer *rb, const size_t bytes);
#endif // !RING_BUFFER_H
