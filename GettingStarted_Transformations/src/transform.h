#ifndef TRANSFORM_H_08182020
#define TRANSFORM_H_08182020

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "glm/gtc/type_ptr.hpp"

class Transform
{
public:
    explicit Transform(const glm::vec3& position = glm::vec3{},
                       const glm::vec3& rotation = glm::vec3{},
                       const glm::vec3& scale = glm::vec3{1.0f,1.0f,1.0f})
        : m_position{position}, m_rotation{rotation}, m_scale{scale}
    {}

    inline glm::mat4 GetModel() const
    {
        glm::mat4 positionMatrix{glm::translate(m_position)};
        glm::mat4 rotationXMatrix{glm::rotate(m_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f))};
        glm::mat4 rotationYMatrix{glm::rotate(m_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f))};
        glm::mat4 rotationZMatrix{glm::rotate(m_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f))};
        glm::mat4 scaleMatrix{glm::scale(m_scale)};

        glm::mat4 rotationMatrix{rotationZMatrix * rotationYMatrix * rotationXMatrix};

        return positionMatrix * rotationMatrix * scaleMatrix;
    }

    inline const glm::vec3& GetPosition() const {return m_position; }
    inline const glm::vec3& GetRotation() const { return m_rotation; }
    inline const glm::vec3& GetScale() const { return m_scale; }

    inline void SetPosition(const glm::vec3& position) { m_position = position; }
    inline void SetRotation(const glm::vec3& rotation) { m_rotation = rotation; }
    inline void SetScale(const glm::vec3& scale) { m_scale = scale; }

private:
    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;
};

#endif // !TRANSFORM_H_08182020
