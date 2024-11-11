#ifndef RENDERER2D_H
#define RENDERER2D_H

#include <cstdint>
#include <memory>

#include <glm/glm.hpp>

namespace Engine
{
	class Camera;
	class Texture2D;

	struct Rect2f;
	struct Rect3f;

	class Renderer2D final
	{
	public:

		Renderer2D() = default;
		~Renderer2D() = default;

		Renderer2D(const Renderer2D& other) = delete;
		Renderer2D(Renderer2D&& other) noexcept = delete;
		Renderer2D& operator=(const Renderer2D& other) = delete;
		Renderer2D& operator=(Renderer2D&& other) noexcept = delete;
		
		// Basics //
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera);
		static void EndScene();
		static void Flush();

		static const glm::vec4& GetDrawColor();
		static void SetDrawColor(const glm::vec4& drawColor);

		static float GetLineWidth();
		static void SetLineWidth(float width);

		// Drawing //

		// Rect
		static void DrawFilledRect(const Rect2f& rect, float rotationAngle = 0.f);
		static void DrawFilledRect(const Rect3f& rect, float rotationAngle = 0.f);
		static void DrawFilledRect(const glm::vec2& position, const glm::vec2& size, float rotationAngle = 0.f);
		static void DrawFilledRect(const glm::vec3& position, const glm::vec2& size, float rotationAngle = 0.f);

		// Texture
		static void DrawTexture(const Ref<Texture2D>& texture, const glm::vec2& position, float rotationAngle = 0.f, const glm::vec4& tintColor = Color::white);
		static void DrawTexture(const Ref<Texture2D>& texture, const glm::vec3& position, float rotationAngle = 0.f, const glm::vec4& tintColor = Color::white);
		static void DrawTexture(const Ref<Texture2D>& texture, const Rect2f& rect, float rotationAngle = 0.f, const glm::vec4& tintColor = Color::white);
		static void DrawTexture(const Ref<Texture2D>& texture, const Rect3f& rect, float rotationAngle = 0.f, const glm::vec4& tintColor = Color::white);
		static void DrawTexture(const Ref<Texture2D>& texture, const glm::vec2& position, const glm::vec2& size, float rotationAngle = 0.f, const glm::vec4& tintColor = Color::white);
		static void DrawTexture(const Ref<Texture2D>& texture, const glm::vec3& position, const glm::vec2& size, float rotationAngle = 0.f, const glm::vec4& tintColor = Color::white);

	private:

		static void CreateBasicResources();

	};
}

#endif // !RENDERER2D_H