# Advanced Cpp End of Term

End of term exam for the Advanced C++ course of Università degli studi di Verona

## Solution Structure

- **BigInt**: Contains a class describing a possible implementation of an infinite integer.
The project contains a main with multiple unit tests testing the features implemented.
  
- **SList**: Contains a simple custom implementation of an STL Forward list.

- **ShirosMemoryManager**: A static library that expose a custom general purpose Memory Manager.
  The memory manager uses two different types of allocators:
  - **FreeListAllocator**: Handles large objects allocation;
  - **SmallObjectAllocator**: Handles small objects allocation.
    
- **MemoryManager**: An empty project containing a set of unit tests to assert the correct
behavior for the features implemented in the static library **ShirosMemoryManager**.

## Usage
**BigInt, MemoryManager and SList** are project that contains an executable main. Se one
of those Visual Studio project as **Startup Project** to run all tests for that exercise.

## Futures