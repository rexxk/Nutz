#include "nutz_pch.h"
#include "LayerStack.h"



namespace Nutz
{


	LayerStack::~LayerStack()
	{
		for (auto& layer : m_LayerStack)
		{
			layer->OnDetach();
		}

		m_LayerStack.clear();
	}


	void LayerStack::AttachLayer(Ref<Layer>& layer)
	{
		if (std::find(m_LayerStack.begin(), m_LayerStack.end(), layer) == m_LayerStack.end())
		{
			m_LayerStack.emplace_back(layer);

			layer->OnAttach();
		}
	}

	void LayerStack::DetachLayer(Ref<Layer>& layer)
	{
		auto location = std::find(m_LayerStack.begin(), m_LayerStack.end(), layer);

		if (location != m_LayerStack.end())
		{
			m_LayerStack.erase(location);

			layer->OnDetach();
		}
	}

}
