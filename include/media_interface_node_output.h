#ifndef MEDIA_INTERFACE_NODE_OUTPUT_H
#define MEDIA_INTERFACE_NODE_OUTPUT_H

#include <node_output.h>

namespace manta {

	class MediaInterface;

	class MediaInterfaceNodeOutput : public NodeOutput {
	protected:
		static const NodeType MediaInterfaceNodeType;

	public:
		MediaInterfaceNodeOutput() : NodeOutput(&MediaInterfaceNodeType) {
			m_mediaInterface = nullptr;
		}

		virtual ~MediaInterfaceNodeOutput() {
			/* void */
		}

		const MediaInterface *getMediaInterface() const { return m_mediaInterface; }
		void setMediaInterface(const MediaInterface *mediaInterface) { m_mediaInterface = mediaInterface; }

	protected:
		const MediaInterface *m_mediaInterface;
	};

} /* namespace manta */

#endif /* MEDIA_INTERFACE_NODE_OUTPUT_H */
