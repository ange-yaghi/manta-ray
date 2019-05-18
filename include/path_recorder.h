#ifndef PATH_RECORDER_H
#define PATH_RECORDER_H

#include <manta_math.h>

#include <vector>
#include <string>
#include <fstream>

namespace manta {

	class PathRecorder {
	public:
		struct PathSegment {
			math::Vector position;

			PathSegment *parent;
			std::vector<PathSegment *>children;

			int vertexIndex;
		};

		struct Tree {
			PathSegment *path;
			std::string name;
		};

	public:
		PathRecorder();
		~PathRecorder();

		void startNewTree(std::string name, const math::Vector &origin);
		void startBranch(const math::Vector &location);
		void endBranch();

		int getTreeCount() const { return (int)m_trees.size(); }
		Tree *getTree(int index) { return m_trees[index]; }

		bool writeObjFile(const std::string &fname);		

	protected:
		std::vector<Tree *> m_trees;
		PathSegment *m_currentPath;

	protected:
		int writePathSegmentVertices(std::ofstream &f, PathSegment *segment, 
			int currentVertexOffset);
		void writePathSegmentLines(std::ofstream &f, PathSegment *segment);

		void destroyTree(Tree *tree);
		void destroyPathSegment(PathSegment *segment);
	};

} /* namespace manta */

#endif /* PATH_RECORDER_H */
