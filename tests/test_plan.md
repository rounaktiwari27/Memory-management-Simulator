# Test Artifacts – Memory Management Simulator

This document lists the manual CLI test cases used to verify each feature
of the simulator. Outputs were visually inspected and matched expected behavior.

---

## Physical Memory & Allocation

init memory 1024  
malloc 100  
malloc 200  
free 1  
dump  

Expected:
- Block splitting on allocation
- Block coalescing on free

---

## Allocation Strategies

set allocator first_fit  
set allocator best_fit  
set allocator worst_fit  

Expected:
- Different free blocks chosen based on strategy

---

## Metrics & Statistics

stats  

Expected:
- Correct used/free memory
- Allocation counters updated
- External fragmentation reported

---

## Buddy Allocator

init memory 512  
set allocator buddy  
malloc 60  
free 1  
dump  

Expected:
- Power-of-two rounding
- Recursive split and coalescing

---

## Cache Simulation

cache init L1 64 8 2  
cache init L2 128 8 2  
access 26  
access 27  
access 26  
cache stats  

Expected:
- L1/L2 hit and miss reporting
- FIFO replacement
- Miss propagation

---

## Virtual Memory

vm init 16 8  
vaccess 32  
vaccess 35  
vaccess 32  
vm stats  

Expected:
- Page fault on first access
- FIFO page replacement
- Page hit on repeated access