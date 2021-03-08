#ifndef DELTA_BASIC_ANIMATION_EXPORT_FILE_H
#define DELTA_BASIC_ANIMATION_EXPORT_FILE_H

#include "delta_core.h"

#include "animation_export_data.h"

#include <fstream>

namespace dbasic {

    class AnimationExportFile : public ysObject {
    public:
        enum Mode {
            Write,
            Read,
            Closed,
        };

        static const unsigned int    MAGIC_NUMBER = 0x4B564E44;
        static const int            FILE_VERSION = 0x0;

        struct ExportFileHeader {
            unsigned int FileMagicNumber;
            int FileVersion;
        };

        struct ObjectDataHeader {
            int ObjectCount;
        };

        struct MotionSectionHeader {
            int MotionCount;
        };

        struct MotionOutputHeader {
            char MotionName[64];
            int StartFrame;
            int EndFrame;
        };

        struct PoseSectionHeader {
            int PoseCount;
        };

        struct PoseOutputHeader {
            char PoseName[64];
            int Frame;
        };

        struct KeyframeSectionHeader {
            int KeyframeCount;
        };

        struct KeyframeOutputHeader {
            char ObjectName[64];
            ysObjectAnimationData::ANIMATION_KEY Type;
            int Frame;
        };

        struct KeyHeader {
            char ObjectName[64];
            int KeyCount;
        };

    public:
        AnimationExportFile();
        ~AnimationExportFile();

        ysError Open(const char *fname, Mode mode = Mode::Write);
        void Close();

        // Write Functions
        ysError WriteCustomData(void *data, int size);
        ysError WriteObjectAnimationData(AnimationExportData *objectAnimationData);

        ysError ReadObjectAnimationData(AnimationExportData *objectAnimationData);

    protected:
        int m_fileVersion;
        Mode m_openMode;

        std::fstream m_file;
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_ANIMATION_EXPORT_FILE_H */
