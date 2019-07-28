#ifndef MANTARAY_OBJECT_CHANNEL_TYPES_H
#define MANTARAY_OBJECT_CHANNEL_TYPES_H

#include <piranha.h>

namespace manta {

    struct ObjectChannel {
        static const piranha::ChannelType MeshChannel;
		static const piranha::ChannelType KdTreeChannel;
    };

    template <typename Type> 
    extern inline const piranha::ChannelType *LookupChannelType() { 
        static_assert(false, "Invalid type lookup"); 
        return nullptr;  
    }

    // Forward declaration of all types
    class Mesh;
	class KDTree;

    // Helper macro
#define ASSIGN_CHANNEL_TYPE(type, channel) template <> extern inline const piranha::ChannelType *LookupChannelType<type>() { return &ObjectChannel::channel; }

    // Register all types
    ASSIGN_CHANNEL_TYPE(Mesh, MeshChannel);
	ASSIGN_CHANNEL_TYPE(KDTree, KdTreeChannel);

} /* namespace manta */

#endif /* MANTARAY_OBJECT_CHANNEL_TYPES_H */
