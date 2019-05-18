#include <material_pointer.h>

manta::MaterialPointer::MaterialPointer() {
	m_material = nullptr;
}

manta::MaterialPointer::~MaterialPointer() {
	/* void */
}

void manta::MaterialPointer::generateRays(RayContainer *rays, const LightRay &incidentRay, const IntersectionPoint &intersectionPoint, int degree, StackAllocator *stackAllocator) const {
	m_material->generateRays(rays, incidentRay, intersectionPoint, degree, stackAllocator);
}

void manta::MaterialPointer::integrateRay(LightRay *ray, const RayContainer &rays, const IntersectionPoint &intersectionPoint) const {
	m_material->integrateRay(ray, rays, intersectionPoint);
}
