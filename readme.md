Memory Management Simulator

Overview

This project is a user-space simulation of an operating system’s memory management subsystem.
It demonstrates how physical memory allocation, caching, and virtual memory interact in a modern OS.
The simulator is designed for conceptual correctness and clarity, not for cycle-accurate hardware modeling.
All components are implemented in C++ and are controlled via a command-line interface (CLI).

⸻

Features Implemented

1. Physical Memory Simulation
	•	Models a contiguous block of physical memory
	•	Memory is divided into variable-sized blocks
	•	Supports block splitting on allocation
	•	Supports block coalescing on deallocation

⸻

2. Memory Allocation Strategies
	•	First Fit
	•	Best Fit
	•	Worst Fit

Allocator strategy can be switched at runtime using the CLI.

⸻

3. Allocation Interface
	•	malloc <size> — allocate memory
	•	free <block_id> — deallocate memory
	•	dump — display memory layout
	•	stats — show allocation statistics

⸻

4. Metrics & Statistics

The simulator tracks:
	•	Total memory
	•	Used and free memory
	•	Memory utilization
	•	External fragmentation
	•	Allocation requests
	•	Successful and failed allocations

Note: Internal fragmentation is not explicitly tracked, as clarified to be optional.

⸻

5. Buddy Allocation System (Optional Extension)
	•	Power-of-two memory allocation
	•	Allocation requests rounded up to nearest power of two
	•	Recursive splitting and buddy coalescing
	•	Implemented as a separate allocator for comparison

⸻

6. Multilevel Cache Simulation (MUST HAVE)
	•	Two cache levels: L1 and L2
	•	Each cache is:
	•	Set-associative
	•	Configurable in size, block size, and associativity
	•	FIFO replacement policy
	•	Real cache behavior using tag, index, and offset
	•	Tracks hits and misses per cache level
	•	Explicit miss propagation across cache hierarchy

The cache models lookup, replacement, and miss propagation behavior, but does not simulate cycle-accurate access latency.
⸻

7. Virtual Memory Simulation (Optional Extension)
	•	Paging-based virtual memory system
	•	Single-process address space
	•	FIFO page replacement
	•	Page fault handling and page eviction
	•	Disk access simulated representationally
	•	Integrated with cache and physical memory pipeline

⸻

8. Integration Between Components

Memory access follows this pipeline:
Virtual Address
 → Page Table
 → Physical Address
 → L1 Cache
 → L2 Cache
 → Main Memory

⸻

Build Instructions

Requirements
	•	C++17 compatible compiler (clang++ or g++)
	•	make

Build and Run->
    make clean
    make
    ./memsim

CLI Usage Examples

Physical Memory
    init memory 1024
    set allocator first_fit
    malloc 100
    malloc 200
    free 1
    dump
    stats

Buddy Allocator
    init memory 512
    set allocator buddy
    malloc 60
    free 1
    dump

Cache Simulation
    cache init L1 64 8 2
    cache init L2 128 8 2
    access 26
    access 27
    cache dump
    cache stats

Virtual Memory
    vm init 16 8
    vaccess 32
    vaccess 35
    vm stats


⸻

Assumptions and Simplifications

The following design choices were made intentionally:
	•	The simulator runs entirely in user space
	•	Physical memory contents are not stored, only metadata
	•	Disk access is representational (no real I/O)
	•	Cache write policies are not simulated
	•	No TLB simulation
	•	No multi-process virtual address spaces
	•	No cycle-accurate timing
	•	Internal fragmentation is not explicitly computed

These simplifications align with the project scope and official clarifications.

⸻

Project Structure

Memory-Simulator/
├── README.md
├── design.md
├── include/
├── src/
├── Makefile

Documentation
	•README.md — overview, features, usage
	•docs/design.md — detailed design decisions, assumptions, and architecture

Test Artifacts

The simulator includes manual CLI test workloads demonstrating allocation patterns, cache hits/misses, and virtual memory accesses. Expected behaviors are described in the documentation and demonstrated in the project demo.