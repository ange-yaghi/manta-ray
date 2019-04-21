#ifndef CONVOLUTION_NODE_H
#define CONVOLUTION_NODE_H

#include <map_2d_node.h>

namespace manta {

	class ConvolutionNode : public VectorMap2DNode {
	public:
		ConvolutionNode();
		~ConvolutionNode();

		void setInputs(Node *base, Node *filter) { m_base = base; m_filter = filter; }

		void setResize(bool resize) { m_resize = resize; }
		bool getResize() const { return m_resize; }

		void setClip(bool clip) { m_clip = clip; }
		bool getClip() const { return m_clip; }

	protected:
		virtual void _initialize();
		virtual void _evaluate();
		virtual void _destroy();

		virtual void registerDependencies();

	protected:
		Node *m_base;
		Node *m_filter;
		bool m_resize;
		bool m_clip;

		VectorMap2D m_outputMap;
	};

} /* namespace manta */

#endif /* IMAGE_OUTPUT_NODE_H */

