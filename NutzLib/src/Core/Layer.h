#pragma once



namespace Nutz
{


	class Layer
	{
	public:
		Layer(const std::string& layerName);
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}

		virtual void OnUpdate() {}


		const std::string& GetName() const { return m_LayerName; }

	private:
		std::string m_LayerName;

	};


}
