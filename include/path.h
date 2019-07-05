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
		Path(const Path &path);
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

		bool isAbsolute() const;
		bool exists() const;

	protected:
		boost::filesystem::path *m_path;

	protected:
		const boost::filesystem::path &getBoostPath() const { return *m_path; }
	};

} /* namespace manta */

#endif /* PATH_H */
