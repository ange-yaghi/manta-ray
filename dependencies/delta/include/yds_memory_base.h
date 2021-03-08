#ifndef YDS_MEMORY_BASE_H
#define YDS_MEMORY_BASE_H

#include "yds_base.h"

#include <malloc.h>
#include <new>
#include <stddef.h>
#include <assert.h>

static const int KB = 1024;
static const int MB = KB * KB;
static const int GB = KB * MB;

// --
// Standard base class for all memory allocators.
// Includes the standard interface for allocating
// generic and specific blocks.
// --
class ysMemoryAllocator : public ysObject
{

public:

    ysMemoryAllocator();
    ysMemoryAllocator(const char *typeID);
    ~ysMemoryAllocator();

    /* Main Interface */

    // --
    // Allocate a block of memory with a specific type.
    //
    //   n: Number of objects to allocate (ie array length)
    // 
    // Returns the allocated array.
    // --
    template<typename TYPE>
    TYPE *Allocate(unsigned int n=1)
    {

        // Allocate an empty, general block of memory
        void *block = AllocateBlock(sizeof(TYPE) * n, n);
        if (!block) return 0;

        // Allocate each of the elements in the array
        for(unsigned int i=0; i < n; i++) 
            new((char *)block + i * sizeof(TYPE)) TYPE;

        return (TYPE *)block;

    }

    // --
    // Free a block of memory with a specific type.
    //
    //   data: Reference to a pointer, the pointer is set to NULL after the operation
    // 
    // --
    template<typename TYPE>
    void Free(TYPE * &data)
    {

        int nObjects = FreeBlock((void *)data);
        for(int i=0; i < nObjects; i++) data[i].~TYPE();

        data = 0;

    }

    /* Low Level Interface */

    // --
    // Allocate an arbitrary block of memory.
    //
    //   size: Total size (bytes) to allocate.
    //   numObjects: Expected number of objects to fill the block (in the case of allocating an array).
    //        
    // Returns a pointer to the new memory block (NULL for failure).
    // --
    virtual void *AllocateBlock(int size, int numObjects=1) = 0;

    // --
    // Free a block of memory.
    //
    //   block: Pointer to target memory address.
    //
    // Returns the number of objects in the freed memory block.
    // --
    virtual int FreeBlock(void *block) = 0;

    // --
    // Destroy all allocated memory used by the allocator.
    //
    // NOTE: This should be called before the allocator's destructor.
    // Also, any objects allocated within the address space of the allocator
    // left after a Destroy() call, will not have their destructors called.
    // --
    virtual void Destroy() {}

};

#endif