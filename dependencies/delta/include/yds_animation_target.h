#ifndef YDS_ANIMATION_TARGET_H
#define YDS_ANIMATION_TARGET_H

#include "yds_math.h"

#include <algorithm>
#include <assert.h>
#include <cmath>

class ysAnimationCurve;

struct TransformTarget {
    void ClearFlags() {
        Animated = 0;
    }

    bool IsAnimated() {
        return Animated > 0;
    }

    void ClearRotation(const ysQuaternion &q) {
        ClearFlags();

        Data[0] = ysMath::GetQuatW(q);
        Data[1] = ysMath::GetQuatX(q);
        Data[2] = ysMath::GetQuatY(q);
        Data[3] = ysMath::GetQuatZ(q);
    }

    void ClearLocation(const ysVector &t) {
        ClearFlags();

        Data[0] = ysMath::GetX(t);
        Data[1] = ysMath::GetY(t);
        Data[2] = ysMath::GetZ(t);
        Data[3] = 0.0f;
    }

    void Accumulate(float t, int index) {
        assert(!std::isnan(t) && !std::isinf(t));

        unsigned int bit = 0x1 << index;
        if ((Animated & bit) == 0) {
            Data[index] = t;
            Animated |= bit;
            return;
        }
        else {
            Data[index] += t;
        }
    }

    void AccumulateQuaternion(ysQuaternion &q, float weight) {
        assert(ysMath::IsValid(q));

        if (Animated == 0) {
            Data[0] = ysMath::GetQuatW(q) * weight;
            Data[1] = ysMath::GetQuatX(q) * weight;
            Data[2] = ysMath::GetQuatY(q) * weight;
            Data[3] = ysMath::GetQuatZ(q) * weight;
        }
        else {
            ysQuaternion current = GetQuaternionResult();
            float cosTheta = ysMath::GetScalar(ysMath::Dot(current, q));

            ysQuaternion transformed = q;
            if (cosTheta < 0) {
                transformed = ysMath::Negate(q);
            }

            Data[0] += ysMath::GetQuatW(transformed) * weight;
            Data[1] += ysMath::GetQuatX(transformed) * weight;
            Data[2] += ysMath::GetQuatY(transformed) * weight;
            Data[3] += ysMath::GetQuatZ(transformed) * weight;
        }

        Animated = 0xF;
    }

    ysVector GetLocationResult() {
        return ysMath::LoadVector(Data[0], Data[1], Data[2], 0.0f);
    }

    ysQuaternion GetQuaternionResult() {
        return ysMath::LoadVector(Data[0], Data[1], Data[2], Data[3]);
    }

protected:
    float Data[4];
    unsigned int Animated;
};

class ysAnimationTarget {
public:
    ysAnimationTarget();
    ~ysAnimationTarget();

    void SetLocationTarget(TransformTarget *target) { m_locationTarget = target; }
    void SetRotationTarget(TransformTarget *target) { m_rotationTarget = target; }

    void Sample(float s, float amplitude);
    void SampleRest(float amplitude);

    void SetLocationCurve(ysAnimationCurve *curve, int index);
    void SetRotationCurve(ysAnimationCurve *curve, int index);
    
protected:
    TransformTarget *m_locationTarget;
    TransformTarget *m_rotationTarget;
    ysAnimationCurve *m_locationCurves[4];
    ysAnimationCurve *m_rotationCurves[4];
};

#endif /* YDS_ANIMATION_TARGET_H */
