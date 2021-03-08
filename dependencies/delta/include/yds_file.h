#ifndef YDS_FILE_H
#define YDS_FILE_H

#include "yds_base.h"

#include <iostream>
#include <fstream>

class ysFile : ysObject {
public:
    static const int MAX_FILE_NAME_LENGTH = 256;

    enum FILE_MODE {
        // Defaults
        FILE_WRITE = 0x000000,
        FILE_TEXT = 0x000000,

        FILE_READ = 0x000001,
        FILE_BINARY = 0x000002,
    };

public:
    ysFile();
    ysFile(const char *fname);
    ~ysFile();

    ysError OpenFile(const char *fname, unsigned int filemode = FILE_READ);
    void CloseFile();

    int GetFileLength();
    void ReadFileToBuffer(char *buffer);

    bool IsOpen() { return m_file.is_open(); }

protected:
    char m_name[MAX_FILE_NAME_LENGTH];
    unsigned int m_filemode;

    std::fstream m_file;
};

#endif /* YDS_FILE_H */
