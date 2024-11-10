#ifndef LAYERSTACK_H
#define LAYERSTACK_H

#include <vector>

#include "Engine/Core/Layer.h"

namespace Engine
{
	class LayerContainer final
	{
	public:

		LayerContainer();
		~LayerContainer();

		void AddLayer(Layer* layer);
		void AddOverlay(Layer* overlay);
		void RemoveLayer(Layer* layer);
		void RemoveOverlay(Layer* overlay);

		// Iterators
		std::vector<std::unique_ptr<Layer>>::iterator begin();
		std::vector<std::unique_ptr<Layer>>::iterator end();
		std::vector<std::unique_ptr<Layer>>::reverse_iterator rbegin();
		std::vector<std::unique_ptr<Layer>>::reverse_iterator rend();

		std::vector<std::unique_ptr<Layer>>::const_iterator begin() const;
		std::vector<std::unique_ptr<Layer>>::const_iterator end()	const;
		std::vector<std::unique_ptr<Layer>>::const_reverse_iterator rbegin() const;
		std::vector<std::unique_ptr<Layer>>::const_reverse_iterator rend() const;

	private:

		std::vector<std::unique_ptr<Layer>> m_ActiveLayers;
		std::vector<std::unique_ptr<Layer>> m_InactiveLayers;
		unsigned int m_LayerInsertIndex;

	};
}

#endif // !LAYERSTACK_H
