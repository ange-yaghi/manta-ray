#ifndef DELTA_BASIC_ANIMATION_EXPORT_DATA_H
#define DELTA_BASIC_ANIMATION_EXPORT_DATA_H

#include "delta_core.h"

namespace dbasic {

    class AnimationExportData;
    class AnimationExportFile;

    class PoseExport : public ysObject {
        friend AnimationExportData;

    public:
        PoseExport();
        ~PoseExport();

        const char *GetName() const { return m_name; }
        int GetFrame() const { return m_frame; }

    protected:
        char m_name[64];
        int m_frame;
    };

    class ObjectKeyframeDataExport : public ysObject {
        friend AnimationExportData;

    public:
        enum KEY_FLAG {
            KEY_FLAG_UNDEFINED = 0x0,
            KEY_FLAG_ROTATION_KEY = 0x1,
            KEY_FLAG_POSITION_KEY = 0x2
        };

        struct KEY_DATA {
            int m_frame;
            unsigned int m_keyType;

            ysVector4 m_rotationKey;    // Used only if this is a rotation key
            ysVector3 m_positionKey;    // Used only if this is a location key
        };

    public:
        ObjectKeyframeDataExport();
        ~ObjectKeyframeDataExport();

        const char *GetObjectName() const { return m_objectName; }

        KEY_DATA *AddKey(KEY_FLAG keyType, int frame);

        int GetKeyCount() const { return m_keyData.GetNumObjects(); }
        KEY_DATA *GetKeyBuffer() { return m_keyData.GetBuffer(); }
        KEY_DATA *GetKey(int i) { return &m_keyData[i]; }

        void PreallocateKeys(int count) { m_keyData.Allocate(count); }

    protected:
        char m_objectName[64];

        ysExpandingArray<KEY_DATA> m_keyData;
    };

    class MotionExport : public ysObject {
        friend AnimationExportData;

    public:
        MotionExport();
        ~MotionExport();

        void Clear();

        const char *GetName() const { return m_name; }

        int GetStartFrame() const { return m_startFrame; }
        int GetEndFrame() const { return m_endFrame; }

    protected:
        char m_name[64];

        int m_startFrame;
        int m_endFrame;
    };

    class AnimationExportData : public ysObject {
        friend AnimationExportFile;

    public:
        AnimationExportData();
        ~AnimationExportData();

        void Clear();

        // Construction Utilities

        ObjectKeyframeDataExport *AddObjectKeyData(char *object);

        void AddMotion(char *name, int startFrame, int endFrame);
        PoseExport *AddPose(char *name, int frame);

        MotionExport *GetMotion(char *name);

        int GetPoseCount() const { return m_poses.GetNumObjects(); }
        int GetKeyCount() const { return m_keyframes.GetNumObjects(); }
        int GetMotionCount() const { return m_motions.GetNumObjects(); }

        ObjectKeyframeDataExport *GetKey(int index) { return &m_keyframes[index]; }
        PoseExport *GetPose(int index) { return &m_poses[index]; }
        MotionExport *GetMotion(int index) { return &m_motions[index]; }

        ysError LoadToolObjectData(ysObjectAnimationData *objectAnimationData);
        ysError LoadTimeTagData(ysTimeTagData *timeTagData);

    protected:
        bool ParseMotionTag(const char *tag, char *name, bool &start);
        bool ParsePoseTag(const char *tag, char *name);

        bool ReadSeparator(const char *tag, int &index);
        bool ReadName(const char *tag, char *name, int &index);

        ysExpandingArray<ObjectKeyframeDataExport>    m_keyframes;
        ysExpandingArray<PoseExport>        m_poses;
        ysExpandingArray<MotionExport>    m_motions;

        int m_referenceFrame;
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_ANIMATION_EXPORT_DATA_H */
