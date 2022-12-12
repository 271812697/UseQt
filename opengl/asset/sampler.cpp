#include "../pch.h"

#include <type_traits>
#include "sampler.h"

namespace asset {
    inline QOpenGLFunctions_4_5_Core** gl=getGlInstanceDPtr();
    Sampler::Sampler(FilterMode mode) : IAsset() {
        (*gl)->glCreateSamplers(1, &id);

        static const float border[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        this->SetParam(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        this->SetParam(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        this->SetParam(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
        this->SetParam(GL_TEXTURE_BORDER_COLOR, border);

        // this list of preset samplers should meet our needs most of the time, if we have
        // special needs, we can customize the sampling state later using `SetParam()`

        switch (mode) {
            case FilterMode::Bilinear: {
                this->SetParam(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                this->SetParam(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                break;
            }
            case FilterMode::Trilinear: {
                this->SetParam(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                this->SetParam(GL_TEXTURE_MAG_FILTER, GL_LINEAR);  // magnification doesn't use mipmaps
                break;
            }
            case FilterMode::Point:
            default: {  // a point sampler will be created by default
                this->SetParam(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                this->SetParam(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                break;
            }
        }
    }

    Sampler::~Sampler() {
        (*gl)->glDeleteSamplers(1, &id);
    }

    void Sampler::Bind(GLuint index) const {
        (*gl)->glBindSampler(index, id);
    }

    void Sampler::Unbind(GLuint index) const {
        (*gl)->glBindSampler(index, 0);
    }

    template<typename T>
    void Sampler::SetParam(GLenum name, T value) const {
        if constexpr (std::is_same_v<T, int>) {
            (*gl)->glSamplerParameteri(id, name, value);
        }
        else if constexpr (std::is_same_v<T, float>) {
            (*gl)->glSamplerParameterf(id, name, value);
        }
    }

    template<typename T>
    void Sampler::SetParam(GLenum name, const T* value) const {
        if constexpr (std::is_same_v<T, int>) {
            (*gl)->glSamplerParameteriv(id, name, value);
        }
        else if constexpr (std::is_same_v<T, float>) {
            (*gl)->glSamplerParameterfv(id, name, value);
        }
    }

    // explicit template function instantiation using X macro
    #define INSTANTIATE_TEMPLATE(T) \
        template void Sampler::SetParam<T>(GLenum name, T value) const; \
        template void Sampler::SetParam<T>(GLenum name, const T* value) const;

    INSTANTIATE_TEMPLATE(int)
    INSTANTIATE_TEMPLATE(float)

    #undef INSTANTIATE_TEMPLATE

}
