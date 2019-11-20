#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//creating a doubly linked list 
//previous pointer, next pointer and data to form a linked list.
struct mem_link_list{
    struct mem_link_list *prev;
    struct mem_link_list *next;
    int size;            // Size of the block
    char alloc;          // flag : 1 for allocated and 0 for hole.
    void *ptr;           // location of block in mem_pool.
};

//function
struct mem_link_list* mem_worst(size_t malloc_size);

void *mem = NULL;
static struct mem_link_list *head;
static struct mem_link_list *next;

void mem_initialize(size_t mem_pool){
    // Initialize memory
    mem = malloc(mem_pool);
    head = malloc(sizeof(struct mem_link_list));
    head->size = mem_pool;
    head->alloc = 0;
    head->ptr = mem;
    next = head;
    // circular linked list 
    head->prev = head;
    head->next = head;
}

// allocate a block of memory with the requested size
// it returns a pointer to the newly allocated block, if not available returns NULL

void *malloc_block(size_t malloc_size) {
    struct mem_link_list* allocate = NULL;
    allocate = mem_worst(malloc_size);
    //if mem_worst returns NULL
    if(!allocate) {
        return NULL;
    }
    if(allocate->size > malloc_size) {
    // unallocate remaining of this block and inserting the block in linked list
    struct mem_link_list* rem = malloc(sizeof(struct mem_link_list));
    rem->next = allocate->next;
    rem->next->prev = rem;
    rem->prev = allocate;
    allocate->next = rem;

    // Dividing allocated memory
    rem->size = allocate->size - malloc_size;
    rem->alloc = 0;
    rem->ptr = allocate->ptr + malloc_size;
    allocate->size = malloc_size;
    next = rem;
    } else {
    next = allocate->next;
    }

    allocate->alloc = 1;
    // Return pointer to the allocated block
    return allocate->ptr;
}

// find the largest unallocated block
struct mem_link_list* mem_worst(size_t malloc_size) {
    struct mem_link_list* loc = head, *max = NULL;
    do {
        if(!(loc->alloc) && (!max || loc->size > max->size) ) {
            max = loc;
        }
    } while((loc = loc->next) != head);

    if(max->size >= malloc_size) {
        return max;
    } 
    else {
        return NULL;
    }
}

// to free a block of memory by free cmd and creating a hole
void free_block(void* block) {
    // searching for the target block
    struct mem_link_list* target = head;
    do {
        if(target->ptr == block) {
            break;
        }
    } while((target = target->next) != head);

    //change the flag to free
    target->alloc = 0;

    // if the previous block is free making it contiguous
    if(target != head && !(target->prev->alloc)) {
        struct mem_link_list* before = target->prev;
        before->next = target->next;
        before->next->prev = before;
        before->size += target->size;
        if(next == target)
            next = before;
        free(target);
        target = before;
    }

    // if the next block is free making contiguous
    if(target->next != head && !(target->next->alloc)) {
        struct mem_link_list* after = target->next;
        target->next = after->next;
        target->next->prev = target;
        target->size += after->size;
        
        if(next == after)
            next = target;        
        free(after);
    }
}

void print_block() {
    //initialize to head and then iterate over all free and allocated blocks
    struct mem_link_list* loc = head;
    //pointer to integer
    size_t int_value = (size_t)head->ptr;
    printf("--------------------- print memory output ----------------------------\n");
    do {
        printf("%s\tstart_addr = %zu\t\tsize = %d\n", (loc->alloc ? "Allocated" : "Hole\t"), ((size_t)loc->ptr -int_value),loc->size);
    } while((loc = loc->next) != head);
    printf("----------------------------------------------------------------------\n");
}