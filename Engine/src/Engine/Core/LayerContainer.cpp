#include "enginepch.h"
#include "LayerContainer.h"

namespace Engine
{
	LayerContainer::LayerContainer()
		: m_ActiveLayers{}
		, m_LayerInsertIndex{ 0 }
	{
	}

	LayerContainer::~LayerContainer()
	{
		for (auto& layer : m_ActiveLayers)
		{
			layer->OnDetach();
		}
		m_ActiveLayers.clear();

		for (auto& layer : m_InactiveLayers)
		{
			layer->OnDetach();
		}
		m_InactiveLayers.clear();
	}

	void LayerContainer::AddLayer(Layer* layer)
	{
		// Check if the layer is in m_InactiveLayers
		const auto it
		{
			std::find_if(m_InactiveLayers.begin(), m_InactiveLayers.end(),
				[&](const std::unique_ptr<Layer>& l)
				{
					return l.get() == layer;
				})
		};

		if (it != m_InactiveLayers.end())
		{
			m_ActiveLayers.emplace(m_ActiveLayers.begin() + m_LayerInsertIndex, std::move(*it));
			m_InactiveLayers.erase(it);
		}
		else
		{
			m_ActiveLayers.emplace(m_ActiveLayers.begin() + m_LayerInsertIndex, std::unique_ptr<Layer>(layer));
		}
		layer->OnAttach();
		m_LayerInsertIndex++;
	}

	void LayerContainer::AddOverlay(Layer* overlay)
	{
		// Check if the layer is in m_InactiveLayers
		const auto it
		{
			std::find_if(m_InactiveLayers.begin(), m_InactiveLayers.end(),
				[&](const std::unique_ptr<Layer>& ol)
				{
					return ol.get() == overlay;
				})
		};

		if (it != m_InactiveLayers.end())
		{
			m_ActiveLayers.push_back(std::move(*it));
			m_InactiveLayers.erase(it);
		}
		else
		{
			m_ActiveLayers.emplace_back(std::unique_ptr<Layer>(overlay));
		}
		overlay->OnAttach();
	}

	void LayerContainer::RemoveLayer(Layer* layer)
	{
		const auto it
		{
			std::find_if(m_ActiveLayers.begin(), m_ActiveLayers.begin() + m_LayerInsertIndex,
				[&](const std::unique_ptr<Layer>& l)
				{
					return l.get() == layer;
				})
		};

		if (it != m_ActiveLayers.begin() + m_LayerInsertIndex)
		{
			layer->OnDetach();
			m_ActiveLayers.erase(it);
			m_LayerInsertIndex--;
		}
	}

	void LayerContainer::RemoveOverlay(Layer* overlay)
	{
		const auto it
		{
			std::find_if(m_ActiveLayers.begin(), m_ActiveLayers.begin() + m_LayerInsertIndex,
				[&](const std::unique_ptr<Layer>& ol)
				{
					return ol.get() == overlay;
				})
		};
		if (it != m_ActiveLayers.end())
		{
			overlay->OnDetach();
			m_ActiveLayers.erase(it);
		}
	}

	// iterators
	std::vector<std::unique_ptr<Layer>>::iterator LayerContainer::begin()
	{
		return m_ActiveLayers.begin();
	}

	std::vector<std::unique_ptr<Layer>>::iterator LayerContainer::end()
	{
		return m_ActiveLayers.end();
	}

	std::vector<std::unique_ptr<Layer>>::reverse_iterator LayerContainer::rbegin()
	{
		return m_ActiveLayers.rbegin();
	}

	std::vector<std::unique_ptr<Layer>>::reverse_iterator LayerContainer::rend()
	{
		return m_ActiveLayers.rend();
	}

	std::vector<std::unique_ptr<Layer>>::const_iterator LayerContainer::begin() const
	{
		return m_ActiveLayers.begin();
	}

	std::vector<std::unique_ptr<Layer>>::const_iterator LayerContainer::end() const
	{
		return m_ActiveLayers.end();
	}

	std::vector<std::unique_ptr<Layer>>::const_reverse_iterator LayerContainer::rbegin() const
	{
		return m_ActiveLayers.rbegin();
	}

	std::vector<std::unique_ptr<Layer>>::const_reverse_iterator LayerContainer::rend() const
	{
		return m_ActiveLayers.rend();
	}
}