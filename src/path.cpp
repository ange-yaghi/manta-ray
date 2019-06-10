#include <path.h>

#include <boost/filesystem.hpp>

manta::Path::Path(const std::string &path) {
	setPath(path);
}

manta::Path::Path(const char *path) {
	setPath(std::string(path));
}

manta::Path::Path() {
	m_path = nullptr;
}

manta::Path::Path(const boost::filesystem::path &path) {
	m_path = new boost::filesystem::path;
	*m_path = path;
}

manta::Path::~Path() {
	delete m_path;
}

std::string manta::Path::toString() const {
	return m_path->string();
}

void manta::Path::setPath(const std::string &path) {
	m_path = new boost::filesystem::path(path);
}

bool manta::Path::operator==(const Path &path) const {
	return boost::filesystem::equivalent(this->getBoostPath(), path.getBoostPath());
}

manta::Path manta::Path::append(const Path &path) const {
	return Path(getBoostPath() / path.getBoostPath());
}

bool manta::Path::isAbsolute() const {
	return m_path->is_complete();
}
