#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

namespace manta {

	class SceneGeometry;
	class Material;

	class SceneObject {
	public:
		SceneObject();
		~SceneObject();

		void setGeometry(SceneGeometry *geometry) { m_geometry = geometry; }
		SceneGeometry *getGeometry() { return m_geometry; }

		void setMaterial(Material *material) { m_material = material; }
		Material *getMaterial() { return m_material; }

	protected:
		SceneGeometry *m_geometry;
		Material *m_material;
	};

}

#endif /* SCENE_OBJECT_H */