#include "../include/object_channel_types.h"

const piranha::ChannelType manta::ObjectChannel::SceneGeometryChannel("SceneGeometryChannel");
const piranha::ChannelType manta::ObjectChannel::MeshChannel("MeshChannel", &SceneGeometryChannel);
const piranha::ChannelType manta::ObjectChannel::KdTreeChannel("KdTreeChannel", &SceneGeometryChannel);
const piranha::ChannelType manta::ObjectChannel::BsdfChannel("BsdfChannel");
const piranha::ChannelType manta::ObjectChannel::BxdfChannel("BxdfChannel");
const piranha::ChannelType manta::ObjectChannel::MaterialChannel("MaterialChannel");
const piranha::ChannelType manta::ObjectChannel::MicrofacetDistributionChannel("MicrofacetDistributionChannel");
const piranha::ChannelType manta::ObjectChannel::MaterialLibraryChannel("MaterialLibraryChannel");
const piranha::ChannelType manta::ObjectChannel::SceneObjectChannel("SceneObjectChannel");
const piranha::ChannelType manta::ObjectChannel::SceneChannel("SceneChannel");
const piranha::ChannelType manta::ObjectChannel::SamplerChannel("SamplerChannel");
const piranha::ChannelType manta::ObjectChannel::CameraChannel("CameraChannel");
const piranha::ChannelType manta::ObjectChannel::ApertureChannel("ApertureChannel");
const piranha::ChannelType manta::ObjectChannel::LensChannel("LensChannel");
const piranha::ChannelType manta::ObjectChannel::MediaInterfaceChannel("MediaInterfaceChannel");
const piranha::ChannelType manta::ObjectChannel::FilterChannel("FilterChannel");
