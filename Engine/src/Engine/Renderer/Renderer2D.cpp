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

#include <iostream>

namespace Engine
{
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
	};

	struct LineVertex
	{
		glm::vec3 position;
		glm::vec4 color;
	};

	struct Renderer2DData
	{
		Renderer2DData() = default;

		// Max Values
		static constexpr uint32_t maxQuads{ 10000 };
		static constexpr uint32_t maxVertices{ maxQuads * 4 };
		static constexpr uint32_t maxIndices{ maxQuads * 6 };
		static constexpr uint32_t maxTextureSlots{ 32 };

		// Basics
		glm::vec4 drawColor{ 1.f, 1.f, 1.f, 1.f };
		float lineWidth{ 2.f };

		// Quads
		std::array<glm::vec4, 4> quadVertexPositions{};
		Ref<VertexArray> quadVertexArray{ nullptr };
		Ref<VertexBuffer> quadVertexBuffer{ nullptr };
		Ref<Shader> quadShader{ nullptr };
		uint32_t quadIndexCount{ 0 };
		QuadVertex* quadVertexBufferBase{ nullptr };
		QuadVertex* quadVertexBufferPtr{ nullptr };

		// Textures
		Ref<Texture2D> whiteTexture{ nullptr };
		std::array<Ref<Texture2D>, maxTextureSlots> textureSlots{};
		uint32_t textureSlotIndex{ 1 }; // 0 is white 1x1 pixel texture

		// Stats
		Renderer2D::Statistics stats{};
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		ENGINE_PROFILE_FUNCTION();

		// Basics
		s_Data.drawColor = Color::white;
		s_Data.lineWidth = 2.f;

		// Buffers
		s_Data.quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.quadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.quadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.quadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		s_Data.quadVertexArray = VertexArray::Create();

		s_Data.quadVertexBuffer = VertexBuffer::Create(Renderer2DData::maxVertices * sizeof(QuadVertex));
		s_Data.quadVertexBuffer->SetLayout(QuadVertex::GetBufferLayout());
		s_Data.quadVertexArray->AddVertexBuffer(s_Data.quadVertexBuffer);

		s_Data.quadVertexBufferBase = new QuadVertex[Renderer2DData::maxVertices];

		uint32_t* quadIndices{ new uint32_t[Renderer2DData::maxIndices] };
		ENGINE_CORE_ASSERT(Renderer2DData::maxIndices % 6 == 0, "maxIndices need to be dividable by 6");
		uint32_t offset{};
		for (uint32_t idx{}; idx < s_Data.maxIndices; idx += 6)
		{
			quadIndices[idx + 0] = offset + 0;
			quadIndices[idx + 1] = offset + 1;
			quadIndices[idx + 2] = offset + 2;

			quadIndices[idx + 3] = offset + 2;
			quadIndices[idx + 4] = offset + 3;
			quadIndices[idx + 5] = offset + 0;

			offset += 4;
		}
		const Ref<IndexBuffer> quadIB{ IndexBuffer::Create(quadIndices, Renderer2DData::maxIndices) };
		s_Data.quadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		// Textures //
		// 1x1 white pixel texture
		s_Data.whiteTexture = Texture2D::Create(TextureSpecification{ 1, 1, ImageFormat::RGBA8, TextureParameters{ TextureFilter::Nearest, TextureWrap::ClampToEdge } });
		const uint32_t whiteTextureData{ 0xffffffff };
		s_Data.whiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));

		// Shader Creation
		s_Data.quadShader = Shader::Create("assets/shaders/renderer2DQuad.glsl");

		// Set first texture slot to 0
		s_Data.textureSlots[0] = s_Data.whiteTexture;
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
		s_Data.quadShader->Bind();
		s_Data.quadShader->SetMat4("u_ViewProjectionMatrix", viewProjectionMatrix);

		StartBach();
	}

	void Renderer2D::EndScene()
	{
		ENGINE_PROFILE_FUNCTION();

		Flush();
	}

	void Renderer2D::Flush()
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

			// Bindings
			for (uint32_t textureSlotIdx{}; textureSlotIdx < s_Data.textureSlotIndex; ++textureSlotIdx)
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

	// Rect
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

		const glm::mat4 transform{ glm::translate(glm::mat4{ 1.f }, position) };
		const glm::mat4 scale{ glm::scale(glm::mat4{ 1.f }, glm::vec3{ size, 1.f }) };
		const glm::mat4 modelMatrix{ transform * scale };

		DrawFilledRect(modelMatrix);
	}

	void Renderer2D::DrawFilledRect(const Rect2f& rect, float rotationAngle)
	{
		DrawFilledRect(rect.position, rect.size, rotationAngle);
	}

	void Renderer2D::DrawFilledRect(const Rect3f& rect, float rotationAngle)
	{
		DrawFilledRect(rect.position, rect.size, rotationAngle);
	}

	void Renderer2D::DrawFilledRect(const glm::vec2& position, const glm::vec2& size, float rotationAngle)
	{
		DrawFilledRect(glm::vec3{ position, 0.f }, size, rotationAngle);
	}

	void Renderer2D::DrawFilledRect(const glm::vec3& position, const glm::vec2& size, float rotationAngle)
	{
		ENGINE_PROFILE_FUNCTION();

		const glm::mat4 transform{ glm::translate(glm::mat4{ 1.f }, position) };
		const glm::mat4 rotation{ glm::rotate(glm::mat4{ 1.0f }, glm::radians(rotationAngle), { 0.0f, 0.0f, 1.0f }) };
		const glm::mat4 scale{ glm::scale(glm::mat4{ 1.f }, glm::vec3{ size, 1.f }) };
		const glm::mat4 modelMatrix{ transform * rotation * scale };

		DrawFilledRect(modelMatrix);
	}

	void Renderer2D::DrawFilledRect(const glm::mat4& modelMatrix)
	{
		ENGINE_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount{ 4 };
		constexpr int whitePixelTextureIdx{ 0 };
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		constexpr float defaultTilingFactor = 1.0f;

		if (s_Data.quadIndexCount >= Renderer2DData::maxIndices) NextBatch();

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
		const glm::mat4 transform{ glm::translate(glm::mat4{ 1.f }, position) };
		const glm::mat4 scale{ glm::scale(glm::mat4{ 1.f }, glm::vec3{ size, 1.f }) };
		const glm::mat4 modelMatrix{ transform * scale };

		DrawTexture(texture, modelMatrix, tintColor, tilingFactor);
	}

	void Renderer2D::DrawTexture(const Ref<Texture2D>& texture, const Rect2f& rect, float rotationAngle, const glm::vec4& tintColor, float tilingFactor)
	{
		DrawTexture(texture, rect.position, rect.size, rotationAngle, tintColor, tilingFactor);
	}

	void Renderer2D::DrawTexture(const Ref<Texture2D>& texture, const Rect3f& rect, float rotationAngle, const glm::vec4& tintColor, float tilingFactor)
	{
		DrawTexture(texture, rect.position, rect.size, rotationAngle, tintColor, tilingFactor);
	}

	void Renderer2D::DrawTexture(const Ref<Texture2D>& texture, const glm::vec2& position, float rotationAngle, const glm::vec4& tintColor, float tilingFactor)
	{
		DrawTexture(texture, glm::vec3{ position, 0.f }, rotationAngle, tintColor, tilingFactor);
	}

	void Renderer2D::DrawTexture(const Ref<Texture2D>& texture, const glm::vec3& position, float rotationAngle, const glm::vec4& tintColor, float tilingFactor)
	{
		DrawTexture(texture, position, glm::vec2{ static_cast<float>(texture->GetWidth()), static_cast<float>(texture->GetHeight()) }, rotationAngle, tintColor, tilingFactor);
	}

	void Renderer2D::DrawTexture(const Ref<Texture2D>& texture, const glm::vec2& position, const glm::vec2& size, float rotationAngle, const glm::vec4& tintColor, float tilingFactor)
	{
		DrawTexture(texture, glm::vec3{ position, 0.f }, glm::vec2{ static_cast<float>(texture->GetWidth()), static_cast<float>(texture->GetHeight()) }, rotationAngle, tintColor, tilingFactor);
	}

	void Renderer2D::DrawTexture(const Ref<Texture2D>& texture, const glm::vec3& position, const glm::vec2& size, float rotationAngle, const glm::vec4& tintColor, float tilingFactor)
	{
		const glm::mat4 transform{ glm::translate(glm::mat4{ 1.f }, position) };
		const glm::mat4 rotation{ glm::rotate(glm::mat4{ 1.f }, glm::radians(rotationAngle), {0.0f, 0.0f, 1.0f})};
		const glm::mat4 scale{ glm::scale(glm::mat4{ 1.f }, glm::vec3{ size, 1.f }) };
		const glm::mat4 modelMatrix{ transform * rotation * scale };

		DrawTexture(texture, modelMatrix, tintColor, tilingFactor);
	}

	void Renderer2D::DrawTexture(const Ref<Texture2D>& texture, const glm::mat4& modelMatrix, const glm::vec4& tintColor, float tilingFactor)
	{
		ENGINE_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount{ 4 };
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		uint32_t textureIdx{};
		for (uint32_t i{}; i < s_Data.textureSlotIndex; i++)
		{
			if (*s_Data.textureSlots[i] == *texture)
			{
				textureIdx = i; break;
			}
		}

		if (!textureIdx)
		{
			if (s_Data.textureSlotIndex >= Renderer2DData::maxTextureSlots) NextBatch();

			ENGINE_CORE_ASSERT(s_Data.textureSlotIndex < Renderer2DData::maxTextureSlots, "textureIdx to high");
			ENGINE_CORE_ASSERT(s_Data.textureSlotIndex >= 0, "textureIdx to low");

			textureIdx = s_Data.textureSlotIndex;
			s_Data.textureSlots[s_Data.textureSlotIndex] = texture;
			s_Data.textureSlotIndex++;
		}

		if (s_Data.quadIndexCount >= Renderer2DData::maxIndices) NextBatch();

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

	void Renderer2D::StartBach()
	{
		s_Data.quadIndexCount = 0;
		s_Data.quadVertexBufferPtr = s_Data.quadVertexBufferBase;

		/*s_Data.circleIndexCount = 0;
		s_Data.circleVertexBufferPtr = s_Data.circleVertexBufferBase;

		s_Data.lineVertexCount = 0;
		s_Data.lineVertexBufferPtr = s_Data.lineVertexBufferBase;*/

		s_Data.textureSlotIndex = 1;
	}

	void Renderer2D::NextBatch()
	{
		ENGINE_PROFILE_FUNCTION();

		Flush();
		StartBach();
	}

}