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
		static void BeginScene(const glm::mat4& viewProjectionMatrix);
		static void EndScene();
		static void Flush();

		static const glm::vec4& GetDrawColor();
		static void SetDrawColor(const glm::vec4& drawColor);

		static float GetLineWidth();
		static void SetLineWidth(float width);

		// Drawing //

		// Lines

		// Rect
		static void DrawFilledRect(const Rect2f& rect);
		static void DrawFilledRect(const Rect3f& rect);
		static void DrawFilledRect(const glm::vec2& position, const glm::vec2& size);
		static void DrawFilledRect(const glm::vec3& position, const glm::vec2& size);

		static void DrawFilledRect(const Rect2f& rect, float rotationAngle);
		static void DrawFilledRect(const Rect3f& rect, float rotationAngle);
		static void DrawFilledRect(const glm::vec2& position, const glm::vec2& size, float rotationAngle);
		static void DrawFilledRect(const glm::vec3& position, const glm::vec2& size, float rotationAngle);

		static void DrawFilledRect(const glm::mat4& modelMatrix);

		// Polygons

		// Texture
		static void DrawTexture(const Ref<Texture2D>&, const Rect2f& rect, const glm::vec4& tintColor = Color::white, float tilingFactor = 1.f);
		static void DrawTexture(const Ref<Texture2D>&, const Rect3f& rect, const glm::vec4& tintColor = Color::white, float tilingFactor = 1.f);
		static void DrawTexture(const Ref<Texture2D>&, const glm::vec2& position, const glm::vec4& tintColor = Color::white, float tilingFactor = 1.f);
		static void DrawTexture(const Ref<Texture2D>&, const glm::vec3& position, const glm::vec4& tintColor = Color::white, float tilingFactor = 1.f);
		static void DrawTexture(const Ref<Texture2D>&, const glm::vec2& position, const glm::vec2& size, const glm::vec4& tintColor = Color::white, float tilingFactor = 1.f);
		static void DrawTexture(const Ref<Texture2D>&, const glm::vec3& position, const glm::vec2& size, const glm::vec4& tintColor = Color::white, float tilingFactor = 1.f);

		static void DrawTexture(const Ref<Texture2D>&, const Rect2f& rect, float rotationAngle, const glm::vec4& tintColor = Color::white, float tilingFactor = 1.f);
		static void DrawTexture(const Ref<Texture2D>&, const Rect3f& rect, float rotationAngle, const glm::vec4& tintColor = Color::white, float tilingFactor = 1.f);
		static void DrawTexture(const Ref<Texture2D>&, const glm::vec2& position, float rotationAngle, const glm::vec4& tintColor = Color::white, float tilingFactor = 1.f);
		static void DrawTexture(const Ref<Texture2D>&, const glm::vec3& position, float rotationAngle, const glm::vec4& tintColor = Color::white, float tilingFactor = 1.f);
		static void DrawTexture(const Ref<Texture2D>&, const glm::vec2& position, const glm::vec2& size, float rotationAngle, const glm::vec4& tintColor = Color::white, float tilingFactor = 1.f);
		static void DrawTexture(const Ref<Texture2D>&, const glm::vec3& position, const glm::vec2& size, float rotationAngle, const glm::vec4& tintColor = Color::white, float tilingFactor = 1.f);

		static void DrawTexture(const Ref<Texture2D>&, const glm::mat4& modelMatrix, const glm::vec4& tintColor = Color::white, float tilingFactor = 1.f);

		struct Statistics
		{
			uint32_t drawCalls{};
			uint32_t quadCount{};

			uint32_t GetTotalVerticesCount() const { return quadCount * 4; }
			uint32_t GetTotalIndicesCount() const { return quadCount * 6; }
		};
		static void ResetStats();
		static Statistics GetStats();

	private:

		static void StartBach();
		static void NextBatch();

	};
}

#endif // !RENDERER2D_H