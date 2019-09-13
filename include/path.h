#ifndef MANTARAY_PATH_H
#define MANTARAY_PATH_H

#include <piranha.h>

#include <string>

// Forward Boost declaration
namespace boost {
    namespace filesystem {
        class path;
    } /* namespace filesystem */
} /* namespace boost */

namespace manta {

    class Path {      
    protected: Path(const boost::filesystem::path &path);
    public:
        Path(const std::string &path);
        Path(const char *path);
        Path(const Path &path);
        Path(const piranha::Path &path);
        Path();
        ~Path();

        std::string toString() const;

        void setPath(const std::string &path);
        bool operator==(const Path &path) const;
        Path append(const Path &path) const;

        void getParentPath(Path *path) const;

        const Path &operator =(const Path &b);

        std::string getExtension() const;
        std::string getStem() const;

        Path getAbsolute() const;

        bool isAbsolute() const;
        bool exists() const;

    protected:
        boost::filesystem::path *m_path;

    protected:
        const boost::filesystem::path &getBoostPath() const { return *m_path; }
    };

} /* namespace manta */

#endif /* MANTARAY_PATH_H */
