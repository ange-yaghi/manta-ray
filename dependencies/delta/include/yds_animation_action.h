#ifndef YDS_ANIMATION_ACTION_H
#define YDS_ANIMATION_ACTION_H

#include "yds_base.h"

#include "yds_animation_curve.h"
#include "yds_math.h"

#include <vector>
#include <string>
#include <map>

class ysAnimationTarget;

class ysAnimationAction : public ysObject {
public:
    ysAnimationAction();
    ~ysAnimationAction();

    ysAnimationCurve *NewCurve(const std::string &target);
    ysAnimationCurve *GetCurve(const std::string &target, ysAnimationCurve::CurveType type);
    int GetCurveCount() const;

    float GetLength() const { return m_length; }
    void SetLength(float length) { m_length = length; }

    void SetName(const std::string &name) { m_name = name; }
    std::string GetName() const { return m_name; }

    bool IsAnimated(const std::string &objectName) const;
    void Bind(const std::string &objectName, ysAnimationTarget *target);

protected:
    std::string m_name;
    std::map<std::string, std::vector<ysAnimationCurve *>> m_curves;
    float m_length;

    int m_curveCount;
};

#endif /* YDS_ANIMATION_ACTION_H */
