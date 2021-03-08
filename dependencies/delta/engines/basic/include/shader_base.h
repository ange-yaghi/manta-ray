#ifndef DELTA_BASIC_SHADER_BASE_H
#define DELTA_BASIC_SHADER_BASE_H

#include "delta_core.h"

#include "material.h"
#include "model_asset.h"

namespace dbasic {

	class ShaderBase : public ysObject {
	public:
		ShaderBase();
		virtual ~ShaderBase();

		virtual ysError UseMaterial(Material* material) = 0;

		virtual void SetObjectTransform(const ysMatrix& mat) = 0;
		virtual void ConfigureModel(float scale, ModelAsset* model) = 0;
	};

} /* namespace dbasic */

#endif /* DELTA_BASIC_SHADER_BASE_H */
