#ifndef MODEL_H
#define MODEL_H

#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/VertexArray.h"

namespace Engine
{
	class Model final
	{
	public:

		explicit Model(Ref<Shader> shader, Ref<VertexArray> vertexArray);
		~Model() = default;

		void Submit(const glm::mat4& viewprojectionMatrix);

		const Ref<Shader>& GetShader() const;
		void SetShader(Ref<Shader> newShader);
		void SetVertexArray(Ref<VertexArray> newVertexArray);

		const glm::vec3& GetPosition() const;
		void SetPosition(const glm::vec3& newPosition);
		void Translate(const glm::vec3 translation);

		const glm::vec3& GetRotation() const;
		void SetRotation(const glm::vec3& newRotation);
		void Rotate(const glm::vec3& rotation);

		const glm::vec3& GetScale() const;
		void SetScale(const glm::vec3& newScale);
		void Scale(const glm::vec3& scale);

	private:

		void UpdateModelMatrix();

	private:

		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		glm::vec3 m_Scale;
		glm::mat4 m_ModelMatrix;

		std::shared_ptr<Shader> m_ModelShader;
		std::shared_ptr<VertexArray> m_ModelVertexArray;

	};
}

#endif // !MODEL_H
