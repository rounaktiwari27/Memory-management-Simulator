Design Document

Memory Management Simulator

⸻

1. Purpose of the Simulator

This project simulates the memory management subsystem of an operating system in user space.
The aim is to demonstrate how different OS-level memory management components work together, rather than to build a real kernel or a cycle-accurate hardware model.

The simulator focuses on correct logical behavior, clear abstractions, and traceable execution, allowing users to observe how memory is allocated, cached, and translated from virtual to physical addresses.

______

2. High-Level Architecture

The simulator is composed of the following major components:
	1.	Physical Memory Allocator
	2.	Allocation Strategies (First Fit, Best Fit, Worst Fit)
	3.	Buddy Allocation System (optional extension)
	4.	Multilevel Cache (L1 + L2)
	5.	Virtual Memory System (paging-based, optional extension)
	6.	CLI-based Integration Layer

The data flow during memory access follows this order:
Virtual Address
 → Page Table
 → Physical Address
 → L1 Cache
 → L2 Cache
 → Main Memory

_____

3. Physical Memory Model

Assumptions
	•	Physical memory is modeled as a single contiguous address space.
	•	Memory is measured in bytes.
	•	Actual memory contents are not stored; only metadata about blocks is tracked.

Design
	•	Memory is divided into variable-sized blocks.
	•	Each block contains:
	•	starting address
	•	size
	•	allocation status
	•	allocation ID
	•	Allocation splits blocks when required.
	•	Deallocation coalesces adjacent free blocks.

This models how an OS kernel manages heap or general-purpose physical memory regions.

⸻

4. Allocation Strategies

The simulator supports the following allocation strategies:
	•	First Fit:
Allocates the first free block large enough to satisfy the request.
	•	Best Fit:
Allocates the smallest free block that can satisfy the request.
	•	Worst Fit:
Allocates the largest available free block.

Each strategy uses the same underlying block structure but differs in how the free list is searched.

Design Choice

Linked lists were used to store memory blocks because:
	•	they naturally support splitting and merging
	•	order preservation helps in visualizing memory layout

This choice aligns with the flexibility allowed in the project hints.

⸻

5. Metrics and Statistics

The simulator tracks the following metrics:
	•	Total memory
	•	Used memory
	•	Free memory
	•	Memory utilization
	•	External fragmentation
	•	Allocation requests
	•	Successful allocations
	•	Failed allocations

Internal Fragmentation

Internal fragmentation is not explicitly computed.
This decision was made based on project clarifications stating that fragmentation handling is optional. The simulator focuses on external fragmentation, which is more relevant to allocation strategies.

This limitation is intentional and documented to avoid ambiguity during evaluation.

⸻

6. Buddy Allocation System (Optional Extension)

The buddy allocator models a classic OS memory allocator with power-of-two blocks.

Design
	•	Total memory size must be a power of two.
	•	Allocation requests are rounded up to the nearest power of two.
	•	Free lists are maintained per block size.
	•	Buddy blocks are identified using XOR on block addresses.
	•	Recursive splitting occurs on allocation.
	•	Recursive coalescing occurs on deallocation.

This component is implemented independently of the variable-sized allocator to demonstrate an alternative memory management approach.

⸻

7. Multilevel Cache Design

Cache Structure
	•	Two cache levels: L1 and L2
	•	Each cache level is:
	•	set-associative
	•	configurable in size, block size, and associativity
	•	Replacement policy: FIFO

Address Breakdown

Each memory address is divided into:
| Tag | Index | Offset |

•	Offset selects the byte within a block.
•	Index selects the cache set.
•	Tag is matched against cache lines within the set.

Replacement Policy

FIFO replacement is applied within each cache set.
When a set is full, the oldest cache line in that set is evicted.

Miss Propagation
	•	L1 miss → check L2
	•	L2 miss → access main memory
	•	Each miss is logged and counted in statistics

Numeric cache latency is not simulated. Instead, the simulator logs the level at which an access is resolved, which satisfies the conceptual requirement of miss penalty propagation.

⸻

8. Virtual Memory System (Optional Extension)

Model

The virtual memory subsystem uses paging and simulates a single-process address space.

Design Choices
	•	Virtual addresses are divided into:
    | Page Number | Offset |

	•	Page table tracks only whether a page is resident.
	•	FIFO page replacement is used.
	•	Disk access is representational, not physically modeled.
	•	No frame allocator is implemented, as permitted by project clarifications.

Page Fault Handling
	•	If a page is not resident, a page fault occurs.
	•	The page is loaded into memory.
	•	If memory is full, the oldest page is evicted using FIFO.

⸻

9. Integration Between Components

When a virtual address is accessed:
	1.	The page table is checked.
	2.	On a page fault, the page is loaded from disk (representational).
	3.	The virtual address is translated to a physical address.
	4.	The physical address is checked in L1 cache.
	5.	On miss, L2 cache is checked.
	6.	On further miss, main memory is accessed.

This end-to-end pipeline models how a real OS resolves memory accesses across multiple layers.

⸻

10. Limitations and Simplifications

The following aspects are intentionally not implemented:
	•	Multi-process virtual address spaces
	•	TLB simulation
	•	Write policies in cache
	•	Cycle-accurate timing
	•	Actual disk I/O
	•	Internal fragmentation accounting

These simplifications keep the simulator focused on core memory-management concepts, as intended by the project scope and clarified in discussions.

⸻

11. Conclusion

This simulator demonstrates the interaction between:
	•	physical memory allocation
	•	caching
	•	virtual memory translation

while maintaining clarity, correctness, and traceability.
All design decisions are documented explicitly to prevent misinterpretation during evaluation.

⸻
