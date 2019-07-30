#ifndef MANTARAY_OBJECT_CHANNEL_TYPES_H
#define MANTARAY_OBJECT_CHANNEL_TYPES_H

#include <piranha.h>

namespace manta {

    struct ObjectChannel {
        static const piranha::ChannelType MeshChannel;
		static const piranha::ChannelType KdTreeChannel;
        static const piranha::ChannelType BsdfChannel;
        static const piranha::ChannelType MaterialChannel;
        static const piranha::ChannelType MicrofacetDistributionChannel;
		static const piranha::ChannelType MaterialLibraryChannel;
    };

    template <typename Type> 
    extern inline const piranha::ChannelType *LookupChannelType() { 
        static_assert(false, "Invalid type lookup"); 
        return nullptr;  
    }

    // Forward declaration of all types
    class Mesh;
	class KDTree;
    class BSDF;
    class SimpleBSDFMaterial;
    class MicrofacetDistribution;
	class MaterialLibrary;
	class Material;

    // Helper macro
#define ASSIGN_CHANNEL_TYPE(type, channel) template <> extern inline const piranha::ChannelType *LookupChannelType<type>() { return &ObjectChannel::channel; }

    // Register all types
    ASSIGN_CHANNEL_TYPE(Mesh, MeshChannel);
	ASSIGN_CHANNEL_TYPE(KDTree, KdTreeChannel);
    ASSIGN_CHANNEL_TYPE(BSDF, BsdfChannel);
    ASSIGN_CHANNEL_TYPE(Material, MaterialChannel);
    ASSIGN_CHANNEL_TYPE(MicrofacetDistribution, MicrofacetDistributionChannel);
	ASSIGN_CHANNEL_TYPE(MaterialLibrary, MaterialLibraryChannel);

} /* namespace manta */

#endif /* MANTARAY_OBJECT_CHANNEL_TYPES_H */
