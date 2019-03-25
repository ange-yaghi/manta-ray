#include <mesh.h>

#include <light_ray.h>
#include <intersection_point.h>
#include <obj_file_loader.h>
#include <coarse_intersection.h>
#include <standard_allocator.h>
#include <material.h>
#include <material_manager.h>
#include <primitives.h>
#include <runtime_statistics.h>

manta::Mesh::Mesh() {
	m_faces = nullptr;
	m_auxFaceData = nullptr;
	m_quadFaces = nullptr;
	m_auxQuadFaceData = nullptr;
	m_vertices = nullptr;
	m_normals = nullptr;
	m_textureCoords = nullptr;
	m_faceBounds = nullptr;

	m_triangleFaceCount = 0;
	m_quadFaceCount = 0;
	m_vertexCount = 0;
	m_normalCount = 0;
	m_texCoordCount = 0;

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
	assert(m_faceBounds == nullptr);
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
	if (m_faceBounds != nullptr) StandardAllocator::Global()->free(m_faceBounds, m_triangleFaceCount);

	m_faces = nullptr;
	m_auxFaceData = nullptr;
	m_quadFaces = nullptr;
	m_auxQuadFaceData = nullptr;
	m_vertices = nullptr;
	m_normals = nullptr;
	m_textureCoords = nullptr;
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
						int iv0 = face1->getOther(e0, e1);

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

	StandardAllocator::Global()->free(m_faces, originalTriangleCount);
	StandardAllocator::Global()->free(m_auxFaceData, originalTriangleCount);

	m_faces = newFaces;
	m_auxFaceData = newAuxData;

	// Clean up temporary memory
	StandardAllocator::Global()->free(usedFlags, originalTriangleCount);
}

void manta::Mesh::computeBounds() {
	m_faceBounds = StandardAllocator::Global()->allocate<AABB>(m_triangleFaceCount);

	for (int i = 0; i < m_triangleFaceCount; i++) {
		int face = i;
		AABB *aabb = &m_faceBounds[i];
		calculateFaceAABB(face, aabb);
	}
}

bool manta::Mesh::findClosestIntersection(const LightRay *ray, CoarseIntersection *intersection, math::real minDepth, math::real maxDepth, StackAllocator *s /**/ STATISTICS_PROTOTYPE) const {
	math::Vector rayDir = ray->getDirection();
	math::Vector raySource = ray->getSource();

	CoarseCollisionOutput output;
	math::real currentMaxDepth = maxDepth;
	bool found = false;
	for (int i = 0; i < m_triangleFaceCount; i++) {
		INCREMENT_COUNTER(RuntimeStatistics::TRIANGLE_TESTS);
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
		INCREMENT_COUNTER(RuntimeStatistics::QUAD_TESTS);
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

void manta::Mesh::fineIntersection(const math::Vector &r, IntersectionPoint *p, const CoarseIntersection *hint) const {
	int faceIndex = hint->faceHint;

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

	p->m_depth = hint->depth;

	math::Vector vertexNormal;
	math::Vector textureCoordinates;
	const AuxFaceData &auxData = m_auxFaceData[faceIndex];

	math::Vector faceNormal = math::normalize(math::cross(math::sub(*vertices[1], *vertices[0]), math::sub(*vertices[2], *vertices[0])));

	if (m_perVertexNormals) {
		if (data[0]->n != -1 && data[1]->n != -1 && data[2]->n != -1) {
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
	}
	else {
		vertexNormal = faceNormal;
	}

	if (m_useTextureCoords) {
		if (data[0]->t != -1 && data[1]->t != -1 && data[2]->t != -1) {
			math::Vector texU = m_textureCoords[data[0]->t];
			math::Vector texV = m_textureCoords[data[1]->t];
			math::Vector texW = m_textureCoords[data[2]->t];

			textureCoordinates = math::add(math::mul(texU, math::loadScalar(u)), math::mul(texV, math::loadScalar(v)));
			textureCoordinates = math::add(textureCoordinates, math::mul(texW, math::loadScalar(w)));
		}
		else {
			textureCoordinates = math::constants::Zero;
		}
		
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

	filterDegenerateFaces();
	computeBounds();
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

	m_faces = newFaces;
	m_auxFaceData = newAuxFaceData;
	m_vertices = newVerts;
	m_normals = newNormals;
	m_textureCoords = newTexCoords;

	m_triangleFaceCount = newFaceCount;
	m_vertexCount = newVertexCount;
	m_normalCount = newNormalCount;
	m_texCoordCount = newTexCoordCount;
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

bool manta::Mesh::findClosestIntersection(int *faceList, int faceCount, const LightRay *ray, CoarseIntersection *intersection, math::real minDepth, math::real maxDepth, StackAllocator *s /**/ STATISTICS_PROTOTYPE) const {
	math::Vector rayDir = ray->getDirection();
	math::Vector raySource = ray->getSource();

	CoarseCollisionOutput output;
	math::real currentMaxDepth = maxDepth;
	bool found = false;
	for (int i = 0; i < faceCount; i++) {
		int face = faceList[i];
		if (face < m_triangleFaceCount) {
			// Face is a triangle
			AABB &aabb = m_faceBounds[face];

			INCREMENT_COUNTER(RuntimeStatistics::TOTAL_BV_TESTS);
			math::real tmin, tmax;
			if (aabb.rayIntersect(*ray, &tmin, &tmax)) {
				if (tmin > currentMaxDepth || tmax < minDepth) continue;

				INCREMENT_COUNTER(RuntimeStatistics::TRIANGLE_TESTS);
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
	}

	for (int i = 0; i < faceCount; i++) {
		int face = faceList[i];
		if (face >= m_triangleFaceCount) {
			// Face is a quad
			INCREMENT_COUNTER(RuntimeStatistics::QUAD_TESTS);
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

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

bool manta::Mesh::checkFaceAABB(int faceIndex, const AABB &bounds) const {
	if (faceIndex >= m_triangleFaceCount) {
		const QuadFace &face = m_quadFaces[faceIndex - m_triangleFaceCount];

		math::Vector v0 = m_vertices[face.u];
		math::Vector v1 = m_vertices[face.v];
		math::Vector v2 = m_vertices[face.w];
		math::Vector v3 = m_vertices[face.r];

		return checkFaceAABB(v0, v1, v2, bounds) || checkFaceAABB(v3, v1, v2, bounds);
	}
	else {
		const Face &face = m_faces[faceIndex];

		math::Vector v0 = m_vertices[face.u];
		math::Vector v1 = m_vertices[face.v];
		math::Vector v2 = m_vertices[face.w];

		return checkFaceAABB(v0, v1, v2, bounds);
	}
}

bool manta::Mesh::checkFaceAABB(const math::Vector &v0, const math::Vector &v1, const math::Vector &v2, const AABB &bounds) const {
	constexpr math::real EPSILON = (math::real)1E-4;

	math::real p0, p1, p2, r;

	math::Vector c = math::mul(math::add(bounds.maxPoint, bounds.minPoint), math::constants::Half);
	math::Vector extents = math::mul(math::sub(bounds.maxPoint, bounds.minPoint), math::constants::Half);
	math::real e0 = math::getX(extents) + EPSILON;
	math::real e1 = math::getY(extents) + EPSILON;
	math::real e2 = math::getZ(extents) + EPSILON;

	math::Vector v0_rel = math::sub(v0, c);
	math::Vector v1_rel = math::sub(v1, c);
	math::Vector v2_rel = math::sub(v2, c);

	math::Vector f0 = math::sub(v1_rel, v0_rel);
	math::Vector f1 = math::sub(v2_rel, v1_rel);
	math::Vector f2 = math::sub(v0_rel, v2_rel);

	math::Vector u0 = math::constants::XAxis;
	math::Vector u1 = math::constants::YAxis;
	math::Vector u2 = math::constants::ZAxis;

	// Naive implementation for now
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			math::Vector u, f, altF;
			if (i == 0) u = u0;
			else if (i == 1) u = u1;
			else if (i == 2) u = u2;

			if (j == 0) {
				f = f0;
				altF = f1;
			}
			else if (j == 1) {
				f = f1;
				altF = f2;
			}
			else if (j == 2) {
				f = f2;
				altF = f0;
			}

			math::Vector a = math::cross(u, f);

			if (abs(math::getX(a)) < 1e-6 && abs(math::getY(a) < 1e-6) && abs(math::getZ(a) < 1e-6)) {
				math::Vector n = math::cross(f, altF);
				a = math::cross(n, f);
			}

			r = e0 * abs(math::getScalar(math::dot(u0, a))) + e1 * abs(math::getScalar(math::dot(u1, a))) + e2 * abs(math::getScalar(math::dot(u2, a)));

			p0 = math::getScalar(math::dot(v0_rel, a));
			p1 = math::getScalar(math::dot(v1_rel, a));
			p2 = math::getScalar(math::dot(v2_rel, a));

			math::real maxp = max(max(p0, p1), p2);
			math::real minp = min(min(p0, p1), p2);

			if (minp > r) return false;
			if (maxp < -r) return false;
		}
	}

	math::real maxX = max(max(math::getX(v0_rel), math::getX(v1_rel)), math::getX(v2_rel));
	math::real minX = min(min(math::getX(v0_rel), math::getX(v1_rel)), math::getX(v2_rel));

	math::real maxY = max(max(math::getY(v0_rel), math::getY(v1_rel)), math::getY(v2_rel));
	math::real minY = min(min(math::getY(v0_rel), math::getY(v1_rel)), math::getY(v2_rel));

	math::real maxZ = max(max(math::getZ(v0_rel), math::getZ(v1_rel)), math::getZ(v2_rel));
	math::real minZ = min(min(math::getZ(v0_rel), math::getZ(v1_rel)), math::getZ(v2_rel));

	if (maxX < -e0 || minX > e0) return false;
	if (maxY < -e1 || minY > e1) return false;
	if (maxZ < -e2 || minZ > e2) return false;

	math::Vector planeNormal = math::cross(f0, f1);
	math::Vector t = math::dot(planeNormal, v0_rel);
	math::real planeD = math::getScalar(math::dot(planeNormal, v0));

	// Check the plane
	math::real plane_r = e0 * abs(math::getX(planeNormal)) + e1 * abs(math::getY(planeNormal)) + e2 * abs(math::getZ(planeNormal));
	math::real s = math::getScalar(math::dot(planeNormal, c)) - planeD;

	return abs(s) <= plane_r;
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
