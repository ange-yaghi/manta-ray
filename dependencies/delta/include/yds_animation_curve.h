#ifndef YDS_ANIMATION_CURVE_H
#define YDS_ANIMATION_CURVE_H

#include <map>
#include <string>

class ysAnimationTarget;

class ysAnimationCurve {
public:
    struct CurveHandle {
        enum class InterpolationMode {
            Linear,
            Bezier
        };

        float s;
        float v;
        
        float l_handle_x;
        float l_handle_y;

        float r_handle_x;
        float r_handle_y;

        InterpolationMode mode;
    };

public:
    enum class CurveType {
        Undefined = 0x0,

        LocationVec = 0x1,
        RotationQuat = 0x2,

        LocationX = 0x3,
        LocationY = 0x4,
        LocationZ = 0x5,

        RotationQuatW = 0x6,
        RotationQuatX = 0x7,
        RotationQuatY = 0x8,
        RotationQuatZ = 0x9
    };

public:
    ysAnimationCurve();
    ~ysAnimationCurve();

    float Sample(float s);
    float GetRestValue();

    void SetTarget(const std::string &target) { m_target = target; }
    std::string GetTarget() const { return m_target; }

    void SetCurveType(CurveType curveType) { m_curveType = curveType; }
    CurveType GetCurveType() const { return m_curveType; }

    void AddSamplePoint(const CurveHandle &handle);
    void AddLinearSamplePoint(float s, float t);
    int GetSampleCount() const { return (int)m_samples.size(); }

    void Attach(ysAnimationTarget *target);

    static float Bezier_t(float x, float p0_x, float p1_x, float p2_x, float p3_x);

protected:
    std::map<float, CurveHandle> m_samples;
    std::string m_target;
    CurveType m_curveType;
};

#endif /* YDS_ANIMATION_CURVE_H */
