#include "alloc.h"
#include "libs.h"
header_t *head = NULL, *tail = NULL;

void heap_memcpy(void* to,size_t size_of_getter,void* from,size_t size_of_copy){
        header_t* from_head = (header_t*)from - 1;
        header_t* to_head   = (header_t*)to - 1;

        if ( size_of_copy > from_head->block_info.size_of_block || 
                        size_of_getter > to_head->block_info.size_of_block) return;

        size_t min = size_of_copy > size_of_getter ? size_of_getter : size_of_copy;
        for ( size_t i = 0 ; i < min; ++i ){
                *(((char*)to) + i) = *(((char*)from)+i);
        }

        return;
}

void memory_set(void* data, size_t data_size, char setter){
        char* buf = (char*)data;
        for(size_t i = 0; i < data_size; ++i){
                *buf = setter; 
                ++buf;
        }
        return;
}

int alloc_rm(void* block){
    if (block == NULL){
         return 1;
    }
    header_t* head_ptr;
    head_ptr = ((header_t*)block) - 1;
    if (head_ptr == tail){
        header_t* new_tail = head,*trash_walker;
        size_t last_free = 0,data_total_size = 0;
        tail->block_info.is_free = 1;
        for(trash_walker = head;trash_walker != tail;
            trash_walker = trash_walker->block_info.next){
                if ( trash_walker->block_info.is_free == 0 ){
                        last_free = 0;
                        data_total_size = 0;
                        new_tail = trash_walker;
                } else{
                        ++last_free;
                        data_total_size+=trash_walker->block_info.size_of_block;
                }
        }
        sbrk(-(data_total_size+last_free*HEADER_SIZE));
        if( new_tail == head && head->block_info.is_free == 1 ){
                head = tail = NULL;
        }
        else {
                tail = new_tail;
                tail->block_info.next = NULL;

        }
        return 0; 
    }
    head_ptr->block_info.is_free = 1;
    return 0;
}
void *alloc(size_t size){
     //check if data is valid
     if ( !size ) return NULL;       
     //if hed is NULL go alloc header
     if (head == NULL ){
            //allocing && fullfilling head data 
            head                           = (header_t*)sbrk(HEADER_SIZE+size); 
            head->block_info.is_free       = 0;
            head->block_info.next          = NULL;
            head->block_info.size_of_block = size;
            //directing tail
            tail = head;
            //NULL the data and return header data
            memory_set((void*)(head+1),size,0);
            return (void*)(head+1); 
     }
     // try to find free existing block
     header_t *new_block = get_free_block(size);
     //if was found
     if ( new_block != NULL ){
            //reserve the block and return it's value
            new_block->block_info.is_free   = 0;
            memory_set((void*)(new_block+1),size,0);
            return (void*)(new_block+1); 
     }
     // if block was not found
     else{
             // get new block after head make it head
             tail->block_info.next          = (header_t*)sbrk(HEADER_SIZE+size);
             tail                           = tail->block_info.next;
             // fullfill the header
             tail->block_info.is_free       = 0;
             tail->block_info.size_of_block = size;
             tail->block_info.next          = NULL;
             //NULL the old data and return block data
             memory_set((void*)(tail+1),tail->block_info.size_of_block,0);
             return (void*)(tail+1);
     }
}
header_t* get_free_block(size_t size){
     header_t* pointer = head;
     //search for an empty block of data with size we require
     while ( pointer != tail ){
            if( pointer->block_info.is_free == 1 &&
                            pointer->block_info.size_of_block >= size ){
                    //if found return this block
                    return pointer;
            }
            //if not found go to next block
            pointer = pointer->block_info.next;
     }
     //if block not found return NULL
     return NULL;
}

void* realloc(void* data, size_t new_size){
        if(data == NULL){
                return NULL;
        }
        if(new_size == 0){
                alloc_rm(data);
                return NULL;
        }
        size_t data_size = ((header_t*)data-1)->block_info.size_of_block;
        if( data_size >= new_size ) return data;
        void* new_data = alloc(new_size);
        heap_memcpy(new_data,new_size,data,data_size);
        return new_data;
}

void memdump(){
    header_t* pointer = head;
    size_t counter = 0;
    if (head == NULL){
            printf("+---------------+\n");
            printf("| heap is empty |\n");
            printf("+---------------+\n");
    }
    while (pointer != NULL){
            printf("+---------------+\n");
            pointer == head ?
            printf("|  >>>head<<<   |\n") : 0;
            pointer == tail ? 
            printf("|  >>>tail<<<   |\n") : 0; 
            printf("| absolute addr | %ld \n",(long)pointer);
            printf("|addr from head | %ld \n",counter),++counter;
            printf("|data block size| %ld \n",pointer->block_info.size_of_block);
            printf("|   now using   | %s \n",pointer->block_info.is_free ? "False" : "True");
            printf("|     data      | ");
            char *accum = (char*)(pointer+1);
            for(size_t i = 0; i < pointer->block_info.size_of_block;++i){
                   printf("%x ", *accum);
                   ++accum;
            }
            printf("\n");
            printf("+---------------+\n");
            pointer != tail ?
            printf("V   next node   V\n") : 0;
            pointer = pointer->block_info.next;
    }
}
