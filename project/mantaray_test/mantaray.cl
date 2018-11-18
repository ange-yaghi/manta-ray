__kernel void test_kernel(__global const float *A, __global const float *B, __global float *C) {

	// Get the index of the current element to be processed
	int i = get_global_id(0);

	// Do the operation
	C[i] = A[i] + B[i];
}