#include "enginepch.h"
#include "Renderer2D.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Engine/Renderer/Camera.h"
#include "Engine/Core/Structs.h"

#include "Engine/Renderer/RenderCommand.h"
#include "Engine/Renderer/RendererAPI.h"

#include "Engine/Renderer/Texture.h"
#include "Engine/Renderer/Shader.h"

namespace Engine
{
	struct PointVertex
	{
		glm::vec3 position;
		glm::vec4 color;

		static BufferLayout GetBufferLayout()
		{
			return BufferLayout
			{
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Color" }
			};
		}
	};

	struct LineVertex
	{
		glm::vec3 position;
		glm::vec4 color;

		static BufferLayout GetBufferLayout()
		{
			return BufferLayout
			{
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Color" }
			};
		}
	};

	struct QuadVertex
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 texCoord;
		int texIndex;
		float tilingFactor;

		static BufferLayout GetBufferLayout()
		{
			return BufferLayout
			{
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Color" },
				{ ShaderDataType::Float2, "a_TexCoord" },
				{ ShaderDataType::Int, "a_TexIndex" },
				{ ShaderDataType::Float, "a_TilingFactor" }
			};
		}
	};

	struct CircleVertex
	{
		glm::vec3 worldPosition;
		glm::vec3 localPosition;
		glm::vec4 color;
		float thickness;
		float fade;

		static BufferLayout GetBufferLayout()
		{
			return BufferLayout
			{
				{ ShaderDataType::Float3, "a_WorldPosition" },
				{ ShaderDataType::Float3, "a_LocalPosition" },
				{ ShaderDataType::Float4, "a_Color" },
				{ ShaderDataType::Float, "a_Thickness" },
				{ ShaderDataType::Float, "a_Fade" }
			};
		}
	};

	struct Renderer2DData
	{
		Renderer2DData() = default;

		// ------------ DEFAULT VALUES --------------//
		static constexpr glm::mat4 identityMatrix{ 1.f };
		static constexpr glm::vec3 zAxis{ 0.f, 0.f, 1.f };

		// -------------- MAX VALUES ----------------//
		static constexpr uint32_t maxPointVertices{ 1000 };
		static constexpr uint32_t maxLineVertices{ 1000 * 2 };
		static constexpr uint32_t maxCircleVertices{ 2000 };
		static constexpr uint32_t maxQuads{ 5000 };
		static constexpr uint32_t maxQuadVertices{ maxQuads * 4 };
		static constexpr uint32_t maxQuadIndices{ maxQuads * 6 };
		static constexpr uint32_t maxTextureSlots{ 32 };

		static_assert(maxLineVertices % 2 == 0, "maxPointsVertices must be divisible by 2");
		static_assert(maxQuadIndices % 6 == 0, "maxQuadIndices must be divisible by 6");

		// -------------- BASICS ----------------//
		glm::vec4 drawColor{ 1.f, 1.f, 1.f, 1.f };
		float pointSize{ 5.f };
		float lineWidth{ 2.f };

		// -------------- POINTS ----------------//
		Ref<VertexArray> pointVertexArray{ nullptr };
		Ref<VertexBuffer> pointVertexBuffer{ nullptr };
		Ref<Shader> pointShader{ nullptr };
		uint32_t pointVertexCount{ 0 };
		PointVertex* pointVertexBufferBase{ nullptr };
		PointVertex* pointVertexBufferPtr{ nullptr };

		// -------------- LINES ----------------//
		Ref<VertexArray> lineVertexArray{ nullptr };
		Ref<VertexBuffer> lineVertexBuffer{ nullptr };
		Ref<Shader> lineShader{ nullptr };
		uint32_t lineVertexCount{ 0 };
		LineVertex* lineVertexBufferBase{ nullptr };
		LineVertex* lineVertexBufferPtr{ nullptr };

		// -------------- QUADS ----------------//
		constexpr static std::array<glm::vec4, 4> quadVertexPositions
		{
			glm::vec4{ -0.5f, -0.5f, 0.0f, 1.0f },
			glm::vec4{  0.5f, -0.5f, 0.0f, 1.0f },
			glm::vec4{  0.5f,  0.5f, 0.0f, 1.0f },
			glm::vec4{ -0.5f,  0.5f, 0.0f, 1.0f }
		};
		Ref<VertexArray> quadVertexArray{ nullptr };
		Ref<VertexBuffer> quadVertexBuffer{ nullptr };
		Ref<Shader> quadShader{ nullptr };
		uint32_t quadIndexCount{ 0 };
		QuadVertex* quadVertexBufferBase{ nullptr };
		QuadVertex* quadVertexBufferPtr{ nullptr };

		// -------------- CIRCLES ----------------//
		Ref<VertexArray> circleVertexArray{ nullptr };
		Ref<VertexBuffer> circleVertexBuffer{ nullptr };
		Ref<Shader> circleShader{ nullptr };
		uint32_t circleIndexCount{ 0 };
		CircleVertex* circleVertexBufferBase{ nullptr };
		CircleVertex* circleVertexBufferPtr{ nullptr };

		// -------------- TEXTURES ----------------//
		Ref<Texture2D> whiteTexture{ nullptr };
		std::array<Ref<Texture2D>, maxTextureSlots> textureSlots{};
		uint32_t textureSlotIndex{ 1 }; // 0 is white 1x1 pixel texture

#if RENDERER2D_STATISTICS
		// Stats
		Renderer2D::Statistics stats{};
#endif
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		ENGINE_PROFILE_FUNCTION();

		// -------------- BASICS ----------------//
		s_Data.drawColor = Color::white;
		s_Data.lineWidth = 2.f;

		// -------------- POINTS ----------------//
		s_Data.pointVertexArray = VertexArray::Create();

		s_Data.pointVertexBuffer = VertexBuffer::Create(Renderer2DData::maxPointVertices * sizeof(PointVertex));
		s_Data.pointVertexBuffer->SetLayout(PointVertex::GetBufferLayout());
		s_Data.pointVertexArray->AddVertexBuffer(s_Data.pointVertexBuffer);

		s_Data.pointVertexBufferBase = new PointVertex[Renderer2DData::maxPointVertices];

		// -------------- LINES ----------------//
		s_Data.lineVertexArray = VertexArray::Create();

		s_Data.lineVertexBuffer = VertexBuffer::Create(Renderer2DData::maxLineVertices * sizeof(LineVertex));
		s_Data.lineVertexBuffer->SetLayout(LineVertex::GetBufferLayout());
		s_Data.lineVertexArray->AddVertexBuffer(s_Data.lineVertexBuffer);

		s_Data.lineVertexBufferBase = new LineVertex[Renderer2DData::maxLineVertices];

		// -------------- QUADS ----------------//
		s_Data.quadVertexArray = VertexArray::Create();

		s_Data.quadVertexBuffer = VertexBuffer::Create(Renderer2DData::maxQuadVertices * sizeof(QuadVertex));
		s_Data.quadVertexBuffer->SetLayout(QuadVertex::GetBufferLayout());
		s_Data.quadVertexArray->AddVertexBuffer(s_Data.quadVertexBuffer);

		s_Data.quadVertexBufferBase = new QuadVertex[Renderer2DData::maxQuadVertices];

		uint32_t* quadIndices{ new uint32_t[Renderer2DData::maxQuadIndices] };
		uint32_t offset{};
		for (size_t idx{}; idx < Renderer2DData::maxQuadIndices; idx += 6)
		{
			quadIndices[idx + 0] = offset + 0;
			quadIndices[idx + 1] = offset + 1;
			quadIndices[idx + 2] = offset + 2;

			quadIndices[idx + 3] = offset + 2;
			quadIndices[idx + 4] = offset + 3;
			quadIndices[idx + 5] = offset + 0;

			offset += 4;
		}
		const Ref<IndexBuffer> quadIB{ IndexBuffer::Create(quadIndices, Renderer2DData::maxQuadIndices) };
		s_Data.quadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		// -------------- CIRCLES ----------------//
		s_Data.circleVertexArray = VertexArray::Create();

		s_Data.circleVertexBuffer = VertexBuffer::Create(Renderer2DData::maxCircleVertices * sizeof(CircleVertex));
		s_Data.circleVertexBuffer->SetLayout(CircleVertex::GetBufferLayout());
		s_Data.circleVertexArray->AddVertexBuffer(s_Data.circleVertexBuffer);
		s_Data.circleVertexArray->SetIndexBuffer(quadIB);

		s_Data.circleVertexBufferBase = new CircleVertex[Renderer2DData::maxCircleVertices];

		// -------------- TEXTURES ----------------//
		// 1x1 white pixel texture
		s_Data.whiteTexture = Texture2D::Create(TextureSpecification{ 1, 1, ImageFormat::RGBA8, TextureParameters{ TextureFilter::Nearest, TextureWrap::ClampToEdge } });
		const uint32_t whiteTextureData{ 0xffffffff };
		s_Data.whiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));
		// Set first texture slot to 0
		s_Data.textureSlots[0] = s_Data.whiteTexture;
		s_Data.whiteTexture->Bind(0);

		// -------------- SHADERS ----------------//
		s_Data.pointShader = Shader::Create("../Engine/assets/shaders/renderer2DPoint.glsl");
		s_Data.lineShader = Shader::Create("../Engine/assets/shaders/renderer2DLine.glsl");
		s_Data.quadShader = Shader::Create("../Engine/assets/shaders/renderer2DQuad.glsl");
		s_Data.circleShader = Shader::Create("../Engine/assets/shaders/renderer2DCircle.glsl");
	}

	void Renderer2D::Shutdown()
	{
		//ENGINE_PROFILE_FUNCTION();
	}

	void Renderer2D::BeginScene(const Camera& camera)
	{
		BeginScene(camera.GetViewProjectionMatrix());
	}

	void Renderer2D::BeginScene(const glm::mat4& viewProjectionMatrix)
	{
		SetShadersViewProjection(viewProjectionMatrix);

		ResetAllBatches();
	}

	void Renderer2D::EndScene()
	{
		ENGINE_PROFILE_FUNCTION();

		FlushAll();
	}

	const glm::vec4& Renderer2D::GetDrawColor()
	{
		return s_Data.drawColor;
	}

	void Renderer2D::SetDrawColor(const glm::vec4& drawColor)
	{
		s_Data.drawColor = drawColor;
	}

	float Renderer2D::GetLineWidth()
	{
		return s_Data.lineWidth;
	}

	void Renderer2D::SetLineWidth(float width)
	{
		s_Data.lineWidth = width;
	}

	float Renderer2D::GetPointSize()
	{
		return s_Data.pointSize;
	}

	void Renderer2D::SetPointSize(float size)
	{
		s_Data.pointSize = size;
	}

	// Points

	void Renderer2D::DrawPoint(const glm::vec2& position)
	{
		DrawPoint(glm::vec3{ position, 0.f });
	}

	void Renderer2D::DrawPoint(const glm::vec3& position)
	{
		if (s_Data.pointVertexCount >= Renderer2DData::maxPointVertices) NextPointBatch();

		s_Data.pointVertexBufferPtr->position = position;
		s_Data.pointVertexBufferPtr->color = s_Data.drawColor;
		s_Data.pointVertexBufferPtr++;

		++s_Data.pointVertexCount;

#if RENDERER2D_STATISTICS
		++s_Data.stats.pointCount;
#endif
	}

	void Renderer2D::DrawPoints(const std::vector<glm::ivec2>& points)
	{
		const uint32_t pointCount{ static_cast<uint32_t>(points.size()) };
		if (s_Data.pointVertexCount + pointCount > Renderer2DData::maxPointVertices) NextPointBatch();

		for (const glm::ivec2& point : points)
		{
			s_Data.pointVertexBufferPtr->position = glm::vec3{ static_cast<glm::vec2>(point), 0.f };
			s_Data.pointVertexBufferPtr->color = s_Data.drawColor;
			s_Data.pointVertexBufferPtr++;
		}

		s_Data.pointVertexCount += pointCount;

#if RENDERER2D_STATISTICS
		s_Data.stats.pointCount += pointCount;
#endif
	}

	void Renderer2D::DrawPoints(const std::vector<glm::ivec3>& points)
	{
		const uint32_t pointCount{ static_cast<uint32_t>(points.size()) };
		if (s_Data.pointVertexCount + pointCount > Renderer2DData::maxPointVertices) NextPointBatch();

		for (const glm::ivec3& point : points)
		{
			s_Data.pointVertexBufferPtr->position = static_cast<glm::vec3>(point);
			s_Data.pointVertexBufferPtr->color = s_Data.drawColor;
			s_Data.pointVertexBufferPtr++;
		}

		s_Data.pointVertexCount += pointCount;

#if RENDERER2D_STATISTICS
		s_Data.stats.pointCount += pointCount;
#endif
	}

	void Renderer2D::DrawPoints(const std::vector<glm::vec2>& points)
	{
		const uint32_t pointCount{ static_cast<uint32_t>(points.size()) };
		if (s_Data.pointVertexCount + pointCount > Renderer2DData::maxPointVertices) NextPointBatch();

		for (const glm::vec2& point : points)
		{
			s_Data.pointVertexBufferPtr->position = glm::vec3{ point, 0.f };
			s_Data.pointVertexBufferPtr->color = s_Data.drawColor;
			s_Data.pointVertexBufferPtr++;
		}

		s_Data.pointVertexCount += pointCount;

#if RENDERER2D_STATISTICS
		s_Data.stats.pointCount += pointCount;
#endif
	}

	void Renderer2D::DrawPoints(const std::vector<glm::vec3>& points)
	{
		const uint32_t pointCount{ static_cast<uint32_t>(points.size()) };
		if (s_Data.pointVertexCount + pointCount > Renderer2DData::maxPointVertices) NextPointBatch();

		for (const glm::vec3& point : points)
		{
			s_Data.pointVertexBufferPtr->position = point;
			s_Data.pointVertexBufferPtr->color = s_Data.drawColor;
			s_Data.pointVertexBufferPtr++;
		}

		s_Data.pointVertexCount += pointCount;

#if RENDERER2D_STATISTICS
		s_Data.stats.pointCount += pointCount;
#endif
	}

	// Lines
	void Renderer2D::DrawLine(const Line2f& line)
	{
		DrawLine(line.pointOne, line.pointTwo);
	}

	void Renderer2D::DrawLine(const Line3f& line)
	{
		DrawLine(line.pointOne, line.pointTwo);
	}
	
	void Renderer2D::DrawLine(const glm::vec2& p0, const glm::vec2& p1)
	{
		DrawLine(glm::vec3{ p0, 0.f }, glm::vec3{ p1, 0.f });
	}

	void Renderer2D::DrawLine(const glm::vec3& p0, const glm::vec3& p1)
	{
		if (s_Data.lineVertexCount >= Renderer2DData::maxLineVertices) NextLineBatch();

		s_Data.lineVertexBufferPtr->position = p0;
		s_Data.lineVertexBufferPtr->color = s_Data.drawColor;
		s_Data.lineVertexBufferPtr++;

		s_Data.lineVertexBufferPtr->position = p1;
		s_Data.lineVertexBufferPtr->color = s_Data.drawColor;
		s_Data.lineVertexBufferPtr++;

		s_Data.lineVertexCount += 2;

#if RENDERER2D_STATISTICS
		++s_Data.stats.lineCount;
#endif
	}

	// Rect
	void Renderer2D::DrawRect(const Rect2f& rect)
	{
		DrawRect(rect.position, rect.size);
	}

	void Renderer2D::DrawRect(const Rect3f& rect)
	{
		DrawRect(rect.position, rect.size);
	}

	void Renderer2D::DrawRect(const glm::vec2& position, const glm::vec2& size)
	{
		DrawRect(glm::vec3{ position.x, position.y, 0.f }, size);
	}

	void Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2& size)
	{
		ENGINE_PROFILE_FUNCTION();

		glm::mat4 modelMatrix{ Renderer2DData::identityMatrix };
		modelMatrix = glm::translate(modelMatrix, position);
		modelMatrix = glm::scale(modelMatrix, glm::vec3{ size, 1.f });

		DrawRect(modelMatrix);
	}

	void Renderer2D::DrawRect(const Rect2f& rect, float radAngle)
	{
		DrawRect(rect.position, rect.size, radAngle);
	}

	void Renderer2D::DrawRect(const Rect3f& rect, float radAngle)
	{
		DrawRect(rect.position, rect.size, radAngle);
	}

	void Renderer2D::DrawRect(const glm::vec2& position, const glm::vec2& size, float radAngle)
	{
		DrawRect(glm::vec3{ position, 0.f }, size, radAngle);
	}

	void Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2& size, float radAngle)
	{
		ENGINE_PROFILE_FUNCTION();

		glm::mat4 modelMatrix{ Renderer2DData::identityMatrix };
		modelMatrix = glm::translate(modelMatrix, position);
		modelMatrix = glm::rotate(modelMatrix, radAngle, Renderer2DData::zAxis);
		modelMatrix = glm::scale(modelMatrix, glm::vec3{ size, 1.f });

		DrawRect(modelMatrix);
	}

	void Renderer2D::DrawRect(const glm::mat4& modelMatrix)
	{
		ENGINE_PROFILE_FUNCTION();

		glm::vec3 lineVertices[4]{};
		for (size_t positionIdx{}; positionIdx < 4; positionIdx++)
		{
			lineVertices[positionIdx] = modelMatrix * s_Data.quadVertexPositions[positionIdx];
		}

		DrawLine(lineVertices[0], lineVertices[1]);
		DrawLine(lineVertices[1], lineVertices[2]);
		DrawLine(lineVertices[2], lineVertices[3]);
		DrawLine(lineVertices[3], lineVertices[0]);
	}

	// Filled Rect
	void Renderer2D::DrawFilledRect(const Rect2f& rect)
	{
		DrawFilledRect(rect.position, rect.size);
	}

	void Renderer2D::DrawFilledRect(const Rect3f& rect)
	{
		DrawFilledRect(rect.position, rect.size);
	}

	void Renderer2D::DrawFilledRect(const glm::vec2& position, const glm::vec2& size)
	{
		DrawFilledRect(glm::vec3{ position.x, position.y, 0.f }, size);
	}

	void Renderer2D::DrawFilledRect(const glm::vec3& position, const glm::vec2& size)
	{
		ENGINE_PROFILE_FUNCTION();

		glm::mat4 modelMatrix{ Renderer2DData::identityMatrix };
		modelMatrix = glm::translate(modelMatrix, position);
		modelMatrix = glm::scale(modelMatrix, glm::vec3{ size, 1.f });

		DrawFilledRect(modelMatrix);
	}

	void Renderer2D::DrawFilledRect(const Rect2f& rect, float radAngle)
	{
		DrawFilledRect(rect.position, rect.size, radAngle);
	}

	void Renderer2D::DrawFilledRect(const Rect3f& rect, float radAngle)
	{
		DrawFilledRect(rect.position, rect.size, radAngle);
	}

	void Renderer2D::DrawFilledRect(const glm::vec2& position, const glm::vec2& size, float radAngle)
	{
		DrawFilledRect(glm::vec3{ position, 0.f }, size, radAngle);
	}

	void Renderer2D::DrawFilledRect(const glm::vec3& position, const glm::vec2& size, float radAngle)
	{
		ENGINE_PROFILE_FUNCTION();

		glm::mat4 modelMatrix{ Renderer2DData::identityMatrix };
		modelMatrix = glm::translate(modelMatrix, position);
		modelMatrix = glm::rotate(modelMatrix, radAngle, Renderer2DData::zAxis);
		modelMatrix = glm::scale(modelMatrix, glm::vec3{ size, 1.f });

		DrawFilledRect(modelMatrix);
	}

	void Renderer2D::DrawFilledRect(const glm::mat4& modelMatrix)
	{
		ENGINE_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount{ 4 };
		constexpr int whitePixelTextureIdx{ 0 };
		constexpr glm::vec2 textureCoords[]{ { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		constexpr float defaultTilingFactor{ 1.f };

		if (s_Data.quadIndexCount >= Renderer2DData::maxQuadIndices) NextQuadBatch();

		for (size_t vertIdx{}; vertIdx < quadVertexCount; ++vertIdx)
		{
			s_Data.quadVertexBufferPtr->position = modelMatrix * s_Data.quadVertexPositions[vertIdx];
			s_Data.quadVertexBufferPtr->color = s_Data.drawColor;
			s_Data.quadVertexBufferPtr->texCoord = textureCoords[vertIdx];
			s_Data.quadVertexBufferPtr->texIndex = whitePixelTextureIdx;
			s_Data.quadVertexBufferPtr->tilingFactor = defaultTilingFactor;
			++s_Data.quadVertexBufferPtr;
		}
		s_Data.quadIndexCount += 6;

#if RENDERER2D_STATISTICS
		++s_Data.stats.quadCount;
#endif
	}

	// Circle
	void Renderer2D::DrawCircle(const Circle2f& circle, float thickness, float fade)
	{
		DrawCircle(circle.center, circle.radius, thickness, fade);
	}

	void Renderer2D::DrawCircle(const Circle3f& circle, float thickness, float fade)
	{
		DrawCircle(circle.center, circle.radius, thickness, fade);
	}

	void Renderer2D::DrawCircle(const glm::vec2& center, float radius, float thickness, float fade)
	{
		DrawCircle(glm::vec3{ center, 0.f }, radius, thickness, fade);
	}

	void Renderer2D::DrawCircle(const glm::vec3& center, float radius, float thickness, float fade)
	{
		glm::mat4 modelMatrix{ Renderer2DData::identityMatrix };
		modelMatrix = glm::translate(modelMatrix, center);
		modelMatrix = glm::scale(modelMatrix, glm::vec3{ radius, radius, 1.f } * 2.f);

		DrawCircle(modelMatrix, thickness, fade);
	}

	void Renderer2D::DrawCircle(const glm::mat4& modelMatrix, float thickness, float fade)
	{
		for (size_t vertexIdx{}; vertexIdx < 4; vertexIdx++)
		{
			s_Data.circleVertexBufferPtr->worldPosition = modelMatrix * s_Data.quadVertexPositions[vertexIdx];
			s_Data.circleVertexBufferPtr->localPosition = s_Data.quadVertexPositions[vertexIdx] * 2.0f;
			s_Data.circleVertexBufferPtr->color = s_Data.drawColor;
			s_Data.circleVertexBufferPtr->thickness = thickness;
			s_Data.circleVertexBufferPtr->fade = fade;
			s_Data.circleVertexBufferPtr++;
		}

		s_Data.circleIndexCount += 6;

#if RENDERER2D_STATISTICS
		++s_Data.stats.circleCount;
#endif
	}

	// Filled Circle
	void Renderer2D::DrawFilledCircle(const Circle2f& circle, float fade)
	{
		DrawCircle(circle, 1.f, fade);
	}

	void Renderer2D::DrawFilledCircle(const Circle3f& circle, float fade)
	{
		DrawCircle(circle, 1.f, fade);
	}

	void Renderer2D::DrawFilledCircle(const glm::vec2& center, float radius, float fade)
	{
		DrawCircle(center, radius, 1.f, fade);
	}

	void Renderer2D::DrawFilledCircle(const glm::vec3& center, float radius, float fade)
	{
		DrawCircle(center, radius, 1.f, fade);
	}

	void Renderer2D::DrawFilledCircle(const glm::mat4& modelMatrix, float fade)
	{
		DrawCircle(modelMatrix, 1.f, fade);
	}

	// Texture
	void Renderer2D::DrawTexture(const Ref<Texture2D>& texture, const Rect2f& rect, const glm::vec4& tintColor, float tilingFactor)
	{
		DrawTexture(texture, rect.position, rect.size, tintColor, tilingFactor);
	}

	void Renderer2D::DrawTexture(const Ref<Texture2D>& texture, const Rect3f& rect, const glm::vec4& tintColor, float tilingFactor)
	{
		DrawTexture(texture, rect.position, rect.size, tintColor, tilingFactor);
	}

	void Renderer2D::DrawTexture(const Ref<Texture2D>& texture, const glm::vec2& position, const glm::vec4& tintColor, float tilingFactor)
	{
		DrawTexture(texture, glm::vec3{ position, 0.f }, tintColor, tilingFactor);
	}

	void Renderer2D::DrawTexture(const Ref<Texture2D>& texture, const glm::vec3& position, const glm::vec4& tintColor, float tilingFactor)
	{
		DrawTexture(texture, position, glm::vec2{ static_cast<float>(texture->GetWidth()), static_cast<float>(texture->GetHeight()) }, tintColor, tilingFactor);
	}

	void Renderer2D::DrawTexture(const Ref<Texture2D>& texture, const glm::vec2& position, const glm::vec2& size, const glm::vec4& tintColor, float tilingFactor)
	{
		DrawTexture(texture, glm::vec3{ position, 0.f }, glm::vec2{ static_cast<float>(texture->GetWidth()), static_cast<float>(texture->GetHeight()) }, tintColor, tilingFactor);
	}

	void Renderer2D::DrawTexture(const Ref<Texture2D>& texture, const glm::vec3& position, const glm::vec2& size, const glm::vec4& tintColor, float tilingFactor)
	{
		ENGINE_PROFILE_FUNCTION();

		glm::mat4 modelMatrix{ Renderer2DData::identityMatrix };
		modelMatrix = glm::translate(modelMatrix, position);
		modelMatrix = glm::scale(modelMatrix, glm::vec3{ size, 1.f });

		DrawTexture(texture, modelMatrix, tintColor, tilingFactor);
	}

	void Renderer2D::DrawTexture(const Ref<Texture2D>& texture, const Rect2f& rect, float radAngle, const glm::vec4& tintColor, float tilingFactor)
	{
		DrawTexture(texture, rect.position, rect.size, radAngle, tintColor, tilingFactor);
	}

	void Renderer2D::DrawTexture(const Ref<Texture2D>& texture, const Rect3f& rect, float radAngle, const glm::vec4& tintColor, float tilingFactor)
	{
		DrawTexture(texture, rect.position, rect.size, radAngle, tintColor, tilingFactor);
	}

	void Renderer2D::DrawTexture(const Ref<Texture2D>& texture, const glm::vec2& position, float radAngle, const glm::vec4& tintColor, float tilingFactor)
	{
		DrawTexture(texture, glm::vec3{ position, 0.f }, radAngle, tintColor, tilingFactor);
	}

	void Renderer2D::DrawTexture(const Ref<Texture2D>& texture, const glm::vec3& position, float radAngle, const glm::vec4& tintColor, float tilingFactor)
	{
		DrawTexture(texture, position, glm::vec2{ static_cast<float>(texture->GetWidth()), static_cast<float>(texture->GetHeight()) }, radAngle, tintColor, tilingFactor);
	}

	void Renderer2D::DrawTexture(const Ref<Texture2D>& texture, const glm::vec2& position, const glm::vec2& size, float radAngle, const glm::vec4& tintColor, float tilingFactor)
	{
		DrawTexture(texture, glm::vec3{ position, 0.f }, glm::vec2{ static_cast<float>(texture->GetWidth()), static_cast<float>(texture->GetHeight()) }, radAngle, tintColor, tilingFactor);
	}

	void Renderer2D::DrawTexture(const Ref<Texture2D>& texture, const glm::vec3& position, const glm::vec2& size, float radAngle, const glm::vec4& tintColor, float tilingFactor)
	{
		ENGINE_PROFILE_FUNCTION();

		glm::mat4 modelMatrix{ Renderer2DData::identityMatrix };
		modelMatrix = glm::translate(modelMatrix, position);
		modelMatrix = glm::rotate(modelMatrix, radAngle, Renderer2DData::zAxis);
		modelMatrix = glm::scale(modelMatrix, glm::vec3{ size, 1.f });

		DrawTexture(texture, modelMatrix, tintColor, tilingFactor);
	}

	void Renderer2D::DrawTexture(const Ref<Texture2D>& texture, const glm::mat4& modelMatrix, const glm::vec4& tintColor, float tilingFactor)
	{
		ENGINE_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount{ 4 };
		constexpr glm::vec2 textureCoords[]{ { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		uint32_t textureIdx{};
		for (uint32_t idx{}; idx < s_Data.textureSlotIndex; idx++)
		{
			if (*s_Data.textureSlots[idx] == *texture)
			{
				textureIdx = idx;
				break;
			}
		}

		if (!textureIdx)
		{
			if (s_Data.textureSlotIndex >= Renderer2DData::maxTextureSlots) NextQuadBatch();

			ENGINE_CORE_ASSERT(s_Data.textureSlotIndex < Renderer2DData::maxTextureSlots, "textureIdx to high");
			ENGINE_CORE_ASSERT(s_Data.textureSlotIndex >= 0, "textureIdx to low");

			textureIdx = s_Data.textureSlotIndex;
			s_Data.textureSlots[s_Data.textureSlotIndex] = texture;
			s_Data.textureSlotIndex++;
		}

		if (s_Data.quadIndexCount >= Renderer2DData::maxQuadIndices) NextQuadBatch();

		for (size_t vertIdx{}; vertIdx < quadVertexCount; ++vertIdx)
		{
			s_Data.quadVertexBufferPtr->position = modelMatrix * s_Data.quadVertexPositions[vertIdx];
			s_Data.quadVertexBufferPtr->color = tintColor;
			s_Data.quadVertexBufferPtr->texCoord = textureCoords[vertIdx];
			s_Data.quadVertexBufferPtr->texIndex = textureIdx;
			s_Data.quadVertexBufferPtr->tilingFactor = tilingFactor;
			++s_Data.quadVertexBufferPtr;
		}
		s_Data.quadIndexCount += 6;

#if RENDERER2D_STATISTICS
		++s_Data.stats.quadCount;
#endif
	}

	// Stats
	void Renderer2D::ResetStats()
	{
#if RENDERER2D_STATISTICS
		memset(&s_Data.stats, 0, sizeof(Statistics));
#endif
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
#if RENDERER2D_STATISTICS
		return s_Data.stats;
#else
		return Renderer2D::Statistics{};
#endif
	}

	// Camera Shaders
	void Renderer2D::SetShadersViewProjection(const glm::mat4& viewProj)
	{
		s_Data.pointShader->Bind();
		s_Data.pointShader->SetMat4("u_ViewProjectionMatrix", viewProj);

		s_Data.lineShader->Bind();
		s_Data.lineShader->SetMat4("u_ViewProjectionMatrix", viewProj);

		s_Data.quadShader->Bind();
		s_Data.quadShader->SetMat4("u_ViewProjectionMatrix", viewProj);

		s_Data.circleShader->Bind();
		s_Data.circleShader->SetMat4("u_ViewProjectionMatrix", viewProj);
	}

	// Next Batches
	void Renderer2D::NextPointBatch()
	{
		FlushPoints();
		ResetPointBatch();
	}

	void Renderer2D::NextLineBatch()
	{
		FlushLines();
		ResetLineBatch();
	}

	void Renderer2D::NextQuadBatch()
	{
		FlushQuads();
		ResetQuadBatch();
		ResetTextureBatch();
	}

	void Renderer2D::NextCircleBatch()
	{
		FlushCircles();
		ResetCircleBatch();
	}

	// Reset Batches
	void Renderer2D::ResetAllBatches()
	{
		ResetPointBatch();
		ResetLineBatch();
		ResetQuadBatch();
		ResetCircleBatch();
		ResetTextureBatch();
	}

	void Renderer2D::ResetPointBatch()
	{
		s_Data.pointVertexCount = 0;
		s_Data.pointVertexBufferPtr = s_Data.pointVertexBufferBase;
	}

	void Renderer2D::ResetLineBatch()
	{
		s_Data.lineVertexCount = 0;
		s_Data.lineVertexBufferPtr = s_Data.lineVertexBufferBase;
	}

	void Renderer2D::ResetQuadBatch()
	{
		s_Data.quadIndexCount = 0;
		s_Data.quadVertexBufferPtr = s_Data.quadVertexBufferBase;
	}

	void Renderer2D::ResetCircleBatch()
	{
		s_Data.circleIndexCount = 0;
		s_Data.circleVertexBufferPtr = s_Data.circleVertexBufferBase;
	}

	void Renderer2D::ResetTextureBatch()
	{
		s_Data.textureSlotIndex = 1;
	}
	
	// Flush Batches
	void Renderer2D::FlushAll()
	{
		FlushPoints();
		FlushLines();
		FlushQuads();
		FlushCircles();
	}

	void Renderer2D::FlushPoints()
	{
		if (s_Data.pointVertexCount)
		{
			const uint32_t dataSize
			{
				static_cast<uint32_t>(
				reinterpret_cast<uint8_t*>(s_Data.pointVertexBufferPtr) -
				reinterpret_cast<uint8_t*>(s_Data.pointVertexBufferBase))
			};
			s_Data.pointVertexBuffer->SetData(s_Data.pointVertexBufferBase, dataSize);

			s_Data.pointShader->Bind();
			RenderCommand::SetPointSize(s_Data.pointSize);
			RenderCommand::DrawVertices(s_Data.pointVertexArray, s_Data.pointVertexCount, RenderMode::Points);

#if RENDERER2D_STATISTICS
			++s_Data.stats.drawCalls;
#endif
		}
	}

	void Renderer2D::FlushLines()
	{
		if (s_Data.lineVertexCount)
		{
			const uint32_t dataSize
			{
				static_cast<uint32_t>(
				reinterpret_cast<uint8_t*>(s_Data.lineVertexBufferPtr) -
				reinterpret_cast<uint8_t*>(s_Data.lineVertexBufferBase))
			};
			s_Data.lineVertexBuffer->SetData(s_Data.lineVertexBufferBase, dataSize);

			s_Data.lineShader->Bind();
			RenderCommand::SetLineWidth(s_Data.lineWidth);
			RenderCommand::DrawVertices(s_Data.lineVertexArray, s_Data.lineVertexCount);
#if RENDERER2D_STATISTICS
			++s_Data.stats.drawCalls;
#endif
		}
	}

	void Renderer2D::FlushQuads()
	{
		if (s_Data.quadIndexCount)
		{
			// Set Data
			const uint32_t dataSize
			{
				static_cast<uint32_t>(
				reinterpret_cast<uint8_t*>(s_Data.quadVertexBufferPtr) -
				reinterpret_cast<uint8_t*>(s_Data.quadVertexBufferBase))
			};
			s_Data.quadVertexBuffer->SetData(s_Data.quadVertexBufferBase, dataSize);

			// Bindings (idx 0 already binded for 1x1 white pixel texture)
			for (uint32_t textureSlotIdx{ 1 }; textureSlotIdx < s_Data.textureSlotIndex; ++textureSlotIdx)
			{
				s_Data.textureSlots[textureSlotIdx]->Bind(textureSlotIdx);
			}
			s_Data.quadShader->Bind();

			// Draw
			RenderCommand::DrawIndexed(s_Data.quadVertexArray, s_Data.quadIndexCount);
#if RENDERER2D_STATISTICS
			++s_Data.stats.drawCalls;
#endif
		}
	}

	void Renderer2D::FlushCircles()
	{
		if (s_Data.circleIndexCount)
		{
			const uint32_t dataSize
			{
				static_cast<uint32_t>(
				reinterpret_cast<uint8_t*>(s_Data.circleVertexBufferPtr) -
				reinterpret_cast<uint8_t*>(s_Data.circleVertexBufferBase))
			};
			s_Data.circleVertexBuffer->SetData(s_Data.circleVertexBufferBase, dataSize);

			s_Data.circleShader->Bind();
			RenderCommand::DrawIndexed(s_Data.circleVertexArray, s_Data.circleIndexCount);

#if RENDERER2D_STATISTICS
			++s_Data.stats.drawCalls;
#endif
		}
	}

}