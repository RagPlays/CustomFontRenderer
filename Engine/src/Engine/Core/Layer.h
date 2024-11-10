#ifndef LAYER_H
#define LAYER_H

#include <string>

namespace Engine
{
	class Event;

	class Layer
	{
	public:

		explicit Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		const std::string& GetName() const;

	protected:

		std::string m_DebugName;

	};
}

#endif // !LAYER_H
