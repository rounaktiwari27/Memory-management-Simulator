#include "cache.h"
#include <iostream>
#include <cmath>


Cache::Cache()
    : cache_size_(0),
      block_size_(0),
      associativity_(0),
      num_sets_(0),
      offset_bits_(0),
      index_bits_(0),
      hits_(0),
      misses_(0) {}


//Helpers 
bool Cache::is_power_of_two(std::size_t x) const {
    return x > 0 && (x & (x - 1)) == 0;
}

std::size_t Cache::extract_index(std::size_t address) const {
    if (index_bits_ == 0) return 0;
    std::size_t index_mask = (std::size_t(1) << index_bits_) - 1;
    return (address >> offset_bits_) & index_mask;
}

std::size_t Cache::extract_tag(std::size_t address) const {
    return address >> (offset_bits_ + index_bits_);
}


//Initialization
bool Cache::init(const std::string &name,
                 std::size_t cache_size,
                 std::size_t block_size,
                 std::size_t associativity) {

    if (cache_size == 0 || block_size == 0 || associativity == 0) {
        std::cout << "Invalid cache parameters\n";
        return false;
    }

    if (!is_power_of_two(block_size) || !is_power_of_two(associativity)) {
        std::cout << "Block size and associativity must be power of two\n";
        return false;
    }

    if (cache_size < block_size * associativity) {
        std::cout << "Cache size too small\n";
        return false;
    }

    name_ = name;
    cache_size_ = cache_size;
    block_size_ = block_size;
    associativity_ = associativity;

    num_sets_ = cache_size_ / (block_size_ * associativity_);

    if (!is_power_of_two(num_sets_)) {
        std::cout << "Number of sets must be power of two\n";
        return false;
    }

    offset_bits_ = static_cast<std::size_t>(std::log2(block_size_));
    index_bits_  = static_cast<std::size_t>(std::log2(num_sets_));

    sets_.clear();
    sets_.resize(num_sets_);

    hits_ = 0;
    misses_ = 0;

    return true;
}


//Access
bool Cache::access(std::size_t address) {
    std::size_t index = extract_index(address);
    std::size_t tag   = extract_tag(address);

    auto &set = sets_[index];

    //Check for hit
    for (const auto &line : set) {
        if (line.valid && line.tag == tag) {
            hits_++;
            return true;
        }
    }

    //Miss
    misses_++;

    //FIFO eviction if set is full
    if (set.size() >= associativity_) {
        set.pop_front();
    }

    //Insert new line
    set.push_back({true, tag});

    return false;
}


//Reset
void Cache::reset() {
    for (auto &set : sets_) {
        set.clear();
    }
    hits_ = 0;
    misses_ = 0;
}


//Dump
void Cache::dump() const {
    std::cout << name_ << " Cache Contents:\n";
    for (std::size_t i = 0; i < num_sets_; ++i) {
        std::cout << "Set " << i << ": ";
        for (const auto &line : sets_[i]) {
            std::cout << "[T=" << line.tag << "] ";
        }
        std::cout << "\n";
    }
}


//Stats
void Cache::stats() const {
    std::size_t total = hits_ + misses_;
    double hit_rate = (total == 0) ? 0.0 : (double)hits_ / total;

    std::cout << name_ << " Cache Stats\n";
    std::cout << "Hits: " << hits_ << "\n";
    std::cout << "Misses: " << misses_ << "\n";
    std::cout << "Hit Rate: " << hit_rate * 100 << "%\n";
}