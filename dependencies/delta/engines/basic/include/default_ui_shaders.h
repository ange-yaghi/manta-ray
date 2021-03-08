#ifndef DELTA_BASIC_DEFAULT_UI_SHADERS_H
#define DELTA_BASIC_DEFAULT_UI_SHADERS_H

#include "delta_core.h"

#include "shader_controls.h"
#include "shader_set.h"

namespace dbasic {

    class DefaultUiShaders : public ysObject {
    public:
        enum class CameraMode {
            Target,
            Flat
        };

    public:
        DefaultUiShaders();
        virtual ~DefaultUiShaders();

        ysError Initialize(ysDevice *device, ysRenderTarget *renderTarget, ysShaderProgram *shaderProgram, ysInputLayout *inputLayout);
        ysError Destroy();

        ShaderSet *GetShaderSet() { return &m_shaderSet; }

        void SetCameraAngle(float angle) { m_cameraAngle = angle; }
        float GetCameraAngle() const { return m_cameraAngle; }

        void SetScreenDimensions(float width, float height);

        void CalculateCamera();
        
    protected:
        ShaderScreenVariables m_shaderScreenVariables;
        ShaderObjectVariables m_shaderObjectVariables;
        LightingControls m_lightingControls;

        float m_cameraAngle;

        float m_screenWidth;
        float m_screenHeight;

    protected:
        ShaderSet m_shaderSet;
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_DEFAULT_UI_SHADERS_H */
