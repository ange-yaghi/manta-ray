#ifndef SINGLE_STRING_NODE_OUTPUT_H
#define SINGLE_STRING_NODE_OUTPUT_H

#include <string_node_output.h>

#include <string>

namespace manta {

	class SingleStringNodeOutput : public StringNodeOutput {
	public:
		SingleStringNodeOutput();
		virtual ~SingleStringNodeOutput();

		virtual void sample(const IntersectionPoint *surfaceInteraction, void *target) const;
		virtual void discreteSample2D(int x, int y, void *target) const;
		virtual void fullCompute(void *target) const;

		std::string getValue() const { return m_value; }
		void setValue(const std::string &v) { m_value = v; }

	protected:
		std::string m_value;
	};

} /* namespace manta */

#endif /* SINGLE_STRING_NODE_OUTPUT_H */
