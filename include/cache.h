#ifndef CACHE_H
#define CACHE_H

#include <vector>
#include <deque>
#include <cstddef>
#include <string>

/*
  Set-Associative cache(FIFO Replacement)
  Address format:   TAG   |  INDEX  | OFFSET |
  cache_size:total cache size in bytes
  block_size:size of one cache block in bytes
  associativity:number of ways per set
 
  number_of_sets = cache_size / (block_size * associativity)
*/

class Cache {
public:
    Cache();

    //Initialize cache parameters
    //Returns false if configuration is invalid
    bool init(const std::string &name,
              std::size_t cache_size,
              std::size_t block_size,
              std::size_t associativity);

    //Access a memory address
    //Returns true if HIT, false if MISS
    bool access(std::size_t address);
    //Reset cache contents and statistics
    void reset();
    //Dump cache contents(per set, FIFO order)
    void dump() const;
    //Print cache statistics
    void stats() const;
    // Stats getters(for hierarchy reporting)
    std::size_t hits() const { return hits_; }
    std::size_t misses() const { return misses_; }

private:
    //Cache line metadata
    struct CacheLine {
        bool valid;
        std::size_t tag;
    };
    //Cache configuration
    std::string name_;
    std::size_t cache_size_;
    std::size_t block_size_;
    std::size_t associativity_;
    std::size_t num_sets_;
    //Address bit calculations
    std::size_t offset_bits_;
    std::size_t index_bits_;
    //Cache storage: vector of sets, FIFO per set
    std::vector<std::deque<CacheLine>> sets_;
    //Statistics
    std::size_t hits_;
    std::size_t misses_;

private:
    //Helpers
    bool is_power_of_two(std::size_t x) const;
    std::size_t extract_index(std::size_t address) const;
    std::size_t extract_tag(std::size_t address) const;
};

#endif