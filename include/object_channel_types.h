#ifndef MANTARAY_OBJECT_CHANNEL_TYPES_H
#define MANTARAY_OBJECT_CHANNEL_TYPES_H

#include <piranha.h>

namespace manta {

    struct ObjectChannel {
        static const piranha::ChannelType MeshChannel;
    };

    template <typename Type> 
    const piranha::ChannelType *LookupTypeChannelType() { 
        static_assert(false, "Invalid type lookup"); 
        return nullptr;  
    }

    // Forward declaration of all types
    class Mesh;

    // Helper macro
#define ASSIGN_CHANNEL_TYPE(type, channel) template <> const piranha::ChannelType *LookupTypeChannelType<type>() { return &ObjectChannel::channel; }

    // Register all types
    ASSIGN_CHANNEL_TYPE(Mesh, MeshChannel);
    

} /* namespace manta */

#endif /* MANTARAY_OBJECT_CHANNEL_TYPES_H */
