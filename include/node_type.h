#ifndef NODE_TYPE_H
#define NODE_TYPE_H

#include <cstring>

namespace manta {

	class NodeType {
	public:
		NodeType(const char *type) { m_typeString = type; m_hash = generateHash(type); }
		~NodeType() {}

	public:
		const char *getType() const { return m_typeString; }

		bool operator==(const NodeType &t) const {
			bool hashCheck = (m_hash == t.m_hash);
			if (!hashCheck) return false;
			else return (strcmp(t.m_typeString, m_typeString) == 0);
		}

	private:
		static int generateHash(const char *string) {
			// Simple hash for now
			int sum = 0, i = 0;
			while (string[i] != '\0') {
				sum += (int)string[i++];
			}
			return sum;
		}

		int getHash() const { return m_hash; }

	private:
		int m_hash;
		const char *m_typeString;
	};

} /* namespace manta */

#endif /* NODE_TYPE_H */
