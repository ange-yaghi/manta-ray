#include <scene_object.h>

manta::SceneObject::SceneObject() {
	m_name = "";
	m_defaultMaterial = nullptr;
	m_geometry = nullptr;
}

manta::SceneObject::~SceneObject() {

}
