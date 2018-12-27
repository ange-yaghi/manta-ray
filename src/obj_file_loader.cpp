#include <obj_file_loader.h>

#include <standard_allocator.h>

#include <fstream>
#include <sstream>
#include <cctype>
#include <functional>
#include <string>

// Taken from https://www.oreilly.com/library/view/c-cookbook/0596007612/ch04s07.html
template<typename T>
void split(const std::basic_string<T>& s, T c, std::vector<std::basic_string<T> >& v) {
	typename std::basic_string<T>::size_type i = 0;
	typename std::basic_string<T>::size_type j = s.find(c);

	if (j == std::basic_string<T>::npos) {
		v.push_back(std::string(s));
		return;
	}

	while (j != std::basic_string<T>::npos) {
		v.push_back(s.substr(i, j - i));
		i = ++j;
		j = s.find(c, j);

		if (j == std::basic_string<T>::npos)
			v.push_back(s.substr(i, s.length()));
	}
}

bool isWhitespace(const std::string &s) {
	for (int i = 0; i < s.length(); i++) {
		if (!std::isspace(s[i])) {
			return false;
		}
	}
	return true;
}

manta::ObjFileLoader::ObjFileLoader() {
	m_currentLine = 0;
}

manta::ObjFileLoader::~ObjFileLoader() {
}

bool manta::ObjFileLoader::readObjFile(const char *fname) {
	std::ifstream inputFile(fname, std::ios::in);

	if (inputFile.is_open()) {
		return readObjFile(inputFile);
	}
	else {
		// The file could not be loaded
		return false;
	}
}

bool manta::ObjFileLoader::readObjFile(std::istream &stream) {
	m_currentLine = 0;
	
	for (std::string line; std::getline(stream, line); ) {
		m_currentLine++;
		std::stringstream ss(line);

		std::string firstWord;
		ss >> firstWord;

		// Skip this line if it's a comment
		if (firstWord[0] == '#') continue;
		else if (firstWord.compare("v") == 0) {
			// Create a new vertex
			math::Vector3 *newVertex = StandardAllocator::Global()->allocate<math::Vector3>();
			m_vertices.push_back(newVertex);
			bool result = readVertexPosition(ss, newVertex);
			if (!result) {
				return false;
			}
		}
		else if (firstWord.compare("vt") == 0) {
			// Read a texture coordinate
			math::Vector2 *texCoords = StandardAllocator::Global()->allocate<math::Vector2>();
			m_texCoords.push_back(texCoords);
			bool result = readVertexTextureCoords(ss, texCoords);
			if (!result) {
				return false;
			}
		}
		else if (firstWord.compare("vn") == 0) {
			// Read a normal value
			math::Vector3 *normal = StandardAllocator::Global()->allocate<math::Vector3>();
			m_normals.push_back(normal);
			bool result = readVertexNormal(ss, normal);
			if (!result) {
				return false;
			}
		}
		else if (firstWord.compare("f") == 0) {
			// Read a face
			ObjFace *newFace = StandardAllocator::Global()->allocate<ObjFace>();
			m_faces.push_back(newFace);
			bool result = readFace(ss, newFace);
			if (!result) {
				return false;
			}
		}
	}

	return true;
}

bool manta::ObjFileLoader::readVertexPosition(std::stringstream &s, math::Vector3 *vertex) const {
	s >> vertex->x;
	if (s.fail()) {
		return false;
	}

	s >> vertex->y;
	if (s.fail()) {
		return false;
	}

	s >> vertex->z;
	if (s.fail()) {
		return false;
	}

	if (!s.eof()) {
		return false;
	}
	else {
		return true;
	}
}

bool manta::ObjFileLoader::readVertexNormal(std::stringstream &s, math::Vector3 *vertex) const {
	s >> vertex->x;
	if (s.fail()) {
		return false;
	}

	s >> vertex->y;
	if (s.fail()) {
		return false;
	}

	s >> vertex->z;
	if (s.fail()) {
		return false;
	}

	if (!s.eof()) {
		return false;
	}
	else {
		return true;
	}
}

bool manta::ObjFileLoader::readVertexTextureCoords(std::stringstream &s, math::Vector2 *texCoords) const {
	s >> texCoords->x;
	if (s.fail()) {
		return false;
	}

	s >> texCoords->y;
	if (s.fail()) {
		return false;
	}

	if (!s.eof()) {
		return false;
	}
	else {
		return true;
	}
}

bool manta::ObjFileLoader::readFace(std::stringstream &s, ObjFace *face) {
	size_t nParameters = 0;
	int vertexIndex = 0;

	// Initialize all indices
	for (int i = 0; i < 3; i++) {
		face->vt[i] = 0;
		face->vn[i] = 0;
		face->v[i] = 0;
	}

	while (!s.eof()) {
		std::string group;
		s >> group;

		std::vector<std::string> elements;
		split(group, '/', elements);

		if (nParameters == 0) nParameters = elements.size();
		else {
			if (nParameters != elements.size()) {
				return false;
			}
		}

		if (nParameters >= 1) {		
			std::stringstream ss = std::stringstream(elements[0]);
			ss >> face->v[vertexIndex];
			if (ss.fail()) {
				return false;
			}
		}
		if (nParameters >= 2) {
			if (!isWhitespace(elements[1])) {
				std::stringstream ss = std::stringstream(elements[1]);
				ss >> face->vt[vertexIndex];
				if (ss.fail()) {
					return false;
				}
			}
		}
		if (nParameters == 3) {
			if (!isWhitespace(elements[2])) {
				std::stringstream ss = std::stringstream(elements[2]);
				ss >> face->vn[vertexIndex];
				if (ss.fail()) {
					return false;
				}
			}
		}
		if (nParameters > 3) { return false; }

		vertexIndex++;
	}

	return true;
}

void manta::ObjFileLoader::destroy() {
	for (std::vector<math::Vector3 *>::iterator it = m_vertices.begin(); it != m_vertices.end(); ++it) {
		StandardAllocator::Global()->free(*it);
	}

	for (std::vector<math::Vector3 *>::iterator it = m_normals.begin(); it != m_normals.end(); ++it) {
		StandardAllocator::Global()->free(*it);
	}

	for (std::vector<math::Vector2 *>::iterator it = m_texCoords.begin(); it != m_texCoords.end(); ++it) {
		StandardAllocator::Global()->free(*it);
	}

	for (std::vector<ObjFace *>::iterator it = m_faces.begin(); it != m_faces.end(); ++it) {
		StandardAllocator::Global()->free(*it);
	}
}
