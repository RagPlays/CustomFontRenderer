#ifndef IMGUILAYER_H
#define IMGUILAYER_H

#include "Engine/Core/Layer.h"

#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/KeyEvent.h"
#include "Engine/Events/MouseEvent.h"

namespace Engine
{
	class ImGuiLayer : public Layer
	{
	public:

		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

		void BlockEvents(bool block);

		void SetDarkThemeColors();

		uint32_t GetActiveWidgetID() const;

	private:

		float m_Time;
		bool m_BlockEvents;

	};
}

#endif // !IMGUILAYER_H