#ifndef MANTARAY_MEDIA_INTERFACE_NODE_OUTPUT_H
#define MANTARAY_MEDIA_INTERFACE_NODE_OUTPUT_H

#include <piranha.h>

namespace manta {

	class MediaInterface;

	class MediaInterfaceNodeOutput : public piranha::NodeOutput {
	protected:
        static const piranha::ChannelType MediaInterfaceNodeType;

	public:
		MediaInterfaceNodeOutput() : NodeOutput(&MediaInterfaceNodeType) {
			m_mediaInterface = nullptr;
		}

		virtual ~MediaInterfaceNodeOutput() {
			/* void */
		}

		const MediaInterface *getMediaInterface() const { return m_mediaInterface; }
		void setMediaInterface(const MediaInterface *mediaInterface) { 
			m_mediaInterface = mediaInterface; 
		}

	protected:
		const MediaInterface *m_mediaInterface;
	};

} /* namespace manta */

#endif /* MANTARAY_MEDIA_INTERFACE_NODE_OUTPUT_H */
