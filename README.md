# Virtual Memory Ringbuffer with Hugepages Support

This project implements a high-performance ringbuffer utilizing virtual memory and supports the use of hugepages for efficient memory management. The ringbuffer is designed to work with large-scale systems where memory performance is critical, and it allows for fast, lock-free read/write operations where memory is abundant.

## Features

- **Ringbuffer Design**: The core structure is a circular buffer that efficiently handles data in a continuous stream.
- **Hugepages Support**: The buffer is designed to take advantage of hugepages, reducing overhead associated with page table management and increasing performance for large memory allocations.
- **Virtual Memory**: The buffer uses virtual memory for flexibility, enabling large buffers that may not fit entirely in physical memory.
- **Efficient Memory Usage**: By leveraging hugepages, the system minimizes memory fragmentation and optimizes cache utilization.

## Requirements

- Linux-based OS
- Kernel with support for hugepages (typically available by default)
- C compiler (GCC/Clang)
- Standard C library

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/Irreq/ringbuffer.git
   cd ringbuffer
   ```

## Hugepage Support

In order to use hugepages, you must have twice the number of hugepages available due to the ringbuffer using dual-pages. See the number of available hugepages on the system:

```bash
cat /proc/meminfo | grep HugePages
```

Set hugepages (example for 2 pages of 2MB)
```bash
echo 2 > /proc/sys/vm/nr_hugepages
```