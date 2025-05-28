#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/mman.h>

#define CHUNK_SIZE 64
#define HEAP_SIZE 4096

typedef struct chunk {
    bool isuse;
    struct chunk *next;
} chunk_t;

typedef struct heap {
    void *heap_start;
    size_t total_size;
    chunk_t *free_list;
    int free_count;
} heap_t;

heap_t heap;

void *smalloc(size_t size) {
    // 필요한 청크 개수 계산
    int chunks_needed = (size + CHUNK_SIZE - 1) / CHUNK_SIZE;
    
    if (chunks_needed > heap.free_count) {
        return NULL;  // 충분한 자유 청크가 없음
    }
    
    return NULL;
}

void sfree(void* ptr) {

}