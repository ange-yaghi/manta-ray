#include <primitives.h>

manta::math::real manta::AABB::surfaceArea() {
	math::Vector d = math::sub(maxPoint, minPoint);

	return 2 * (
		math::getX(d) * math::getY(d) +
		math::getX(d) * math::getZ(d) +
		math::getY(d) * math::getZ(d));
}

void manta::calculateAABB(const Triangle *triangle, AABB *target) {
	target->minPoint = triangle->u;
	target->maxPoint = triangle->u;

	target->minPoint = math::componentMin(target->minPoint, triangle->v);
	target->minPoint = math::componentMin(target->minPoint, triangle->w);

	target->maxPoint = math::componentMax(target->maxPoint, triangle->v);
	target->maxPoint = math::componentMax(target->maxPoint, triangle->w);
}

void manta::AABB::merge(const AABB &b) {
	minPoint = math::componentMin(minPoint, b.minPoint);
	maxPoint = math::componentMax(maxPoint, b.maxPoint);
}
