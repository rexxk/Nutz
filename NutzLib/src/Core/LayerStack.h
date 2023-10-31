#pragma once

#include "Layer.h"


namespace Nutz
{


	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack();

		void AttachLayer(Ref<Layer>& layer);
		void DetachLayer(Ref<Layer>& layer);

		std::vector<Ref<Layer>>::iterator begin() { return m_LayerStack.begin(); }
		std::vector<Ref<Layer>>::const_iterator cbegin() { return m_LayerStack.cbegin(); }

		std::vector<Ref<Layer>>::iterator end() { return m_LayerStack.end(); }
		std::vector<Ref<Layer>>::const_iterator cend() { return m_LayerStack.cend(); }

	private:
		std::vector<Ref<Layer>> m_LayerStack;
	};


}
