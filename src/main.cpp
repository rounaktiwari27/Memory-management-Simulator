#include <iostream>
#include <sstream>
#include <string>
#include "physical_memory.h"
#include "buddy_allocator.h"
#include "cache.h"
#include "virtual_memory.h"

enum class ActiveAllocator {
    PHYSICAL,
    BUDDY
};


int main() {
    PhysicalMemory phys;
    BuddyAllocator buddy;
    ActiveAllocator active = ActiveAllocator::PHYSICAL;
    Cache L1, L2;
    bool l1_ready = false;
    bool l2_ready = false;
    VirtualMemory vm;
    bool vm_ready = false; 

    std::string line;
    std::cout << "memsim> ";

    while (std::getline(std::cin, line)) {
        std::stringstream ss(line);
        std::string cmd;
        ss >> cmd;
        //----
        if (cmd == "exit") {
            break;
        }
        //----
        else if (cmd == "vm") {
            std::string sub;
            ss >> sub;

            if (sub == "init") {
                std::size_t page_size, num_pages;
                ss >> page_size >> num_pages;

                vm_ready = vm.init(page_size, num_pages);
                if (vm_ready)
                    std::cout << "Virtual memory initialized\n";
            }
            else if (sub == "stats") {
                if (vm_ready)
                    vm.stats();
                else
                    std::cout << "Virtual memory not initialized\n";
            }
            else {
                std::cout << "Unknown vm command\n";
            }
        }
        //----
        else if (cmd == "vaccess") {
            std::size_t vaddr;
            ss >> vaddr;

            if (!vm_ready) {
                std::cout << "Virtual memory not initialized\n";
                continue;
            }
            if (!l1_ready || !l2_ready) {
                std::cout << "Caches not initialized\n";
                continue;
            }

            //Virtual to Physical
            std::size_t paddr = vm.access(vaddr);
            //Cache hierarchy
            if (L1.access(paddr)) {
                std::cout << "PAGE HIT → L1 HIT\n";
            }
            else {
                std::cout << "PAGE ";
                if (L2.access(paddr)) {
                    std::cout << "HIT → L1 MISS → L2 HIT\n";
                }
                else {
                    std::cout << "HIT → L1 MISS → L2 MISS → MEMORY ACCESS\n";
                }
            }
        }
        //----
        else if (cmd == "stats") {
            if (active == ActiveAllocator::PHYSICAL)
                phys.stats();
            else
                buddy.stats();
        }
        //----
        else if (cmd == "init") {
            std::string word;
            std::size_t size;

            if (!(ss >> word >> size) || word != "memory") {
                std::cout << "Usage: init memory <size>\n";
            } else {
                phys.init(size);
                buddy.init(size);  
                active = ActiveAllocator::PHYSICAL;
                std::cout << "Initialized memory of size " << size << "\n";
            }
        }
        //----
        else if (cmd == "set") {
            std::string what, type;
            ss >> what >> type;
            if (what != "allocator") {
                std::cout << "Usage: set allocator <type>\n";
            }
            else if (type == "buddy") {
                active = ActiveAllocator::BUDDY;
                std::cout << "Switched to Buddy Allocator\n";
            }
            else {
                active = ActiveAllocator::PHYSICAL;
                if (type == "first_fit")
                    phys.set_allocator(AllocatorType::FIRST_FIT);
                else if (type == "best_fit")
                    phys.set_allocator(AllocatorType::BEST_FIT);
                else if (type == "worst_fit")
                    phys.set_allocator(AllocatorType::WORST_FIT);
                else {
                    std::cout << "Unknown allocator\n";
                    continue;
                }
                std::cout << "Switched to Physical Allocator (" << type << ")\n";
            }
        }
        //----
        else if (cmd == "malloc") {
            std::size_t size;
            ss >> size;
            int id = (active == ActiveAllocator::PHYSICAL) ? phys.malloc(size) : buddy.malloc(size);
            if (id == -1) {
                std::cout << "Allocation failed\n";
            } else {
                std::cout << "Allocated block id=" << id << "\n";
            }
        }
        //----
        else if (cmd == "free") {
            int id;
            ss >> id;
            bool ok = (active == ActiveAllocator::PHYSICAL) ? phys.free_block(id) : buddy.free_block(id);
            std::cout << (ok ? "Block freed\n" : "Invalid block id\n");
        }
        //----
        else if (cmd == "dump") {
            if (active == ActiveAllocator::PHYSICAL)
                phys.dump();
            else
                buddy.dump();
        }
        //----
        else if (cmd == "cache") {
            std::string sub;
            ss >> sub;

            if (sub == "init") {
                std::string level;
                std::size_t cache_size, block_size, ways;
                ss >> level >> cache_size >> block_size >> ways;

                if (level == "L1") {
                    l1_ready = L1.init("L1", cache_size, block_size, ways);
                    if (l1_ready) std::cout << "L1 cache initialized\n";
                }
                else if (level == "L2") {
                    l2_ready = L2.init("L2", cache_size, block_size, ways);
                    if (l2_ready) std::cout << "L2 cache initialized\n";
                }
                else {
                    std::cout << "Unknown cache level\n";
                }
            }
            else if (sub == "dump") {
                if (l1_ready) L1.dump();
                if (l2_ready) L2.dump();
            }
            else if (sub == "stats") {
                if (l1_ready) L1.stats();
                if (l2_ready) L2.stats();
            }
            else {
                std::cout << "Unknown cache command\n";
            }
        }
        //----
        else if (cmd == "access") {
            std::size_t address;
            ss >> address;

            if (!l1_ready || !l2_ready) {
                std::cout << "Caches not initialized\n";
                continue;
            }
            if (L1.access(address)) {
                std::cout << "L1 HIT\n";
            }
            else {
                std::cout << "L1 MISS → ";
                if (L2.access(address)) {
                    std::cout << "L2 HIT\n";
                }
                else {
                    std::cout << "L2 MISS → MEMORY ACCESS\n";
                }
            }
        }
        //----
        else {
            std::cout << "Unknown command\n";
        }
        std::cout << "memsim> ";
    }

    return 0;
}