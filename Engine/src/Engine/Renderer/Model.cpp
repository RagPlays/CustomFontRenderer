#include "enginepch.h"
#include "Model.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Renderer.h"

namespace Engine
{
	Model::Model(Ref<Shader> shader, Ref<VertexArray> vertexArray)
		: m_Position{}
		, m_Rotation{}
		, m_Scale{ 1.f, 1.f, 1.f }
		, m_ModelMatrix{ glm::mat4{ 1.f} }
		, m_ModelShader{ shader }
		, m_ModelVertexArray{ vertexArray }
	{
		if (!m_ModelShader->UniformExists("u_ViewProjectionMatrix"))
		{
			ENGINE_CORE_ERROR("Shader \"{0}\", has no valid ViewProjectionMatrix called \"u_ViewProjectionMatrix\"", m_ModelShader->GetName());
			ENGINE_CORE_ASSERT(false, "Shader is not valid.");
		}
		if (!m_ModelShader->UniformExists("u_ModelMatrix"))
		{
			ENGINE_CORE_ERROR("Shader \"{0}\", has no valid ModelMatrix \"u_ModelMatrix\"", m_ModelShader->GetName());
			ENGINE_CORE_ASSERT(false, "Shader is not valid.");
		}
	}

	void Model::Submit(const glm::mat4& viewprojectionMatrix)
	{
		UpdateModelMatrix();

		m_ModelShader->Bind();
		m_ModelShader->SetMat4("u_ViewProjectionMatrix", viewprojectionMatrix);
		m_ModelShader->SetMat4("u_ModelMatrix", m_ModelMatrix);

		Renderer::Submit(m_ModelShader, m_ModelVertexArray);
	}

	const Ref<Shader>& Model::GetShader() const
	{
		return m_ModelShader;
	}

	void Model::SetShader(Ref<Shader> newShader)
	{
		if (newShader->UniformExists("u_ViewProjectionMatrix") && newShader->UniformExists("u_ModelMatrix"))
		{
			m_ModelShader = newShader;
			return;
		}
		ENGINE_CORE_ERROR("Shader \"{0}\", has not valid ViewProjectionMatrix or ModelMatrix", m_ModelShader->GetName());
		ENGINE_CORE_ASSERT(false, "Shader is not valid.");
	}

	void Model::SetVertexArray(Ref<VertexArray> newVertexArray)
	{
		m_ModelVertexArray = newVertexArray;
	}

	const glm::vec3& Model::GetPosition() const
	{
		return m_Position;
	}

	void Model::SetPosition(const glm::vec3& newPosition)
	{
		m_Position = newPosition;
	}

	void Model::Translate(const glm::vec3 translation)
	{
		m_Position += translation;
	}

	const glm::vec3& Model::GetRotation() const
	{
		return m_Rotation;
	}

	void Model::SetRotation(const glm::vec3& newRotation)
	{
		m_Rotation = newRotation;
	}

	void Model::Rotate(const glm::vec3& rotation)
	{
		m_Rotation += rotation;
	}

	const glm::vec3& Model::GetScale() const
	{
		return m_Scale;
	}

	void Model::SetScale(const glm::vec3& newScale)
	{
		m_Scale = newScale;
	}

	void Model::Scale(const glm::vec3& scale)
	{
		m_Scale += scale;
	}

	void Model::UpdateModelMatrix()
	{
		const glm::mat4 transform{ glm::translate(glm::mat4{ 1.f }, m_Position) };
		const glm::mat4 rotation{ glm::mat4_cast(glm::quat{ glm::radians(m_Rotation) }) };
		const glm::mat4 scale{ glm::scale(glm::mat4{ 1.f }, m_Scale) };

		m_ModelMatrix = transform * rotation * scale;
	}
}