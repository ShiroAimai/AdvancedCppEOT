#include "pch.h"
#include "FreeListAllocator.h"

namespace {
	/** Given an address it computes its padding taking into account the passed alignment */
	const std::size_t ComputePadding(const std::size_t InAddress, const std::size_t InAlignment)
	{
		std::size_t padding = InAlignment - (InAddress & InAlignment);

		return (padding == InAlignment)
			? 0 //aligned
			: padding;
	}

	/** Given an address and an header size, it computes its padding taking into account the passed alignment */
	const std::size_t ComputePaddingWithHeader(const std::size_t InAddress, const std::size_t InAlignment, const std::size_t InHeaderSize) {
		return InHeaderSize + ComputePadding(InAddress + InHeaderSize, InAlignment);
	}

	/** Check if the given address is aligned with the alignment passed */
	bool isAligned(const std::size_t address, const std::size_t alignment)
	{
		return ComputePadding(address, alignment) == 0;
	}

}

FreeListAllocator::FreeListAllocator(const std::size_t TotalSize, const FitPolicy policy)
	: m_totalSizeAllocated(TotalSize), m_policy(policy)
{
	Reset();
}

void FreeListAllocator::Release()
{
	free(mp_start);
	mp_start = nullptr;
}

FreeListAllocator::~FreeListAllocator()
{
	Release();
}

void FreeListAllocator::Reset()
{
	if (mp_start)
	{
		Release();
	}

	mp_start = malloc(m_totalSizeAllocated);

	//interpret allocated memory as a unique big Node
	Node* head = static_cast<Node*>(mp_start);
	
	assert(head != nullptr);

	head->data.blockSize = m_totalSizeAllocated;
	head->next = nullptr;
	m_freeList.head = nullptr;
	m_freeList.insert(nullptr, head);
}

void* FreeListAllocator::Allocate(const std::size_t AllocationSize, const std::size_t alignment, size_t& OutAllocationSize)
{
	assert(AllocationSize > 0 && alignment > 0 && "Allocation Size and Alignment must be positive");
	
	static constexpr std::size_t allocationHeaderSize = sizeof(FreeListAllocator::AllocatedBlockHeader);

	//Linearly search the list for a block that has enough space to allocate AllocationSize bytes
	std::size_t OutNewAddressPadding;
	Node* OutResultNode,* OutPrevNode;
	OutResultNode = OutPrevNode = nullptr;
	
	Find(AllocationSize, alignment, OutNewAddressPadding, OutPrevNode, OutResultNode);
	
	assert(OutResultNode && "Not enough Memory for any new Block");

	//OutNewAddressPadding contains both 
	//subtract AllocatedBlockHeader size to get just the alignment padding
	const std::size_t alignmentPadding = OutNewAddressPadding - allocationHeaderSize; 
	//required size is (RequestedAllocationSize + Padding)
	const std::size_t requiredSize = AllocationSize + OutNewAddressPadding;

	const std::size_t remainingBlockSize = OutResultNode->data.blockSize - requiredSize;
	const std::size_t resNodeAddress = reinterpret_cast<std::size_t>(OutResultNode);

	if (remainingBlockSize > 0)
	{
		//add new free block of size "remainingBlockSize" just after the result node
		Node* freeNode = reinterpret_cast<Node*>(resNodeAddress + requiredSize);
		freeNode->data.blockSize = remainingBlockSize;
		m_freeList.insert(OutResultNode, freeNode);
	}

	m_freeList.remove(OutPrevNode, OutResultNode); //detach resultNode from freeList in order to use it

	//setup data for allocation block
	const std::size_t AllocatedBlockHeaderAddress = resNodeAddress + alignmentPadding;
	const std::size_t dataAddress = AllocatedBlockHeaderAddress + allocationHeaderSize;
	AllocatedBlockHeader* _header = reinterpret_cast<AllocatedBlockHeader*>(AllocatedBlockHeaderAddress);
	_header->blockSize = requiredSize;
	_header->padding = static_cast<char>(alignmentPadding);

	assert(isAligned(AllocatedBlockHeaderAddress, alignof(AllocatedBlockHeader)));

	OutAllocationSize = requiredSize;
	return reinterpret_cast<void*>(dataAddress);
}

size_t FreeListAllocator::Deallocate(void* ptr)
{
	static constexpr std::size_t AllocationHeaderSize = sizeof(AllocatedBlockHeader);
	
	const std::size_t address = reinterpret_cast<std::size_t>(ptr);
	//get back allocationHeaderSize from ptr subtracting header block size
	const std::size_t allocatedBlockHeaderAddress = address - AllocationHeaderSize;
	//reinterpret the obtained address as a pointer to AllocationBlockHeader to access its members
	const AllocatedBlockHeader* allocatedBlockHeader = reinterpret_cast<AllocatedBlockHeader*>(allocatedBlockHeaderAddress);

	assert(allocatedBlockHeader != nullptr);
	assert(allocatedBlockHeader->blockSize >= AllocationHeaderSize);

	const std::size_t AlignmentPadding = static_cast<std::size_t>(allocatedBlockHeader->padding);
	const std::size_t DeallocationSize = allocatedBlockHeader->blockSize;

	//retrieve base address subtracting alignmentPadding e allocation header size from ptr
	Node* freeNode = reinterpret_cast<Node*>(allocatedBlockHeaderAddress - AlignmentPadding);
	freeNode->data.blockSize = DeallocationSize; //take back allocated size
	freeNode->next = nullptr;

	//iterate and insert the new free node back in the correct position inside the free list
	Node* it = m_freeList.head, * prev = nullptr;
	for (; it != nullptr; it = it->next) {
		if (ptr < it) {
			m_freeList.insert(prev, freeNode); //found the correct position inside the list
			break;
		}
		prev = it;
	}
	//try to merge contiguous nodes into a unique free block
	Coalescence(prev, freeNode);

	return DeallocationSize;
}

void FreeListAllocator::Coalescence(Node* prevBlock, Node* freeBlock)
{
	if (freeBlock->next != nullptr && (reinterpret_cast<std::size_t>(freeBlock) + freeBlock->data.blockSize) == reinterpret_cast<std::size_t>(freeBlock->next))
	{
		freeBlock->data.blockSize += freeBlock->next->data.blockSize;
		m_freeList.remove(freeBlock, freeBlock->next);
	}

	if (prevBlock != nullptr && (reinterpret_cast<std::size_t>(prevBlock) + prevBlock->data.blockSize) == reinterpret_cast<std::size_t>(freeBlock))
	{
		prevBlock->data.blockSize += freeBlock->data.blockSize;
		m_freeList.remove(prevBlock, freeBlock);
	}
}

void FreeListAllocator::Find(const std::size_t InSize, const std::size_t InAlignment, std::size_t& OutPadding, Node*& OutPreviousNode, Node*& OutFoundNode)
{
	switch (m_policy)
	{
	case FitPolicy::BEST_FIT:
		FindBest(InSize, InAlignment, OutPadding, OutPreviousNode, OutFoundNode);
		break; 
	case FitPolicy::FIRST_FIT:
		FindFirst(InSize, InAlignment, OutPadding, OutPreviousNode, OutFoundNode);
		break;
	}
}

void FreeListAllocator::FindBest(const std::size_t size, const std::size_t alignment, std::size_t& padding, Node*& previousNode, Node*& resNode)
{
	// Iterate the whole list and return a ptr with the best fit
	
	static constexpr std::size_t AllocationBlockHeaderAlignment = alignof(AllocatedBlockHeader);

	Node* bestBlock = nullptr, *prevBest = nullptr;
	Node* it = m_freeList.head,
		* prev = nullptr;
	/** Current smallest difference (blockSize - requiredSize) among all FreeBlock blocks*/
	std::size_t smallestDiff = std::numeric_limits<std::size_t>::max();
	for (; it != nullptr; it = it->next)
	{
		const std::size_t InAlignment = std::max(AllocationBlockHeaderAlignment, alignment);
		padding = ComputePaddingWithHeader(reinterpret_cast<std::size_t>(it), InAlignment, sizeof(AllocatedBlockHeader));
		const std::size_t requiredSpace = size + padding;
		if (it->data.blockSize >= requiredSpace && ((it->data.blockSize - requiredSpace) < smallestDiff)) {
			smallestDiff = it->data.blockSize - requiredSpace;
			bestBlock = it;
			prevBest = prev;
		}
		prev = it;
	}
	
	previousNode = prevBest;
	resNode = bestBlock;
}

void FreeListAllocator::FindFirst(const std::size_t size, const std::size_t alignment, std::size_t& padding, Node*& previousNode, Node*& resNode)
{
	//just iterate list and return first node that can handle a new block of size "size"
	Node* it = m_freeList.head, * prev = nullptr;

	const std::size_t AllocationBlockHeaderAlignment = alignof(AllocatedBlockHeader);
	for (; it != nullptr; it = it->next)
	{
		const std::size_t InAlignment = AllocationBlockHeaderAlignment > alignment
			? AllocationBlockHeaderAlignment
			: alignment;
		padding = ComputePaddingWithHeader(reinterpret_cast<std::size_t>(it), InAlignment, sizeof(AllocatedBlockHeader));
		const std::size_t requiredSpace = size + padding;
		if (it->data.blockSize >= requiredSpace) {
			break; //this node can handle the required space
		}
		prev = it;
	}

	previousNode = prev;
	resNode = it;
}