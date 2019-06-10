#ifndef PATH_H
#define PATH_H

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
		Path();
		~Path();

		std::string toString() const;

		void setPath(const std::string &path);
		bool operator==(const Path &path) const;
		Path append(const Path &path) const;

		bool isAbsolute() const;

	protected:
		boost::filesystem::path *m_path;

	protected:
		const boost::filesystem::path &getBoostPath() const { return *m_path; }
	};

} /* namespace manta */

#endif /* PATH_H */
