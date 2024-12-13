#ifndef RENDERER2D_H
#define RENDERER2D_H

#include <vector>
#include <cstdint>
#include <memory>

#include <glm/glm.hpp>

namespace Engine
{
	class Camera;
	class Texture2D;

	struct Line2f;
	struct Line3f;

	struct Rect2f;
	struct Rect3f;

	struct Circle2f;
	struct Circle3f;

	class Renderer2D final
	{
	public:

		Renderer2D() = default;
		~Renderer2D() = default;

		Renderer2D(const Renderer2D& other) = delete;
		Renderer2D(Renderer2D&& other) noexcept = delete;
		Renderer2D& operator=(const Renderer2D& other) = delete;
		Renderer2D& operator=(Renderer2D&& other) noexcept = delete;
		
		// Basics ------------------------------------------------------

		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera);
		static void BeginScene(const glm::mat4& viewProjectionMatrix);
		static void EndScene();

		static const glm::vec4& GetDrawColor();
		static void SetDrawColor(const glm::vec4& drawColor);

		static float GetLineWidth();
		static void SetLineWidth(float width);

		static float GetPointSize();
		static void SetPointSize(float size);

		// Drawing ------------------------------------------------------
		
		// Points
		static void DrawPoint(const glm::vec2& position);
		static void DrawPoint(const glm::vec3& position);

		static void DrawPoints(const std::vector<glm::ivec2>& points);
		static void DrawPoints(const std::vector<glm::ivec3>& points);
		static void DrawPoints(const std::vector<glm::vec2>& points);
		static void DrawPoints(const std::vector<glm::vec3>& points);

		// Lines
		static void DrawLine(const Line2f& line);
		static void DrawLine(const Line3f& line);
		static void DrawLine(const glm::vec2& p0, const glm::vec2& p1);
		static void DrawLine(const glm::vec3& p0, const glm::vec3& p1);

		// Rect
		static void DrawRect(const Rect2f& rect);
		static void DrawRect(const Rect3f& rect);
		static void DrawRect(const glm::vec2& position, const glm::vec2& size);
		static void DrawRect(const glm::vec3& position, const glm::vec2& size);

		static void DrawRect(const Rect2f& rect, float radAngle);
		static void DrawRect(const Rect3f& rect, float radAngle);
		static void DrawRect(const glm::vec2& position, const glm::vec2& size, float radAngle);
		static void DrawRect(const glm::vec3& position, const glm::vec2& size, float radAngle);

		static void DrawRect(const glm::mat4& modelMatrix);

		// Filled Rect
		static void DrawFilledRect(const Rect2f& rect);
		static void DrawFilledRect(const Rect3f& rect);
		static void DrawFilledRect(const glm::vec2& position, const glm::vec2& size);
		static void DrawFilledRect(const glm::vec3& position, const glm::vec2& size);

		static void DrawFilledRect(const Rect2f& rect, float radAngle);
		static void DrawFilledRect(const Rect3f& rect, float radAngle);
		static void DrawFilledRect(const glm::vec2& position, const glm::vec2& size, float radAngle);
		static void DrawFilledRect(const glm::vec3& position, const glm::vec2& size, float radAngle);

		static void DrawFilledRect(const glm::mat4& modelMatrix);

		// Circle
		static void DrawCircle(const Circle2f& circle, float thickness = 0.1f, float fade = 0.005f);
		static void DrawCircle(const Circle3f& circle, float thickness = 0.1f, float fade = 0.005f);
		static void DrawCircle(const glm::vec2& center, float radius, float thickness = 0.1f, float fade = 0.005f);
		static void DrawCircle(const glm::vec3& center, float radius, float thickness = 0.1f, float fade = 0.005f);

		static void DrawCircle(const glm::mat4& modelMatrix, float thickness = 0.1f, float fade = 0.005f);

		// Filled Circle
		static void DrawFilledCircle(const Circle2f& circle, float fade = 0.005f);
		static void DrawFilledCircle(const Circle3f& circle, float fade = 0.005f);
		static void DrawFilledCircle(const glm::vec2& center, float radius, float fade = 0.005f);
		static void DrawFilledCircle(const glm::vec3& center, float radius, float fade = 0.005f);

		static void DrawFilledCircle(const glm::mat4& modelMatrix, float fade = 0.005f);

		// Polygons

		// Filled Polygons

		// Texture
		static void DrawTexture(const Ref<Texture2D>&, const Rect2f& rect, const glm::vec4& tintColor = Color::white, float tilingFactor = 1.f);
		static void DrawTexture(const Ref<Texture2D>&, const Rect3f& rect, const glm::vec4& tintColor = Color::white, float tilingFactor = 1.f);
		static void DrawTexture(const Ref<Texture2D>&, const glm::vec2& position, const glm::vec4& tintColor = Color::white, float tilingFactor = 1.f);
		static void DrawTexture(const Ref<Texture2D>&, const glm::vec3& position, const glm::vec4& tintColor = Color::white, float tilingFactor = 1.f);
		static void DrawTexture(const Ref<Texture2D>&, const glm::vec2& position, const glm::vec2& size, const glm::vec4& tintColor = Color::white, float tilingFactor = 1.f);
		static void DrawTexture(const Ref<Texture2D>&, const glm::vec3& position, const glm::vec2& size, const glm::vec4& tintColor = Color::white, float tilingFactor = 1.f);

		static void DrawTexture(const Ref<Texture2D>&, const Rect2f& rect, float radAngle, const glm::vec4& tintColor = Color::white, float tilingFactor = 1.f);
		static void DrawTexture(const Ref<Texture2D>&, const Rect3f& rect, float radAngle, const glm::vec4& tintColor = Color::white, float tilingFactor = 1.f);
		static void DrawTexture(const Ref<Texture2D>&, const glm::vec2& position, float radAngle, const glm::vec4& tintColor = Color::white, float tilingFactor = 1.f);
		static void DrawTexture(const Ref<Texture2D>&, const glm::vec3& position, float radAngle, const glm::vec4& tintColor = Color::white, float tilingFactor = 1.f);
		static void DrawTexture(const Ref<Texture2D>&, const glm::vec2& position, const glm::vec2& size, float radAngle, const glm::vec4& tintColor = Color::white, float tilingFactor = 1.f);
		static void DrawTexture(const Ref<Texture2D>&, const glm::vec3& position, const glm::vec2& size, float radAngle, const glm::vec4& tintColor = Color::white, float tilingFactor = 1.f);

		static void DrawTexture(const Ref<Texture2D>&, const glm::mat4& modelMatrix, const glm::vec4& tintColor = Color::white, float tilingFactor = 1.f);

		struct Statistics
		{
			uint32_t drawCalls{};
			uint32_t pointCount{};
			uint32_t lineCount{};
			uint32_t quadCount{};
			uint32_t circleCount{};

			uint32_t GetTotalVerticesCount() const
			{
				const uint32_t quadVerticesCount{ quadCount * 4 };
				const uint32_t lineVerticesCount{ lineCount * 2 };
				return pointCount + lineVerticesCount + quadVerticesCount;
			}
			uint32_t GetTotalIndicesCount() const { return quadCount * 6; }
		};
		static void ResetStats();
		static Statistics GetStats();

	private:

		static void SetShadersViewProjection(const glm::mat4& viewProj);

		static void NextPointBatch();
		static void NextLineBatch();
		static void NextQuadBatch();
		static void NextCircleBatch();

		static void ResetAllBatches();

		static void ResetPointBatch();
		static void ResetLineBatch();
		static void ResetQuadBatch();
		static void ResetCircleBatch();
		static void ResetTextureBatch();

		static void FlushAll();
		static void FlushPoints();
		static void FlushLines();
		static void FlushQuads();
		static void FlushCircles();

	};
}

#endif // !RENDERER2D_H