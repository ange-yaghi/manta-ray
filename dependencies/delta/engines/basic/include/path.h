#ifndef DELTA_BASIC_PATH_H
#define DELTA_BASIC_PATH_H

#include <string>

namespace boost {
    namespace filesystem {
        class path;
    } /* namespace filesystem */
} /* namespace boost */

namespace dbasic {

    class Path {
    protected: Path(const boost::filesystem::path &path);
    public:
        Path(const std::string &path);
        Path(const char *path);
        Path(const Path &path);
        Path();
        ~Path();

        std::string ToString() const;

        void SetPath(const std::string &path);
        bool operator==(const Path &path) const;
        Path Append(const Path &path) const;

        void GetParentPath(Path *path) const;

        const Path &operator=(const Path &b);

        std::string GetExtension() const;
        std::string GetStem() const;

        Path GetAbsolute() const;

        bool IsAbsolute() const;
        bool Exists() const;

    protected:
        boost::filesystem::path *m_path;

    protected:
        const boost::filesystem::path &GetBoostPath() const { return *m_path; }
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_PATH_H */
