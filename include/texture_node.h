#ifndef TEXTURE_MAP_H
#define TEXTURE_MAP_H

#include <node.h>

#include <manta_math.h>
#include <image_file_node.h>
#include <vector_map_2d_node_output.h>

namespace manta {

	struct IntersectionPoint;

	class TextureNode : public Node {
	public:
		TextureNode();
		virtual ~TextureNode();

		// Deprecated
		void loadFile(const char *fname, bool correctGamma);

		const VectorMap2DNodeOutput *getMainOutput() const { return &m_textureOutput; }

	protected:
		virtual void registerInputs();
		virtual void registerOutputs();

		virtual void _initialize();
		virtual void _evaluate();
		virtual void _destroy();

	protected:
		VectorMap2DNodeOutput m_textureOutput;
		ImageFileNode m_defaultNode; // Deprecated
	};

} /* namespace manta */

#endif /* TEXTURE_MAP_H */
