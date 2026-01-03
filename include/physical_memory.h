#ifndef PHYSICAL_MEMORY_H
#define PHYSICAL_MEMORY_H

#include <list>
#include <cstddef>

// Allocation strategy 
enum class AllocatorType {
    FIRST_FIT,
    BEST_FIT,
    WORST_FIT
};

// Memory block representation 
struct Block {
    std::size_t start;  //starting address
    std::size_t size;//size in bytes
    bool free;
    int id;  // block id, -1 if free

    Block(std::size_t s, std::size_t sz, bool f, int i)
        : start(s), size(sz), free(f), id(i) {}
};

//Physical memory simulator 
class PhysicalMemory {
public:
    PhysicalMemory();

    void init(std::size_t total_size);
    void dump() const;
    //allocation interface
    int malloc(std::size_t size);
    bool free_block(int id);
    //allocator control
    void set_allocator(AllocatorType type);
    //statistics
    void stats() const;

private:
    
    std::size_t used_memory_;
    std::size_t total_alloc_requests_;
    std::size_t successful_allocs_;
    std::size_t failed_allocs_;

    std::size_t total_size_;
    std::list<Block> blocks_;
    int next_id_;
    AllocatorType allocator_;

    // allocation strategies
    int malloc_first_fit(std::size_t size);
    int malloc_best_fit(std::size_t size);
    int malloc_worst_fit(std::size_t size);

    //shared allocation helper
    int allocate_from_block(std::list<Block>::iterator it, std::size_t size);
};

#endif