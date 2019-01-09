#include <path_recorder.h>

#include <fstream>
#include <assert.h>
#include <sstream>

manta::PathRecorder::PathRecorder() {
	m_currentPath = nullptr;
}

manta::PathRecorder::~PathRecorder() {
	int treeCount = getTreeCount();

	for (int i = 0; i < treeCount; i++) {
		destroyTree(m_trees[i]);
	}
}

void manta::PathRecorder::startNewTree(std::string name, const math::Vector &origin) {
	Tree *newTree = new Tree;
	newTree->name = name;
	newTree->path = new PathSegment;

	newTree->path->position = origin;
	newTree->path->parent = nullptr;

	m_trees.push_back(newTree);
	m_currentPath = newTree->path;
}

void manta::PathRecorder::startBranch(const math::Vector &location) {
	assert(m_currentPath != nullptr);

	PathSegment *newSegment = new PathSegment;
	newSegment->position = location;
	newSegment->parent = m_currentPath;

	m_currentPath->children.push_back(newSegment);
	m_currentPath = newSegment;
}

void manta::PathRecorder::endBranch() {
	assert(m_currentPath != nullptr);
	assert(m_currentPath->parent != nullptr);

	m_currentPath = m_currentPath->parent;
}

bool manta::PathRecorder::writeObjFile(const std::string &fname) {	
	int treeCount = getTreeCount();

	if (treeCount <= 0) {
		// Don't bother writing if there is nothing to write
		return false;
	}

	std::ofstream f(fname);

	assert(f.is_open());

	int currentVertexIndex = 0;
	for (int i = 0; i < treeCount; i++) {
		Tree *tree = m_trees[i];
		f << "o " << tree->name << std::endl;

		currentVertexIndex = writePathSegmentVertices(f, tree->path, currentVertexIndex);
		writePathSegmentLines(f, tree->path);
	}

	if (f.bad()) {
		return false;
	}

	f.close();

	return true;
}

int manta::PathRecorder::writePathSegmentVertices(std::ofstream &f, PathSegment *segment, int currentVertexOffset) {
	int newVertexOffset = currentVertexOffset + 1;
	segment->vertexIndex = currentVertexOffset;

	math::real x = math::getX(segment->position);
	math::real y = math::getY(segment->position);
	math::real z = math::getZ(segment->position);

	f << "v " << x << " " << y << " " << z << std::endl;
	
	int childCount = (int)segment->children.size();
	for (int i = 0; i < childCount; i++) {
		newVertexOffset = writePathSegmentVertices(f, segment->children[i], newVertexOffset);
	}

	return newVertexOffset;
}

void manta::PathRecorder::writePathSegmentLines(std::ofstream &f, PathSegment *segment) {
	if (segment->parent != nullptr) {
		f << "l " << segment->vertexIndex + 1 << " " << segment->parent->vertexIndex + 1 << std::endl;
	}
	
	int childCount = (int)segment->children.size();
	for (int i = 0; i < childCount; i++) {
		writePathSegmentLines(f, segment->children[i]);
	}
}

void manta::PathRecorder::destroyTree(Tree *tree) {
	destroyPathSegment(tree->path);
	delete tree;
}

void manta::PathRecorder::destroyPathSegment(PathSegment *segment) {
	int childCount = (int)segment->children.size();
	for (int i = 0; i < childCount; i++) {
		destroyPathSegment(segment->children[i]);
	}

	delete segment;
}
