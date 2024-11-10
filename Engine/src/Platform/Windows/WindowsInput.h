#ifndef WINDOWSINPUT_H
#define WINDOWSINPUT_H

#include "Engine/Core/Input.h"

namespace Engine
{
	class WindowsInput : public Input
	{
	protected:

		virtual bool IsKeyPressedImpl(KeyCode key) override;
		virtual bool IsMouseButtonPressedImpl(MouseCode button) override;
		virtual glm::vec2 GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;

	private:

	};
}

#endif // !WINDOWSINPUT_H