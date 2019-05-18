#include <scene_geometry.h>

manta::SceneGeometry::SceneGeometry() {
	m_id = -1;
}

manta::SceneGeometry::~SceneGeometry() {
	/* void */
}

int manta::SceneGeometry::getId() const {
	return m_id;
}

void manta::SceneGeometry::setId(int id) {
	m_id = id;
}
