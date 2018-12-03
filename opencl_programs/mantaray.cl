__kernel void test_kernel(__global const float *A, __global const float *B, __global float *C) {

	// Get the index of the current element to be processed
	int i = get_global_id(0);

	// Do the operation
	C[i] = A[i] + 2*B[i];
}

__kernel void test_kernel_sqrt(__global const float *A, __global const float *B, __global float *C) {

	// Get the index of the current element to be processed
	int i = get_global_id(0);

	// Do the operation
	C[i] = sqrt(A[i] + 2 * B[i]);
}

typedef struct tag_face {
	float4 edgePlaneVWNormal;
	float4 edgePlaneWUNormal;

	float4 normal;
	float4 p0;
} face;

typedef struct tag_face_v2 {
	float4 edgePlaneVWNormal;
	float4 edgePlaneWUNormal;
	float4 edgePlaneVUNormal;

	float4 normal;
	float4 p0;

	float scaleVW;
	float scaleWU;

	int padding[10];
} face_v2;

typedef struct tag_light_ray {
	float4 ray_dir;
	float4 ray_origin;
} light_ray;

typedef struct tag_output_depths {
	int depths1;
	int depths2;
	int depths3;
	int depths4;
} output_depths;

__kernel void mesh_intersection_test(
	__global const face *faces,
	__global const light_ray *rays,
	__global output_depths *output_depth) {

	int i = get_global_id(0);

	int flags1 = 0;
	int flags2 = 0;
	int flags3 = 0;
	int flags4 = 0;

	for (int r = 0; r < 128; r++) {
		// Get the index of the current element to be processed
		__global const light_ray *ray = &rays[r];

		float denom = dot(faces[i].normal, ray->ray_dir);

		if (denom < 1e-6 && denom > -1e-6) {
			continue;
		}

		float4 p0r0 = faces[i].p0 - ray->ray_origin;
		float depth = dot(p0r0, faces[i].normal) / denom;

		if (depth < 0.0) {
			continue;
		}

		float4 s = ray->ray_origin + (ray->ray_dir * depth);

		float u = dot(s.xyz, faces[i].edgePlaneVWNormal.xyz) - faces[i].edgePlaneVWNormal.w;
		if (u < 0.0 || u > 1.0) {
			continue;
		}

		float v = dot(s.xyz, faces[i].edgePlaneWUNormal.xyz) - faces[i].edgePlaneWUNormal.w;
		if (v < 0.0) {
			continue;
		}

		float w = 1.0 - u - v;
		if (w < 0.0) {
			continue;
		}

		if (r < 32) {
			flags1 |= 0x1 << r;
		}
		else if (r < 64) {
			flags2 |= 0x1 << (r - 32);
		}
		else if (r < 96) {
			flags3 |= 0x1 << (r - 64);
		}
		else if (r < 128) {
			flags4 |= 0x1 << (r - 96);
		}
	}
	output_depth[i].depths1 = flags1;
	output_depth[i].depths2 = flags2;
	output_depth[i].depths3 = flags3;
	output_depth[i].depths4 = flags4;
}

__kernel void mesh_intersection_test_v2(
	__global const face_v2 *faces,
	__global const light_ray *rays,
	__global output_depths *output_depth) {

	int i = get_global_id(0);

	int flags1 = 0;
	int flags2 = 0;
	int flags3 = 0;
	int flags4 = 0;

	for (int r = 0; r < 128; r++) {
		// Get the index of the current element to be processed
		__global const light_ray *ray = &rays[r];

		float denom = dot(faces[i].normal, ray->ray_dir);

		if (denom < 1e-6 && denom > -1e-6) {
			continue;
		}

		float4 p0r0 = faces[i].p0 - ray->ray_origin;
		float depth = dot(p0r0, faces[i].normal) / denom;

		if (depth < 0.0) {
			continue;
		}

		float4 s = ray->ray_origin + (ray->ray_dir * depth);
		float delta = 0.01;

		float u = dot(s.xyz, faces[i].edgePlaneVWNormal.xyz);
		if (u < (faces[i].edgePlaneVWNormal.w - delta * faces[i].scaleVW)) {
			continue;
		}

		float v = dot(s.xyz, faces[i].edgePlaneWUNormal.xyz);
		if (v < (faces[i].edgePlaneWUNormal.w - delta * faces[i].scaleWU)) {
			continue;
		}

		float w = dot(s.xyz, faces[i].edgePlaneVUNormal.xyz);
		if (w < (faces[i].edgePlaneVUNormal.w - delta)) {
			continue;
		}

		if (r < 32) {
			flags1 |= 0x1 << r;
		}
		else if (r < 64) {
			flags2 |= 0x1 << (r - 32);
		}
		else if (r < 96) {
			flags3 |= 0x1 << (r - 64);
		}
		else if (r < 128) {
			flags4 |= 0x1 << (r - 96);
		}
	}
	output_depth[i].depths1 = flags1;
	output_depth[i].depths2 = flags2;
	output_depth[i].depths3 = flags3;
	output_depth[i].depths4 = flags4;
}
