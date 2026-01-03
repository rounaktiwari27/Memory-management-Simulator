#include "virtual_memory.h"
#include <iostream>
#include <cmath>


VirtualMemory::VirtualMemory()
    : page_size_(0),
      num_pages_(0),
      offset_bits_(0),
      page_hits_(0),
      page_faults_(0),
      page_evictions_(0) {}


//Helpers
bool VirtualMemory::is_power_of_two(std::size_t x) const {
    return x > 0 && (x & (x - 1)) == 0;
}

std::size_t VirtualMemory::extract_page_number(std::size_t vaddr) const {
    return vaddr >> offset_bits_;
}

std::size_t VirtualMemory::extract_offset(std::size_t vaddr) const {
    return vaddr & ((std::size_t(1) << offset_bits_) - 1);
}


//Initialization
bool VirtualMemory::init(std::size_t page_size, std::size_t num_pages) {
    if (!is_power_of_two(page_size) || page_size == 0 || num_pages == 0) {
        std::cout << "Invalid virtual memory configuration\n";
        return false;
    }

    page_size_ = page_size;
    num_pages_ = num_pages;
    offset_bits_ = static_cast<std::size_t>(std::log2(page_size_));

    resident_pages_.clear();
    while (!fifo_queue_.empty()) fifo_queue_.pop();

    page_hits_ = 0;
    page_faults_ = 0;
    page_evictions_ = 0;

    return true;
}


//Access
std::size_t VirtualMemory::access(std::size_t virtual_address) {
    std::size_t page_number = extract_page_number(virtual_address);
    std::size_t offset = extract_offset(virtual_address);

    //Bounds check 
    if (page_number >= num_pages_) {
        std::cout << "Invalid virtual address\n";
        return 0;
    }

    //PAGE HIT
    if (resident_pages_.count(page_number)) {
        page_hits_++;
        return page_number * page_size_ + offset;
    }

    //PAGE FAULT
    page_faults_++;

    //Evict if memory full
    if (resident_pages_.size() >= num_pages_) {
        std::size_t victim = fifo_queue_.front();
        fifo_queue_.pop();
        resident_pages_.erase(victim);
        page_evictions_++;
    }

    //Page in
    resident_pages_.insert(page_number);
    fifo_queue_.push(page_number);

    return page_number * page_size_ + offset;
}


//Stats
void VirtualMemory::stats() const {
    std::cout << "Virtual Memory Stats\n";
    std::cout << "Page hits: " << page_hits_ << "\n";
    std::cout << "Page faults: " << page_faults_ << "\n";
    std::cout << "Page evictions: " << page_evictions_ << "\n";
}


//Reset
void VirtualMemory::reset() {
    resident_pages_.clear();
    while (!fifo_queue_.empty()) fifo_queue_.pop();
    page_hits_ = 0;
    page_faults_ = 0;
    page_evictions_ = 0;
}