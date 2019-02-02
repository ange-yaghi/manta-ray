#ifndef SINGLE_COLOR_NODE_H
#define SINGLE_COLOR_NODE_H

#include <vector_material_node.h>

namespace manta {

	class SingleColorNode : public VectorMaterialNode {
	public:
		SingleColorNode();
		SingleColorNode(const math::Vector &color);
		~SingleColorNode();

		virtual math::Vector sample(const IntersectionPoint *surfaceInteraction) const;

		void setColor(const math::Vector &color) { m_color = color; }
		math::Vector getColor() const { return m_color; }

	protected:
		math::Vector m_color;
	};

} /* namespace manta */

#endif /* SINGLE_COLOR_NODE_H */
