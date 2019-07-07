#ifndef NODE_TYPE_H
#define NODE_TYPE_H

#include <cstring>

namespace manta {

	class NodeType {
	public:
		NodeType();
		NodeType(const char *type, const NodeType *parent = nullptr);
		~NodeType();

		void initialize(const char *type, const NodeType *parent);

	public:
		const char *getType() const { return m_typeString; }
		bool isCompatibleWith(const NodeType &t) const;
		bool operator==(const NodeType &t) const;

	private:
		static int generateHash(const char *string);
		int getHash() const { return m_hash; }

	private:
		const NodeType *m_parent;
		int m_hash;
		const char *m_typeString;
	};

} /* namespace manta */

#endif /* NODE_TYPE_H */
