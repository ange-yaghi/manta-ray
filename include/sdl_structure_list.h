#ifndef SDL_STRUCTURE_LIST_H
#define SDL_STRUCTURE_LIST_H

#include <sdl_parser_structure.h>

#include <vector>

namespace manta {

	template <typename T>
	class SdlStructureList : public SdlParserStructure {
	public:
		SdlStructureList() { /* void */ }
		~SdlStructureList() { /* void */ }

		void add(T *item) { m_items.push_back(item); registerComponent(item); }
		T *getItem(int index) const { return m_items[index]; }
		int getItemCount() const { return (int)m_items.size(); }

	protected:
		std::vector<T *> m_items;
	};

	class SdlNode;

	typedef SdlStructureList<SdlNode> SdlNodeList;

} /* namespace manta */

#endif /* SDL_STRUCTURE_LIST_H */
