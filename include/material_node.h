#ifndef MATERIAL_NODE_H
#define MATERIAL_NODE_H

#include <string>

namespace manta {

	class MaterialNode {
	public:
		MaterialNode();
		virtual ~MaterialNode();

		void setId(int id) { m_id = id; }
		int getId() const { return m_id; }

		void setName(const std::string &name) { m_name = name; }
		std::string getName() const { return m_name; }

	protected:
		int m_id;
		std::string m_name;
	};

} /* namespace manta */

#endif /* MATERIAL_NODE_H */
