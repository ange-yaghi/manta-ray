#ifndef DELTA_BASIC_CONSOLE_SHADERS_H
#define DELTA_BASIC_CONSOLE_SHADERS_H

#include "delta_core.h"

#include "shader_controls.h"
#include "shader_set.h"

namespace dbasic {

    class ConsoleShaders : public ysObject {
    public:
        ConsoleShaders();
        virtual ~ConsoleShaders();

        ysError Initialize(ShaderSet *shaderSet, ysRenderTarget *renderTarget, ysShaderProgram *shaderProgram, ysInputLayout *inputLayout);
        ysError Destroy();

        void SetCameraAngle(float angle) { m_cameraAngle = angle; }
        float GetCameraAngle() const { return m_cameraAngle; }

        void SetScreenDimensions(float width, float height);

        void CalculateCamera();

        void SetFlagBit(int bit);
        int GetFlagBit() const;

        StageEnableFlags GetFlags() const;

        void SetTexture(ysTexture *texture);

    protected:
        ConsoleShaderObjectVariables m_shaderObjectVariables;
        ShaderScreenVariables m_shaderScreenVariables;

        float m_cameraAngle;

        float m_screenWidth;
        float m_screenHeight;

        TextureHandle m_textureHandle;

    protected:
        ShaderStage *m_mainStage;
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_CONSOLE_SHADERS_H */
