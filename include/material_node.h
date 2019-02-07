#ifndef MATERIAL_NODE_H
#define MATERIAL_NODE_H

#include <string>

namespace manta {

	struct IntersectionPoint;
	class StackAllocator;

	struct MaterialNodeMemory {
		unsigned char memory[64];
		void *extraMemory;
	};

	class MaterialNode {
	public:
		MaterialNode();
		virtual ~MaterialNode();

		virtual void initialize(const IntersectionPoint *surfaceInteraction, MaterialNodeMemory *memory, StackAllocator *stackAllocator) const;
		void free(MaterialNodeMemory *memory, StackAllocator *stackAllocator) const;

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
