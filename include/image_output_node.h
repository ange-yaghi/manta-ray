#ifndef IMAGE_OUTPUT_NODE_H
#define IMAGE_OUTPUT_NODE_H

#include <node.h>

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

		void setInputNode(Node *inputNode) { m_inputNode = inputNode; }

	protected:
		virtual void _initialize();
		virtual void _evaluate();
		virtual void _destroy();

		virtual void registerDependencies();

	protected:
		int m_jpegQuality;
		std::string m_outputFilename;
		bool m_gammaCorrection;

	protected:
		Node *m_inputNode;
	};

} /* namespace manta */

#endif /* IMAGE_OUTPUT_NODE_H */
