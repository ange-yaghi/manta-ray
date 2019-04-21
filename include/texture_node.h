#ifndef TEXTURE_MAP_H
#define TEXTURE_MAP_H

#include <vector_node.h>

#include <manta_math.h>
#include <image_file_node.h>

namespace manta {

	struct IntersectionPoint;

	class TextureNode : public VectorNode {
	public:
		TextureNode();
		virtual ~TextureNode();

		// Deprecated
		void loadFile(const char *fname, bool correctGamma);

		virtual math::Vector sample(const IntersectionPoint *surfaceInteraction) const;

	protected:
		virtual void registerDependencies();

		virtual void _destroy();

	protected:
		Node *m_mapInputNode;
		ImageFileNode m_defaultNode; // Deprecated
	};

} /* namespace manta */

#endif /* TEXTURE_MAP_H */
