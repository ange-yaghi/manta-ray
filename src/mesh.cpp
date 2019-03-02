#include <mesh.h>

#include <light_ray.h>
#include <intersection_point.h>
#include <obj_file_loader.h>
#include <coarse_intersection.h>
#include <standard_allocator.h>
#include <material.h>
#include <material_manager.h>
#include <primitives.h>

manta::Mesh::Mesh() {
	m_faces = nullptr;
	m_auxFaceData = nullptr;
	m_quadFaces = nullptr;
	m_auxQuadFaceData = nullptr;
	m_vertices = nullptr;
	m_normals = nullptr;
	m_textureCoords = nullptr;

	m_triangleFaceCount = 0;
	m_quadFaceCount = 0;
	m_vertexCount = 0;
	m_normalCount = 0;
	m_texCoordCount = 0;

	m_precomputedValues = nullptr;

	m_fastIntersectEnabled = false;
	m_fastIntersectRadius = (math::real)0.0;

	m_perVertexNormals = false;
}

manta::Mesh::~Mesh() {
	assert(m_faces == nullptr);
	assert(m_auxFaceData == nullptr);
	assert(m_quadFaces == nullptr);
	assert(m_auxQuadFaceData == nullptr);
	assert(m_vertices == nullptr);
	assert(m_normals == nullptr);
	assert(m_textureCoords == nullptr);
	assert(m_precomputedValues == nullptr);
}

void manta::Mesh::initialize(int faceCount, int vertexCount, int normalCount, int texCoordCount) {
	m_faces = StandardAllocator::Global()->allocate<Face>(faceCount);
	m_auxFaceData = StandardAllocator::Global()->allocate<AuxFaceData>(faceCount);

	// By default assume there are no quads in the mesh
	m_quadFaces = nullptr;
	m_auxQuadFaceData = nullptr;

	m_vertices = StandardAllocator::Global()->allocate<math::Vector>(vertexCount, 16);

	if (normalCount > 0) {
		m_normals = StandardAllocator::Global()->allocate<math::Vector>(normalCount, 16);
	}

	if (texCoordCount > 0) {
		m_textureCoords = StandardAllocator::Global()->allocate<math::Vector>(texCoordCount, 16);
	}

	m_triangleFaceCount = faceCount;
	m_quadFaceCount = 0;
	m_vertexCount = vertexCount;
	m_normalCount = normalCount;
	m_texCoordCount = texCoordCount;
}

void manta::Mesh::destroy() {
	if (m_faces != nullptr) StandardAllocator::Global()->free(m_faces, m_triangleFaceCount);
	if (m_auxFaceData != nullptr) StandardAllocator::Global()->free(m_auxFaceData, m_triangleFaceCount);
	if (m_quadFaces != nullptr) StandardAllocator::Global()->free(m_quadFaces, m_quadFaceCount);
	if (m_auxQuadFaceData != nullptr) StandardAllocator::Global()->free(m_auxQuadFaceData, m_quadFaceCount);
	if (m_vertices != nullptr) StandardAllocator::Global()->aligned_free(m_vertices, m_vertexCount);
	if (m_normals != nullptr) StandardAllocator::Global()->aligned_free(m_normals, m_normalCount);
	if (m_textureCoords != nullptr) StandardAllocator::Global()->aligned_free(m_textureCoords, m_texCoordCount);
	if (m_precomputedValues != nullptr) StandardAllocator::Global()->aligned_free(m_precomputedValues, m_triangleFaceCount);

	m_faces = nullptr;
	m_auxFaceData = nullptr;
	m_quadFaces = nullptr;
	m_auxQuadFaceData = nullptr;
	m_vertices = nullptr;
	m_normals = nullptr;
	m_textureCoords = nullptr;
	m_precomputedValues = nullptr;
}

void manta::Mesh::filterDegenerateFaces() {
	// Delete faces that are too small
	int actualFaceCount = m_triangleFaceCount;
	for (int i = 0; i < actualFaceCount; i++) {
		math::Vector u = m_vertices[m_faces[i].u];
		math::Vector v = m_vertices[m_faces[i].v];
		math::Vector w = m_vertices[m_faces[i].w];

		math::Vector normal = math::cross(math::sub(v, u), math::sub(w, u));
		if (abs(math::getX(normal)) < 1E-9 && abs(math::getY(normal)) < 1E-9 && abs(math::getZ(normal)) < 1E-9) {
			m_faces[i] = m_faces[actualFaceCount - 1];
			m_auxFaceData[i] = m_auxFaceData[actualFaceCount - 1];
			actualFaceCount--;
			i--;
		}
	}

	Face *newFaces = StandardAllocator::Global()->allocate<Face>(actualFaceCount);
	AuxFaceData *newAuxFaceData = StandardAllocator::Global()->allocate<AuxFaceData>(actualFaceCount);
	for (int i = 0; i < actualFaceCount; i++) {
		newFaces[i] = m_faces[i];
		newAuxFaceData[i] = m_auxFaceData[i];
	}

	StandardAllocator::Global()->free(m_faces, m_triangleFaceCount);
	StandardAllocator::Global()->free(m_auxFaceData, m_triangleFaceCount);

	m_faces = newFaces;
	m_auxFaceData = newAuxFaceData;
	m_triangleFaceCount = actualFaceCount;
}

void manta::Mesh::findQuads() {
	struct NewQuad {
		int face1;
		int face2;

		// Start and end vertices that defined the edge
		// between the two merged faces
		int e0;
		int e1;
		int e0p;
		int e1p;
	};

	// First generate adjacency matrix
	std::vector<std::vector<int>> adj;
	std::vector<NewQuad> newQuads;

	int originalTriangleCount = m_triangleFaceCount;
	bool *usedFlags = StandardAllocator::Global()->allocate<bool>(m_triangleFaceCount);

	for (int i = 0; i < m_vertexCount; i++) {
		adj.push_back(std::vector<int>());
	}

	for (int i = 0; i < m_triangleFaceCount; i++) {
		Face *face = &m_faces[i];
		
		adj[face->u].push_back(i);
		adj[face->v].push_back(i);
		adj[face->w].push_back(i);

		// Set flags
		usedFlags[i] = false;
	}

	// Find all coplanar triangles
	for (int i = 0; i < m_vertexCount; i++) {
		std::vector<int> &connectedFaces = adj[i];
		for (int f1 = 0; f1 < connectedFaces.size(); f1++) {
			if (usedFlags[connectedFaces[f1]]) continue;
			for (int f2 = f1 + 1; f2 < connectedFaces.size(); f2++) {
				if (usedFlags[connectedFaces[f2]]) continue;

				Face *face1 = &m_faces[connectedFaces[f1]];
				Face *face2 = &m_faces[connectedFaces[f2]];

				int e0, e1, e0p, e1p;
				int iv0, iv1;
				bool attached = face1->isAttached(*face2, &e0, &e1, &e0p, &e1p);

				if (attached) {
					// Check if the normals align
					math::Vector u1 = m_vertices[face1->indices[0]];
					math::Vector v1 = m_vertices[face1->indices[1]];
					math::Vector w1 = m_vertices[face1->indices[2]];

					math::Vector u2 = m_vertices[face2->indices[0]];
					math::Vector v2 = m_vertices[face2->indices[1]];
					math::Vector w2 = m_vertices[face2->indices[2]];

					math::Vector n1 = math::normalize(math::cross(math::sub(v1, u1), math::sub(w1, u1)));
					math::Vector n2 = math::normalize(math::cross(math::sub(v2, u2), math::sub(w2, u2)));

					math::real d = ::abs((math::real)1.0 - math::getScalar(math::dot(n1, n2)));

					// Faces are nearly coplanar
					if (d < 1E-5) {
						// Check handedness (ensure that the first vertex appears on the 'left' of the edge)
						iv0 = face1->getOther(e0, e1);

						math::Vector ev0 = m_vertices[face1->indices[e0]];
						math::Vector ev1 = m_vertices[face1->indices[e1]];
						math::Vector a0 = m_vertices[face1->indices[iv0]];

						// Compute the edge function
						math::real e = math::getScalar(math::dot(math::cross(math::sub(ev1, ev0), math::sub(a0, ev0)), n1));

						NewQuad newQuad;

						if (e > 0) {
							newQuad.face1 = connectedFaces[f1];
							newQuad.face2 = connectedFaces[f2];

							newQuad.e0 = e0;
							newQuad.e1 = e1;
							newQuad.e0p = e0p;
							newQuad.e1p = e1p;
							newQuads.push_back(newQuad);
						}
						else {
							newQuad.face1 = connectedFaces[f2];
							newQuad.face2 = connectedFaces[f1];

							newQuad.e0 = e0p;
							newQuad.e1 = e1p;
							newQuad.e0p = e0;
							newQuad.e1p = e1;
							newQuads.push_back(newQuad);
						}

						usedFlags[connectedFaces[f1]] = true;
						usedFlags[connectedFaces[f2]] = true;
					}
				}
			}
		}
	}

	// Generate new quadfaces
	m_quadFaceCount = (int)newQuads.size();

	m_quadFaces = StandardAllocator::Global()->allocate<QuadFace>(m_quadFaceCount);
	m_auxQuadFaceData = StandardAllocator::Global()->allocate<QuadAuxFaceData>(m_quadFaceCount);

	for (int i = 0; i < m_quadFaceCount; i++) {
		NewQuad &w = newQuads[i];

		Face &f1 = m_faces[w.face1];
		Face &f2 = m_faces[w.face2];

		AuxFaceData &af1 = m_auxFaceData[w.face1];
		AuxFaceData &af2 = m_auxFaceData[w.face2];

		QuadFace newQuad;
		QuadAuxFaceData auxData;

		// Generate new quad while preserving winding order
		int v1 = f1.getOther(w.e0, w.e1);
		int v2 = f2.getOther(w.e0p, w.e1p);
		
		newQuad.u = f1.indices[v1];
		newQuad.v = f1.indices[(v1 + 1) % 3];
		newQuad.w = f1.indices[(v1 + 2) % 3];
		newQuad.r = f2.indices[v2];

		auxData.data[0] = af1.data[v1];
		auxData.data[1] = af1.data[(v1 + 1) % 3];
		auxData.data[2] = af1.data[(v1 + 2) % 3];
		auxData.data[3] = af2.data[v2];
		auxData.materials[0] = af1.material;
		auxData.materials[1] = af2.material;

		m_quadFaces[i] = newQuad;
		m_auxQuadFaceData[i] = auxData;
	}
	
	// Update triangle list
	std::vector<Face> newTrianglesTemp;
	std::vector<AuxFaceData> newAuxFaceDataTemp;
	for (int i = 0; i < m_triangleFaceCount; i++) {
		if (!usedFlags[i]) {
			newTrianglesTemp.push_back(m_faces[i]);
			newAuxFaceDataTemp.push_back(m_auxFaceData[i]);
		}
	}

	m_triangleFaceCount = (int)newTrianglesTemp.size();
	Face *newFaces = StandardAllocator::Global()->allocate<Face>(m_triangleFaceCount);
	AuxFaceData *newAuxData = StandardAllocator::Global()->allocate<AuxFaceData>(m_triangleFaceCount);

	for (int i = 0; i < m_triangleFaceCount; i++) {
		newFaces[i] = newTrianglesTemp[i];
		newAuxData[i] = newAuxFaceDataTemp[i];
	}

	StandardAllocator::Global()->free(m_faces, m_triangleFaceCount);
	StandardAllocator::Global()->free(m_auxFaceData, m_triangleFaceCount);

	m_faces = newFaces;
	m_auxFaceData = newAuxData;

	// Clean up temporary memory
	StandardAllocator::Global()->free(usedFlags, originalTriangleCount);
}

void manta::Mesh::precomputeValues() {
	if (m_precomputedValues != nullptr) StandardAllocator::Global()->aligned_free(m_precomputedValues, m_triangleFaceCount);

	m_precomputedValues = StandardAllocator::Global()->allocate<PrecomputedValues>(m_triangleFaceCount, 16);

	for (int i = 0; i < m_triangleFaceCount; i++) {
		PrecomputedValues *cache = &m_precomputedValues[i];
		math::Vector u = m_vertices[m_faces[i].u];
		math::Vector v = m_vertices[m_faces[i].v];
		math::Vector w = m_vertices[m_faces[i].w];

		math::Vector normal = math::cross(math::sub(v, u), math::sub(w, u));

		cache->normal = math::normalize(normal);

		computePlane(math::cross(normal, math::sub(w, v)), v, &cache->edgePlaneVW);
		computePlane(math::cross(normal, math::sub(u, w)), w, &cache->edgePlaneWU);
		computePlane(math::cross(normal, math::sub(v, u)), u, &cache->edgePlaneVU);

		// Scale the planes such that the computed barycentric coordinates are correct
		math::Vector scaleVW = math::loadScalar(
			math::getScalar(math::dot(u, cache->edgePlaneVW.normal)) - cache->edgePlaneVW.d);
		math::Vector scaleWU = math::loadScalar(
			math::getScalar(math::dot(v, cache->edgePlaneWU.normal)) - cache->edgePlaneWU.d);

		cache->edgePlaneVW.normal = math::div(cache->edgePlaneVW.normal, scaleVW);
		cache->edgePlaneVW.d /= math::getScalar(scaleVW);

		cache->edgePlaneWU.normal = math::div(cache->edgePlaneWU.normal, scaleWU);
		cache->edgePlaneWU.d /= math::getScalar(scaleWU);

		cache->scaleWU = (math::real)1.0 / math::getScalar(scaleWU);
		cache->scaleVW = (math::real)1.0 / math::getScalar(scaleVW);

		cache->p0 = u;
	}
}

bool manta::Mesh::findClosestIntersection(const LightRay *ray, CoarseIntersection *intersection, math::real minDepth, math::real maxDepth, StackAllocator *s) const {
	math::Vector rayDir = ray->getDirection();
	math::Vector raySource = ray->getSource();

	CoarseCollisionOutput output;
	math::real currentMaxDepth = maxDepth;
	bool found = false;
	for (int i = 0; i < m_triangleFaceCount; i++) {
		if (detectTriangleIntersection(i, minDepth, currentMaxDepth, ray, &output)) {
			intersection->depth = output.depth;
			intersection->faceHint = i; // Face index
			intersection->sceneGeometry = this;
			intersection->subdivisionHint = -1; // Not used for triangles

			intersection->su = output.u;
			intersection->sv = output.v;
			intersection->sw = output.w;

			currentMaxDepth = output.depth;
			found = true;
		}
	}

	for (int i = 0; i < m_quadFaceCount; i++) {
		if (detectQuadIntersection(i, minDepth, currentMaxDepth, ray, &output)) {
			intersection->depth = output.depth;
			intersection->faceHint = i; // Face index
			intersection->subdivisionHint = output.subdivisionHint;
			intersection->sceneGeometry = this;

			intersection->su = output.u;
			intersection->sv = output.v;
			intersection->sw = output.w;

			currentMaxDepth = output.depth;
			found = true;
		}
	}

	return found;
}

manta::math::Vector manta::Mesh::getClosestPoint(const CoarseIntersection *hint, const math::Vector &p) const {
	return getClosestPointOnFace(hint->faceHint, p);
}

void manta::Mesh::getVicinity(const math::Vector &p, math::real radius, IntersectionList *list, SceneObject *object) const {
	/* Deprecated */
}

void manta::Mesh::fineIntersection(const math::Vector &r, IntersectionPoint *p, const CoarseIntersection *hint) const {
	int faceIndex = hint->faceHint;
	//PrecomputedValues &cache = m_precomputedValues[faceIndex];
	math::real u, v, w; // Barycentric coordinates
	u = hint->su;
	v = hint->sv;
	w = hint->sw;

	math::Vector *vertices[3];
	AuxData *data[3];

	short int material;

	// Decide which vertices to interpolate between
	if (faceIndex >= m_triangleFaceCount) {
		// Face is a quad
		QuadFace *face = &m_quadFaces[faceIndex - m_triangleFaceCount];
		QuadAuxFaceData *auxData = &m_auxQuadFaceData[faceIndex - m_triangleFaceCount];

		int subHint = hint->subdivisionHint;
		if (subHint == 0) {
			vertices[0] = &m_vertices[face->u];
			vertices[1] = &m_vertices[face->v];
			vertices[2] = &m_vertices[face->w];

			data[0] = &auxData->data[0];
			data[1] = &auxData->data[1];
			data[2] = &auxData->data[2];

			//math::real temp = w;
			//w = u;
			//u = temp;

			material = auxData->materials[0];
		}
		else {
			// Swap 2nd and 3rd vertices to preserve winding
			vertices[0] = &m_vertices[face->v];
			vertices[2] = &m_vertices[face->w];
			vertices[1] = &m_vertices[face->r];

			data[0] = &auxData->data[1];
			data[2] = &auxData->data[2];
			data[1] = &auxData->data[3];

			math::real temp = w;
			w = v;
			v = temp;

			material = auxData->materials[1];
		}
	}
	else {
		// Face is a triangle
		Face *face = &m_faces[faceIndex];
		AuxFaceData *auxData = &m_auxFaceData[faceIndex];

		vertices[0] = &m_vertices[face->u];
		vertices[1] = &m_vertices[face->v];
		vertices[2] = &m_vertices[face->w];

		data[0] = &auxData->data[0];
		data[1] = &auxData->data[1];
		data[2] = &auxData->data[2];

		material = auxData->material;
	}

	/*
	getClosestPointOnFaceBarycentric(hint->faceHint, r, &u, &v, &w);

	if (math::getX(r) == 0.0 && math::getY(r) == 0.0 && math::getZ(r) == 0.0) {
		int a = 0;

	}

	if (std::isnan(u)) {
		getClosestPointOnFaceBarycentric(hint->faceHint, r, &u, &v, &w);
		std::cout << "NAN" << std::endl;
	}

	assert(!std::isnan(u));
	assert(!std::isnan(v));
	assert(!std::isnan(w));
	*/

	p->m_depth = hint->depth;

	math::Vector vertexNormal;
	math::Vector textureCoordinates;
	const AuxFaceData &auxData = m_auxFaceData[faceIndex];

	math::Vector faceNormal = math::normalize(math::cross(math::sub(*vertices[1], *vertices[0]), math::sub(*vertices[2], *vertices[0])));

	if (m_perVertexNormals) {
		math::Vector normalU = m_normals[data[0]->n];
		math::Vector normalV = m_normals[data[1]->n];
		math::Vector normalW = m_normals[data[2]->n];

		vertexNormal = math::add(math::mul(normalU, math::loadScalar(u)), math::mul(normalV, math::loadScalar(v)));
		vertexNormal = math::add(vertexNormal, math::mul(normalW, math::loadScalar(w)));
		vertexNormal = math::normalize(vertexNormal);
	}
	else {
		vertexNormal = faceNormal;
	}

	if (m_useTextureCoords) {
		math::Vector texU = m_textureCoords[data[0]->t];
		math::Vector texV = m_textureCoords[data[1]->t];
		math::Vector texW = m_textureCoords[data[2]->t];

		textureCoordinates = math::add(math::mul(texU, math::loadScalar(u)), math::mul(texV, math::loadScalar(v)));
		textureCoordinates = math::add(textureCoordinates, math::mul(texW, math::loadScalar(w)));
	}
	else {
		textureCoordinates = math::constants::Zero;
	}

	p->m_vertexNormal = vertexNormal;
	p->m_faceNormal = faceNormal;
	p->m_position = r;
	p->m_textureCoodinates = textureCoordinates;
	p->m_material = material;
}

bool manta::Mesh::fastIntersection(const LightRay *ray) const {
	if (m_fastIntersectEnabled) {
		math::Vector d_pos = math::sub(ray->getSource(), m_fastIntersectPosition);
		math::Vector d_dot_dir = math::dot(d_pos, ray->getDirection());
		math::Vector mag2 = math::magnitudeSquared3(d_pos);

		math::Vector radius2 = math::loadScalar(m_fastIntersectRadius * m_fastIntersectRadius);
		math::Vector det = math::sub(math::mul(d_dot_dir, d_dot_dir), math::sub(mag2, radius2));

		if (math::getScalar(det) < (math::real)0.0) {
			return false;
		}
		else {
			det = math::sqrt(det);
			math::Vector t1 = math::sub(det, d_dot_dir);
			math::Vector t2 = math::sub(math::negate(det), d_dot_dir);

			math::real t1_s = math::getScalar(t1);
			math::real t2_s = math::getScalar(t2);

			return t1_s > (math::real)0.0 || t2_s > (math::real)0.0;
		}
	}
	else return true;
}

void manta::Mesh::loadObjFileData(ObjFileLoader *data, MaterialManager *materialLibrary, int defaultMaterialIndex, unsigned int globalId) {
	initialize(data->getFaceCount(), data->getVertexCount(), data->getNormalCount(), data->getTexCoordCount());

	for (unsigned int i = 0; i < data->getFaceCount(); i++) {
		ObjFace *face = data->getFace(i);
		m_faces[i].u = face->v1 - 1;
		m_faces[i].v = face->v2 - 1;
		m_faces[i].w = face->v3 - 1;

		m_auxFaceData[i].data[0].n = face->vn1 - 1;
		m_auxFaceData[i].data[1].n = face->vn2 - 1;
		m_auxFaceData[i].data[2].n = face->vn3 - 1;

		m_auxFaceData[i].data[0].t = face->vt1 - 1;
		m_auxFaceData[i].data[1].t = face->vt2 - 1;
		m_auxFaceData[i].data[2].t = face->vt3 - 1;

		// Resolve the material reference
		if (face->material == nullptr) {
			m_auxFaceData[i].material = defaultMaterialIndex;
		}
		else {
			if (materialLibrary != nullptr) {
				Material *material = materialLibrary->searchByName(face->material->name);
				if (material != nullptr) {
					m_auxFaceData[i].material = material->getIndex();
				}
				else {
					// TODO: raise an error or notification if this happens
					m_auxFaceData[i].material = defaultMaterialIndex;
				}
			}
			else {
				m_auxFaceData[i].material = defaultMaterialIndex;
			}
		}
	}

	for (unsigned int i = 0; i < data->getVertexCount(); i++) {
		math::Vector3 *v = data->getVertex(i);
		m_vertices[i] = math::loadVector(*v);
	}

	if (data->getNormalCount() > 0) {
		for (unsigned int i = 0; i < data->getNormalCount(); i++) {
			math::Vector3 *n = data->getNormal(i);
			m_normals[i] = math::loadVector(*n);
		}
		m_perVertexNormals = true;
	}
	else {
		m_perVertexNormals = false;
	}

	if (data->getTexCoordCount() > 0) {
		for (unsigned int i = 0; i < data->getTexCoordCount(); i++) {
			math::Vector2 *t = data->getTexCoords(i);
			m_textureCoords[i] = math::loadVector(*t);
		}
		m_useTextureCoords = true;
	}
	else {
		m_useTextureCoords = false;
	}

	precomputeValues();
}

void manta::Mesh::merge(const Mesh *mesh) {
	int newFaceCount = m_triangleFaceCount + mesh->getTriangleFaceCount();
	int newVertexCount = m_vertexCount + mesh->getVertexCount();
	int newNormalCount = m_normalCount + mesh->getNormalCount();
	int newTexCoordCount = m_texCoordCount + mesh->getTexCoordCount();

	Face *newFaces = nullptr;
	AuxFaceData *newAuxFaceData = nullptr;
	math::Vector *newVerts = nullptr;
	math::Vector *newNormals = nullptr;
	math::Vector *newTexCoords = nullptr;

	if (newFaceCount > 0) {
		newFaces = StandardAllocator::Global()->allocate<Face>(newFaceCount);
		newAuxFaceData = StandardAllocator::Global()->allocate<AuxFaceData>(newFaceCount);
	}

	if (newVertexCount > 0) newVerts = StandardAllocator::Global()->allocate<math::Vector>(newVertexCount, 16);
	if (newNormalCount > 0) newNormals = StandardAllocator::Global()->allocate<math::Vector>(newNormalCount, 16);
	if (newTexCoordCount > 0) newTexCoords = StandardAllocator::Global()->allocate<math::Vector>(newTexCoordCount, 16);

	if (newFaceCount > 0) {
		memcpy((void *)newFaces, (void *)m_faces, sizeof(Face) * m_triangleFaceCount);
		memcpy((void *)newAuxFaceData, (void *)m_auxFaceData, sizeof(AuxFaceData) * m_triangleFaceCount);
	}
	if (newVertexCount > 0) memcpy((void *)newVerts, (void *)m_vertices, sizeof(math::Vector) * m_vertexCount);
	if (newNormalCount > 0) memcpy((void *)newNormals, (void *)m_normals, sizeof(math::Vector) * m_normalCount);
	if (newTexCoordCount > 0) memcpy((void *)newTexCoords, (void *)m_textureCoords, sizeof(math::Vector) * m_texCoordCount);

	for (int i = 0; i < mesh->getFaceCount(); i++) {
		Face &newFace = newFaces[i + m_triangleFaceCount];
		AuxFaceData &auxData = newAuxFaceData[i + m_triangleFaceCount];
		const Face *mergeFace = mesh->getFace(i);
		const AuxFaceData *mergeAuxData = mesh->getAuxFace(i);

		auxData.material = mergeAuxData->material;

		if (newFaceCount > 0) {
			newFace.u = mergeFace->u + m_vertexCount;
			newFace.v = mergeFace->v + m_vertexCount;
			newFace.w = mergeFace->w + m_vertexCount;
		}

		if (newNormalCount > 0) {
			auxData.data[0].n = mergeAuxData->data[0].n + m_normalCount;
			auxData.data[1].n = mergeAuxData->data[1].n + m_normalCount;
			auxData.data[2].n = mergeAuxData->data[2].n + m_normalCount;
		}

		if (newTexCoordCount > 0) {
			auxData.data[0].t = mergeAuxData->data[0].t + m_texCoordCount;
			auxData.data[1].t = mergeAuxData->data[1].t + m_texCoordCount;
			auxData.data[2].t = mergeAuxData->data[2].t + m_texCoordCount;
		}
	}

	for (int i = 0; i < mesh->getVertexCount(); i++) {
		newVerts[i + m_vertexCount] = *mesh->getVertex(i);
	}

	for (int i = 0; i < mesh->getNormalCount(); i++) {
		newNormals[i + m_normalCount] = *mesh->getNormal(i);
	}

	for (int i = 0; i < mesh->getTexCoordCount(); i++) {
		newTexCoords[i + m_texCoordCount] = *mesh->getTexCoord(i);
	}

	if (m_faces != nullptr) StandardAllocator::Global()->free(m_faces, m_triangleFaceCount);
	if (m_auxFaceData != nullptr) StandardAllocator::Global()->free(m_auxFaceData, m_triangleFaceCount);
	if (m_vertices != nullptr) StandardAllocator::Global()->aligned_free(m_vertices, m_vertexCount);
	if (m_normals != nullptr) StandardAllocator::Global()->aligned_free(m_normals, m_normalCount);
	if (m_textureCoords != nullptr) StandardAllocator::Global()->aligned_free(m_textureCoords, m_texCoordCount);
	if (m_precomputedValues != nullptr) StandardAllocator::Global()->aligned_free(m_precomputedValues, m_triangleFaceCount);

	m_faces = newFaces;
	m_auxFaceData = newAuxFaceData;
	m_vertices = newVerts;
	m_normals = newNormals;
	m_textureCoords = newTexCoords;

	m_triangleFaceCount = newFaceCount;
	m_vertexCount = newVertexCount;
	m_normalCount = newNormalCount;
	m_texCoordCount = newTexCoordCount;
	m_precomputedValues = nullptr;

	precomputeValues();
}

manta::math::Vector manta::Mesh::getClosestPointOnFace(int faceIndex, const math::Vector &p) const {
	PrecomputedValues &cache = m_precomputedValues[faceIndex];

	math::Vector a = m_vertices[m_faces[faceIndex].u];
	math::Vector b = m_vertices[m_faces[faceIndex].v];
	math::Vector c = m_vertices[m_faces[faceIndex].w];

	math::Vector ab = math::sub(b, a);
	math::Vector ac = math::sub(c, a);
	math::Vector ap = math::sub(p, a);

	// P in vertex region outside A
	math::real d1 = math::getScalar(math::dot(ab, ap));
	math::real d2 = math::getScalar(math::dot(ac, ap));
	if (d1 <= (math::real)0.0 && d2 <= 0.0) return a; // Barycentric coordinates: (1, 0, 0)

	// P in vertex region outside B
	math::Vector bp = math::sub(p, b);
	math::real d3 = math::getScalar(math::dot(ab, bp));
	math::real d4 = math::getScalar(math::dot(ac, bp));
	if (d3 >= (math::real)0.0 && d4 <= d3) return b; // Barycentric coordinates: (0, 1, 0)

	// P in edge region of AB
	math::real vc = d1 * d4 - d3 * d2;
	if (vc <= (math::real)0.0 && d1 >= (math::real)0.0 && d3 <= (math::real)0.0) {
		math::real v = d1 / (d1 - d3);
		return math::add(a, math::mul(math::loadScalar(v), ab));
	}

	// P in vertex region outside C
	math::Vector cp = math::sub(p, c);
	math::real d5 = math::getScalar(math::dot(ab, cp));
	math::real d6 = math::getScalar(math::dot(ac, cp));
	if (d6 >= (math::real)0.0 && d5 <= d6) return c;

	// P in edge region AC
	math::real vb = d5 * d2 - d1 * d6;
	if (vb <= (math::real)0.0 && d2 >= (math::real)0.0 && d6 <= (math::real)0.0) {
		math::real w = d2 / (d2 - d6);
		return math::add(a, math::mul(math::loadScalar(w), ac)); // Barycentric coordinates (1 - w, 0, w)
	}

	// P in edge region of BC
	math::real va = d3 * d6 - d5 * d4;
	if (va <= (math::real)0.0 && (d4 - d3) >= (math::real)0.0 && (d5 - d6) >= (math::real)0.0) {
		math::real w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		return math::add(b, math::mul(math::loadScalar(w), math::sub(c, b))); // Barycentric coordinates (0, 1 - w, w)
	}

	// P inside face region
	math::real denom = (math::real)1.0 / (va + vb + vc);
	math::real v = vb * denom;
	math::real w = vc * denom;
	return
		math::add(
			math::add(
				a,
				math::mul(ab, math::loadScalar(v))),
			math::mul(math::loadScalar(w), ac));
}

void manta::Mesh::getClosestPointOnFaceBarycentric(int faceIndex, const math::Vector &p, math::real *bu, math::real *bv, math::real *bw) const {
	PrecomputedValues &cache = m_precomputedValues[faceIndex];

	math::Vector a = m_vertices[m_faces[faceIndex].u];
	math::Vector b = m_vertices[m_faces[faceIndex].v];
	math::Vector c = m_vertices[m_faces[faceIndex].w];

	math::Vector ab = math::sub(b, a);
	math::Vector ac = math::sub(c, a);
	math::Vector ap = math::sub(p, a);

	// P in vertex region outside A
	math::real d1 = math::getScalar(math::dot(ab, ap));
	math::real d2 = math::getScalar(math::dot(ac, ap));
	if (d1 <= (math::real)0.0 && d2 <= 0.0) {
		// Barycentric coordinates: (1, 0, 0)
		*bu = (math::real)1.0;
		*bv = (math::real)0.0;
		*bw = (math::real)0.0;
		return;
	}

	// P in vertex region outside B
	math::Vector bp = math::sub(p, b);
	math::real d3 = math::getScalar(math::dot(ab, bp));
	math::real d4 = math::getScalar(math::dot(ac, bp));
	if (d3 >= (math::real)0.0 && d4 <= d3) {
		// Barycentric coordinates: (0, 1, 0)
		*bu = (math::real)0.0;
		*bv = (math::real)1.0;
		*bw = (math::real)0.0;
		return;
	}

	// P in edge region of AB
	math::real vc = d1 * d4 - d3 * d2;
	if (vc <= (math::real)0.0 && d1 >= (math::real)0.0 && d3 <= (math::real)0.0) {
		math::real v = d1 / (d1 - d3);
		*bu = (math::real)1.0 - v;
		*bv = v;
		*bw = (math::real)0.0;
		return;
	}

	// P in vertex region outside C
	math::Vector cp = math::sub(p, c);
	math::real d5 = math::getScalar(math::dot(ab, cp));
	math::real d6 = math::getScalar(math::dot(ac, cp));
	if (d6 >= (math::real)0.0 && d5 <= d6) {
		*bu = (math::real)0.0;
		*bv = (math::real)0.0;
		*bw = (math::real)1.0;
		return;
	}

	// P in edge region AC
	math::real vb = d5 * d2 - d1 * d6;
	if (vb <= (math::real)0.0 && d2 >= (math::real)0.0 && d6 <= (math::real)0.0) {
		math::real w = d2 / (d2 - d6);
		// Barycentric coordinates (1 - w, 0, w)
		*bu = (math::real)1.0 - w;
		*bv = (math::real)0.0;
		*bw = w;
		return;
	}

	// P in edge region of BC
	math::real va = d3 * d6 - d5 * d4;
	if (va <= (math::real)0.0 && (d4 - d3) >= (math::real)0.0 && (d5 - d6) >= (math::real)0.0) {
		math::real w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		// Barycentric coordinates (0, 1 - w, w)
		*bu = (math::real)0.0;
		*bv = (math::real)1.0 - w;
		*bw = w;
		return;
	}

	// P inside face region
	math::real denom = (math::real)1.0 / (va + vb + vc);
	math::real v = vb * denom;
	math::real w = vc * denom;
	
	*bu = (math::real)1.0 - v - w;
	*bv = v;
	*bw = w;
}

bool manta::Mesh::testClosestPointOnFace(int faceIndex, math::real maxDepth, const math::Vector &p) const {
	PrecomputedValues &cache = m_precomputedValues[faceIndex];

	math::Vector denom = math::dot(cache.normal, cache.normal);

	math::Vector p0r0 = math::sub(p, cache.p0);
	math::Vector d = math::div(math::dot(p0r0, cache.normal), math::negate(denom));

	math::real depth_s = math::getScalar(d);

	if (abs(depth_s) > maxDepth) return false;

	math::Vector closestPoint = getClosestPointOnFace(faceIndex, p);
	math::Vector cp = math::sub(p, closestPoint);

	if (math::getScalar(math::magnitudeSquared3(cp)) > maxDepth * maxDepth) {
		return false;
	}

	return true;
}

bool manta::Mesh::detectTriangleIntersection(int faceIndex, math::real minDepth, math::real maxDepth, const LightRay *ray, CoarseCollisionOutput *output) const {
	Face &face = m_faces[faceIndex];

	math::Vector v0, v1, v2;
	v0 = m_vertices[face.u];
	v1 = m_vertices[face.v];
	v2 = m_vertices[face.w];

	math::Vector rayOrigin = ray->getSource();
	math::Vector p0t = math::sub(v0, rayOrigin);
	math::Vector p1t = math::sub(v1, rayOrigin);
	math::Vector p2t = math::sub(v2, rayOrigin);

	int kz = ray->getKZ();
	int kx = ray->getKX();
	int ky = ray->getKY();
	p0t = math::permute(p0t, kx, ky, kz);
	p1t = math::permute(p1t, kx, ky, kz);
	p2t = math::permute(p2t, kx, ky, kz);

	const math::Vector3 &shear = ray->getShear();
	math::real sx = shear.x;
	math::real sy = shear.y;
	math::real sz = shear.z;

	math::Vector s = math::loadVector(sx, sy);
	math::Vector p0t_z = math::loadScalar(math::getZ(p0t));
	math::Vector p1t_z = math::loadScalar(math::getZ(p1t));
	math::Vector p2t_z = math::loadScalar(math::getZ(p2t));

	p0t = math::add(p0t, math::mul(s, p0t_z));
	p1t = math::add(p1t, math::mul(s, p1t_z));
	p2t = math::add(p2t, math::mul(s, p2t_z));

	math::real e0 = math::getX(p1t) * math::getY(p2t) - math::getY(p1t) * math::getX(p2t);
	math::real e1 = math::getX(p2t) * math::getY(p0t) - math::getY(p2t) * math::getX(p0t);
	math::real e2 = math::getX(p0t) * math::getY(p1t) - math::getY(p0t) * math::getX(p1t);

	if ((e0 < 0 || e1 < 0 || e2 < 0) && (e0 > 0 || e1 > 0 || e2 > 0)) return false;

	math::real det = e0 + e1 + e2;
	if (det == (math::real)0.0) {
		return false;
	}

	// Compute distance
	math::real p0t_sz = math::getZ(p0t) * sz;
	math::real p1t_sz = math::getZ(p1t) * sz;
	math::real p2t_sz = math::getZ(p2t) * sz;

	math::real t_scaled = math::getScalar(math::dot(math::loadVector(e0, e1, e2), math::loadVector(p0t_sz, p1t_sz, p2t_sz)));

	if (det < 0 && (t_scaled >= 0 || t_scaled < maxDepth * det)) return false;
	else if (det > 0 && (t_scaled <= 0 || t_scaled > maxDepth *det)) return false;

	math::real invDet = 1 / det;
	output->depth = t_scaled * invDet;
	output->u = e0 * invDet;
	output->v = e1 * invDet;
	output->w = e2 * invDet;

	return true;
}

bool manta::Mesh::detectQuadIntersection(int faceIndex, math::real minDepth, math::real maxDepth, const LightRay *ray, CoarseCollisionOutput *output) const {
	QuadFace &face = m_quadFaces[faceIndex];

	int kx = ray->getKX();
	int ky = ray->getKY();
	int kz = ray->getKZ();

	// First compute the edge function at the meeting edge of the two constituent triangles
	/*math::Vector v0, v1;
	if (kx != 2 || ky != 0 || kz != 1) {
		v1 = m_vertices[face.v];
		v0 = m_vertices[face.w];
	}
	else {
		v0 = m_vertices[face.v];
		v1 = m_vertices[face.w];
	}*/

	math::Vector v0, v1, vu;
	v0 = m_vertices[face.v];
	v1 = m_vertices[face.w];
	vu = m_vertices[face.u];

	math::Vector rayOrigin = ray->getSource();
	math::Vector pe0t = math::sub(v0, rayOrigin);
	math::Vector pe1t = math::sub(v1, rayOrigin);
	math::Vector put = math::sub(vu, rayOrigin);
	
	pe0t = math::permute(pe0t, kx, ky, kz);
	pe1t = math::permute(pe1t, kx, ky, kz);
	put = math::permute(put, kx, ky, kz);

	const math::Vector3 &shear = ray->getShear();
	math::real sx = shear.x;
	math::real sy = shear.y;
	math::real sz = shear.z;

	math::Vector s = math::loadVector(sx, sy);
	math::Vector p0t_z = math::loadScalar(math::getZ(pe0t));
	math::Vector p1t_z = math::loadScalar(math::getZ(pe1t));
	math::Vector put_z = math::loadScalar(math::getZ(put));

	pe0t = math::add(pe0t, math::mul(s, p0t_z));
	pe1t = math::add(pe1t, math::mul(s, p1t_z));
	put = math::add(put, math::mul(s, put_z));

	math::real e_half = math::getX(pe0t) * math::getY(pe1t) - math::getY(pe0t) * math::getX(pe1t);
	math::real e_u = math::getX(put) * math::getY(pe0t) - math::getY(put) * math::getX(pe0t);

	int subdivision;
	math::Vector p0t, p1t, p2t;
	math::real e0, e1, e2;

	if ((e_half > 0) == (e_u > 0)) {
		subdivision = 0;

		p0t = put;
		p1t = pe0t;
		p2t = pe1t;

		e0 = e_half;
		e1 = math::getX(p2t) * math::getY(p0t) - math::getY(p2t) * math::getX(p0t);
		e2 = e_u;
	}
	else {
		subdivision = 1;
		math::Vector v2 = m_vertices[face.r];

		p2t = math::sub(v2, rayOrigin);
		p2t = math::permute(p2t, kx, ky, kz);
		math::Vector p2t_z = math::loadScalar(math::getZ(p2t));
		p2t = math::add(p2t, math::mul(s, p2t_z));

		p0t = pe0t;
		p1t = pe1t;

		e0 = math::getX(p1t) * math::getY(p2t) - math::getY(p1t) * math::getX(p2t);
		e1 = math::getX(p2t) * math::getY(p0t) - math::getY(p2t) * math::getX(p0t);
		e2 = e_half;
	}

	//math::Vector p3t = math::sub(m_vertices[face.r], rayOrigin);
	//p3t = math::permute(p3t, kx, ky, kz);
	//math::Vector p3t_z = math::loadScalar(math::getZ(p3t));
	//p3t = math::add(p3t, math::mul(s, p3t_z));

	
	//math::real e1 = math::getX(p2t) * math::getY(p0t) - math::getY(p2t) * math::getX(p0t);
	

	if ((e0 < 0 || e1 < 0 || e2 < 0) && (e0 > 0 || e1 > 0 || e2 > 0)) return false;

	math::real det = e0 + e1 + e2;
	if (det == (math::real)0.0) {
		return false;
	}

	// Compute distance
	math::real p0t_sz = math::getZ(p0t) * sz;
	math::real p1t_sz = math::getZ(p1t) * sz;
	math::real p2t_sz = math::getZ(p2t) * sz;

	math::real t_scaled = math::getScalar(math::dot(math::loadVector(e0, e1, e2), math::loadVector(p0t_sz, p1t_sz, p2t_sz)));

	if (det < 0 && (t_scaled >= 0 || t_scaled < maxDepth * det)) return false;
	else if (det > 0 && (t_scaled <= 0 || t_scaled > maxDepth *det)) return false;

	math::real invDet = 1 / det;
	output->depth = t_scaled * invDet;
	output->u = e0 * invDet;
	output->v = e1 * invDet;
	output->w = e2 * invDet;
	output->subdivisionHint = subdivision;

	return true;
}

inline bool manta::Mesh::detectIntersection(int faceIndex, math::real u, math::real v, math::real w, math::real delta) const {
	PrecomputedValues &cache = m_precomputedValues[faceIndex];

	math::real ru = u - cache.edgePlaneVW.d;
	math::real rv = v - cache.edgePlaneWU.d;

	// Check quasi-barycentric components u, v, w
	if (ru < -delta || ru > ((math::real)1.0 + delta)) return false;
	if (rv < -delta) return false;
	if (1 - ru - rv < -delta) return false;

	return true;
}

bool manta::Mesh::findClosestIntersection(int *faceList, int faceCount, const LightRay *ray, CoarseIntersection *intersection, math::real minDepth, math::real maxDepth, StackAllocator *s) const {
	math::Vector rayDir = ray->getDirection();
	math::Vector raySource = ray->getSource();

	CoarseCollisionOutput output;
	math::real currentMaxDepth = maxDepth;
	bool found = false;
	for (int i = 0; i < faceCount; i++) {
		int face = faceList[i];
		if (face < m_triangleFaceCount) {
			// Face is a triangle
			if (detectTriangleIntersection(face, minDepth, currentMaxDepth, ray, &output)) {
				intersection->depth = output.depth;
				intersection->faceHint = faceList[i]; // Face index
				intersection->subdivisionHint = -1; // Not used for triangles
				intersection->sceneGeometry = this;

				intersection->su = output.u;
				intersection->sv = output.v;
				intersection->sw = output.w;

				currentMaxDepth = output.depth;
				found = true;
			}
		}
	}

	for (int i = 0; i < faceCount; i++) {
		int face = faceList[i];
		if (face >= m_triangleFaceCount) {
			// Face is a quad
			if (detectQuadIntersection(face - m_triangleFaceCount, minDepth, currentMaxDepth, ray, &output)) {
				intersection->depth = output.depth;
				intersection->faceHint = faceList[i]; // Face index
				intersection->subdivisionHint = output.subdivisionHint; // Tells which half of the quad the point lies on
				intersection->sceneGeometry = this;

				intersection->su = output.u;
				intersection->sv = output.v;
				intersection->sw = output.w;

				currentMaxDepth = output.depth;
				found = true;
			}
		}
	}

	return found;
}

void manta::Mesh::getVicinity(int *faceList, int faceCount, const math::Vector &p, math::real radius, IntersectionList *list, SceneObject *object) const {
	/* Deprecated */
}

void manta::Mesh::calculateFaceAABB(int faceIndex, AABB *target) const {
	if (faceIndex < m_triangleFaceCount) {
		const Face *face = &m_faces[faceIndex];
		math::Vector u = m_vertices[face->u];
		math::Vector v = m_vertices[face->v];
		math::Vector w = m_vertices[face->w];

		target->minPoint = u;
		target->maxPoint = u;

		target->minPoint = math::componentMin(target->minPoint, v);
		target->minPoint = math::componentMin(target->minPoint, w);

		target->maxPoint = math::componentMax(target->maxPoint, v);
		target->maxPoint = math::componentMax(target->maxPoint, w);
	}
	else {
		const QuadFace *face = &m_quadFaces[faceIndex - m_triangleFaceCount];
		math::Vector u = m_vertices[face->u];
		math::Vector v = m_vertices[face->v];
		math::Vector w = m_vertices[face->w];
		math::Vector r = m_vertices[face->r];

		target->minPoint = u;
		target->maxPoint = u;

		target->minPoint = math::componentMin(target->minPoint, v);
		target->minPoint = math::componentMin(target->minPoint, w);
		target->minPoint = math::componentMin(target->minPoint, r);

		target->maxPoint = math::componentMax(target->maxPoint, v);
		target->maxPoint = math::componentMax(target->maxPoint, w);
		target->maxPoint = math::componentMax(target->maxPoint, r);
	}
}

void manta::Mesh::computePlane(const math::Vector &n, const math::Vector &p, Plane *plane) const {
	plane->normal = math::normalize(n);
	plane->d = math::getScalar(math::dot(plane->normal, p));
}
