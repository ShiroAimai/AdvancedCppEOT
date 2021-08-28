# Advanced Cpp End of Term

End of term exam for the Advanced C++ course of Università degli studi di Verona

## Solution Structure

- **BigInt**: Contains a class modelling my implementation of an infinite integer.
  
- **SList**: Contains a simple custom implementation of an STL Forward list.

- **ShirosMemoryManager**: A static library that exposes a custom general purpose Memory Manager.
  The memory manager makes use of two different types of allocators:
  - **FreeListAllocator**: Handles allocation of large objects (Greater than a fixed cap size);
  - **SmallObjectAllocator**: Handles allocation of small objects.
    
- **MemoryManager**: An empty project containing a set of unit tests to assert the correct
behavior for the features implemented in the static library **ShirosMemoryManager**.

## Usage
**BigInt, MemoryManager and SList** are project that contains an executable main. Set one
of those Visual Studio project as **Startup Project** to run all associated tests for that exercise.

## Futures