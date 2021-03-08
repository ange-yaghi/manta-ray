#ifndef YDS_DYNAMIC_ALLOCATOR_H
#define YDS_DYNAMIC_ALLOCATOR_H

#include "yds_memory_base.h"

typedef unsigned short int BlockIndex;

struct BlockLink {
    void *Address;
    unsigned int Size;
    unsigned int NumObjects;

    BlockIndex Index;
    char Free;
    char Valid;

    BlockLink *Next;
    BlockLink *Previous;
};

// --
// Standard allocator for allocating blocks of any 
// size.
// --
class ysDynamicAllocator : public ysMemoryAllocator {
public:
    ysDynamicAllocator();
    ~ysDynamicAllocator();

    virtual void *AllocateBlock(int size, int numObjects = 1);
    virtual int FreeBlock(void *block);
    virtual void Destroy();

public:
    // --
    // Returns memory address validity.
    //
    //   address: Memory address
    //
    // Return:
    //   0: Valid memory address
    //   1: Memory address too large
    //   -1: Memory address too small
    // --
    int CheckMemoryAddress(void *address);

    // --
    // Set the allocator's parent. This will cause this allocator
    // to take control of a segment of the parent's memory.
    //
    //   parent: Memory allocator parent
    // 
    // --
    void SetParent(ysMemoryAllocator *parent);

    // --
    // Allocate the buffer to be used by the allocator.
    //
    //   size: Total size of the buffer (bytes)
    //
    // --
    void CreateBuffer(int size);

    // --
    // Initialize the block list to be used by the allocator.
    // 
    //   maxBlocks: Number of blocks (ie. maximum number of allocations)
    // 
    // --
    void CreateBlocks(int maxBlocks);

public:

    /* ERROR CHECKING */

    // --
    // Checks whether all blocks are valid and there
    // are no memory violations within the allocator
    // structure.
    // --
    bool CheckValid();

protected:

    // --
    // Retrieve the free block with the largest size.
    // --
    BlockLink *UpdateAllocatorBlock();

    // --
    // Add a new block the block queue.
    //
    // Returns a pointer to the new block.
    // --
    inline BlockLink *AddBlock() {
        if (m_nBlocks >= m_maxBlocks) return NULL;

        BlockLink *newBlock = m_blocks[m_nBlocks++];
        newBlock->Valid = 1;

        return newBlock;

    }

    // --
    // Remove a block from the block queue.
    // 
    //   index: Index of the block to remove.
    // 
    // --
    void RemoveBlock(BlockIndex index);

private:
    // Buffer from which the allocator allocates data
    void *m_data;

    // Main pool of preallocated blocks
    BlockLink *m_blockPool;

    // Table of pointers, to prevent having to reoder full structures
    BlockLink **m_blocks;

    // Current free block to perform allocations with (also usually the largest)
    BlockLink *m_allocatorBlock;

    // Current number of blocks in use by the allocator
    int m_nBlocks;


    // The number of blocks in the allocator pool
    int m_maxBlocks;

    // The size of the total allocation pool
    int m_maxSize;


    // Parent
    ysMemoryAllocator *m_parent;

private:
    // Total allocation tracking, mainly for debugging purposes
    int m_totalAllocation;
};

class ysCompositeAllocator : public ysDynamicAllocator {
public:
    ysCompositeAllocator() { m_currentAllocator = 0; }
    ~ysCompositeAllocator() {}

    void Initialize(int subdivisions, int subdivisionSize, int blocksPerSubdivision) {
        m_numSubdivisions = subdivisions;

        int bufferSize = subdivisions * subdivisionSize;
        int blockListSize = subdivisions * (sizeof(BlockLink) + sizeof(BlockLink *)) * blocksPerSubdivision;
        int allocatorListSize = subdivisions * sizeof(ysDynamicAllocator *);
        int padding = 100 * KB;

        m_parent.CreateBuffer(bufferSize + blockListSize + allocatorListSize + padding);
        m_parent.CreateBlocks(subdivisions * 4 + 1);

        m_allocators = m_parent.Allocate<ysDynamicAllocator>(subdivisions);

        for (int i = 0; i < subdivisions; i++) {
            m_allocators[i].SetParent(&m_parent);
            m_allocators[i].CreateBuffer(subdivisionSize);
            m_allocators[i].CreateBlocks(blocksPerSubdivision);
        }
    }

    virtual void *AllocateBlock(int size, int numObjects = 1) {
        void *result;
        int startIndex = m_currentAllocator;

        do {
            result = m_allocators[m_currentAllocator++].AllocateBlock(size, numObjects);
            if (m_currentAllocator >= m_numSubdivisions) m_currentAllocator = 0;

        } while (!result && m_currentAllocator != startIndex);

        return result;
    }

    virtual int FreeBlock(void *block) {
        int left = 0;
        int right = m_numSubdivisions - 1;

        while (true) {
            int middle = (left + right) / 2;
            int result = m_allocators[middle].CheckMemoryAddress(block);

            if (result == 1) {
                left = middle + 1;
            }
            else if (result == -1) {
                right = middle - 1;
            }
            else {
                return m_allocators[middle].FreeBlock(block);
                break;
            }
        }
    }

protected:
    int m_currentAllocator;

    ysDynamicAllocator m_parent;

    ysDynamicAllocator *m_allocators;
    int m_numSubdivisions;
};

#endif /* YDS_DYNAMIC_ALLOCATOR_H */
