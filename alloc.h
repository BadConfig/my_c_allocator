#include <unistd.h>
#define ROUND_THE_DATA char round[16];
typedef union header{
        struct{
            size_t size_of_block;
            char is_free;
            union header *next;
        }block_info;
        ROUND_THE_DATA
} header_t;
#define HEADER_SIZE sizeof(header_t)
header_t*   get_free_block  (size_t size);
void        *alloc          (size_t size);
int         alloc_rm        (void* pointer);
void*       realloc         (void* data, size_t new_size);
void        memdump         ();
void        heap_memcpy     (void* to,size_t size_of_getter,void* from, size_t size_of_copy);
void        memory_set      (void* data, size_t data_size, char setter);
