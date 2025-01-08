#ifndef ENGINEMATH_H
#define ENGINEMATH_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace Engine::Math
{
    glm::vec2 Lerp(const glm::vec2& start, const glm::vec2& end, float t); // Linear Interpolation
    glm::vec3 Lerp(const glm::vec3& start, const glm::vec3& end, float t);
    
    glm::vec2 Bezier(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, float t); // Bezier Interpolation
    glm::vec3 Bezier(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, float t);
    glm::vec3 Bezier(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, float t);
}

#endif //!ENGINEMATH_H