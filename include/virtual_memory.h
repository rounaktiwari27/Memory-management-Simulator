#ifndef VIRTUAL_MEMORY_H
#define VIRTUAL_MEMORY_H

#include <cstddef>
#include <queue>
#include <unordered_set>
#include <string>

/*
Virtual Memory Simulator (Paging + FIFO)
- Single process
- Paging-based virtual memory
- FIFO page replacement
- Disk is representational

Virtual Address Format:
| PAGE NUMBER | OFFSET |

Physical Address:
physical_address = page_number * page_size + offset
*/

class VirtualMemory {
public:
    VirtualMemory();

    //Initialize virtual memory system
    //page_size must be power of two
    bool init(std::size_t page_size, std::size_t num_pages);
    //Access a virtual address
    //Returns translated physical address
    std::size_t access(std::size_t virtual_address);
    //Print virtual memory statistics
    void stats() const;
    //Reset page table and stats
    void reset();

private:
    //Helpers
    bool is_power_of_two(std::size_t x) const;
    std::size_t extract_page_number(std::size_t vaddr) const;
    std::size_t extract_offset(std::size_t vaddr) const;

private:
    //Configuration
    std::size_t page_size_;
    std::size_t num_pages_;
    std::size_t offset_bits_;

    //Page table(presence only)
    std::unordered_set<std::size_t> resident_pages_;

    //FIFO replacement queue
    std::queue<std::size_t> fifo_queue_;

    //Statistics
    std::size_t page_hits_;
    std::size_t page_faults_;
    std::size_t page_evictions_;
};

#endif