#ifndef IMGUILAYER_H
#define IMGUILAYER_H

#if ENGINE_IMGUI

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

		void Begin();
		void End();

		void BlockEvents(bool block);

		void SetDarkThemeColors();

	private:

		float m_Time;
		bool m_BlockEvents;

	};
}

#endif

#endif // !IMGUILAYER_H