
#pragma once
#include "asset.h"
namespace asset {

    class VAO : public IAsset {
      public:
        VAO();
        ~VAO();

        VAO(const VAO&) = delete;
        VAO& operator=(const VAO&) = delete;
        VAO(VAO&& other) noexcept = default;
        VAO& operator=(VAO&& other) noexcept = default;

        void Bind() const override;
        void Unbind() const override;

        void SetVBO(GLuint vbo, GLuint attr_id, GLint offset, GLint size, GLint stride, GLenum type) const;
        void SetIBO(GLuint ibo) const;
        void Draw(GLenum mode, GLsizei count);
    };

}
