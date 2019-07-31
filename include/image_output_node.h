#ifndef MANTARAY_IMAGE_OUTPUT_NODE_H
#define MANTARAY_IMAGE_OUTPUT_NODE_H

#include "node.h"

#include "vector_map_2d_node_output.h"

namespace manta {

	class ImageOutputNode : public Node {
	public:
		ImageOutputNode();
		~ImageOutputNode();

		void setGammaCorrection(bool gammaCorrection) { m_gammaCorrection = gammaCorrection; }
		bool getGammaCorrection() const { return m_gammaCorrection; }

		void setOutputFilename(const std::string &fname) { m_outputFilename = fname; }
		const std::string &getOutputFilename() const { return m_outputFilename; }

		void setJpegQuality(int quality) { m_jpegQuality = quality; }
		int getJpegQuality() const { return m_jpegQuality; }

		void setInput(piranha::pNodeInput inputNode) { m_input = inputNode; }

	protected:
		virtual void _initialize();
		virtual void _evaluate();
		virtual void _destroy();

		virtual void registerInputs();
		virtual void registerOutputs();

	protected:
		int m_jpegQuality;
		std::string m_outputFilename;
		bool m_gammaCorrection;

        piranha::pNodeInput m_input;
        piranha::pNodeInput m_correctGammaInput;
        piranha::pNodeInput m_jpegQualityInput;
        piranha::pNodeInput m_filenameInput;
	};

} /* namespace manta */

#endif /* IMAGE_OUTPUT_NODE_H */
