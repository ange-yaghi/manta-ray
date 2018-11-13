#include <pch.h>
#include "utilities.h"

#include <stack_allocator.h>

TEST(MemoryTests, StackSanityCheck) {
	manta::StackAllocator s;
	s.initialize(100 * sizeof(int));

	int *ints = (int *)s.allocate(100 * sizeof(int), 1);

	for (int i = 0; i < 100; i++) {
		ints[i] = i;
	}

	s.free(ints);
}

TEST(MemoryTests, StackMultipleFrames) {
	
	manta::StackAllocator s;
	s.initialize(2048);

	int **ints = (int **)s.allocate(100 * sizeof(int *), 1);

	for (int i = 0; i < 100; i++) {
		ints[i] = (int *)s.allocate(sizeof(int));
	}

	for (int i = 0; i < 100; i++) {
		*ints[i] = i;
	}

	for (int i = 0; i < 100; i++) {
		EXPECT_EQ(*ints[i], i);
	}

	for (int i = 99; i >= 0; i--) {
		s.free((void *)ints[i]);
	}

	s.free((void *)ints);
}

TEST(MemoryTests, StackAlignedAllocate) {

	manta::StackAllocator s;
	s.initialize(2048);

	void *test4 = s.allocate(4, 4);
	EXPECT_TRUE((unsigned __int64)test4 % 4 == 0);

	void *test8 = s.allocate(4, 8);
	EXPECT_TRUE((unsigned __int64)test4 % 8 == 0);

	void *test16 = s.allocate(4, 16);
	EXPECT_TRUE((unsigned __int64)test4 % 16 == 0);

	s.free(test16);
	s.free(test8);
	s.free(test4);
}
