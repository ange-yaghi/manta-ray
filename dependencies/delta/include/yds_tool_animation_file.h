#ifndef YDS_TOOL_ANIMATION_FILE_H
#define YDS_TOOL_ANIMATION_FILE_H

#include "yds_base.h"
#include "yds_object_animation_data.h"
#include "yds_time_tag_data.h"

#include <fstream>

class ysToolAnimationFile : public ysObject {
public:
    struct HeaderVersion000 {
        int ObjectCount;
    };

    // Keep track of allocations
    class Allocation : public ysDynamicArrayElement {
    public:
        void *m_allocation;
    };

    // File Information
    static const int CURRENT_FILE_VERSION = 0x0;
    static const unsigned int MAGIC_NUMBER = 0x50F1AA;

    // Software codes
    enum class EditorId {
        UNDEFINED = 0x0,

        AUTODESK_3DS_MAX = 0x1,
        DELTA_STUDIOS_COMPILER = 0x2,
        BLENDER = 0x3,

        COUNT = 0x3
    };

    // Compilation status
    enum class CompilationStatus {
        UNDEFINED = 0x0,
        RAW = 0x1,
        COMPILED = 0x2,

        COUNT
    };

public:
    ysToolAnimationFile();
    ~ysToolAnimationFile();

    ysError Open(const char *fname);
    ysError Close();

    int GetFileVersion() const { return m_fileVersion; }
    int GetObjectCount() const;
    EditorId GetLastEditor() const { return m_lastEditor; }
    CompilationStatus GetCompilationStatus() const { return m_compilationStatus; }

    ysError ReadObjectAnimation(ysObjectAnimationData **newObject);
    ysError ReadTimeTagData(ysTimeTagData **newTimeTagData);

protected:
    // Protected Methods
    ysError ReadHeader(int fileVersion);
    ysError ReadString(char *dest);

    ysError ReadObjectAnimationVersion000(ysObjectAnimationData *object);
    ysError ReadTimeTagDataVersion000(ysTimeTagData *timeTagData);

    template<typename Type>
    Type *Allocate(int count) {
        //Type *ret = (Type *)malloc(sizeof(Type) * count);
        Type *ret = new Type[count];
        Allocation *track = m_allocationTracker.New();
        track->m_allocation = (void *)ret;

        return ret;
    }

    void DestroyMemory();
    void ReleaseMemory();

protected:
    void *m_header;
    int m_fileVersion;

    EditorId m_lastEditor;
    CompilationStatus m_compilationStatus;

    // File
    std::fstream m_file;

    ysDynamicArray<Allocation, 4> m_allocationTracker;
};

#endif /* YDS_TOOL_ANIMATION_FILE_H */
