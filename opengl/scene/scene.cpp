#include "pch.h"
#include "scene.h"
#include "renderer.h"
#include "../util/path.h"
using namespace asset;
using namespace component;
namespace scene {
    inline QOpenGLFunctions_4_5_Core** gl=getGlInstanceDPtr();
    Scene::Scene(const std::string& title) : title(title), directory() {
        this->resource_manager = ResourceManager();
    }

    Scene::~Scene() {
        registry.each([this](auto id) {
            CORE_TRACE("Destroying entity: {0}", directory.at(id));
        });

        registry.clear();
    }

    Entity Scene::CreateEntity(const std::string& name, ETag tag) {
        Entity e = { name, registry.create(), &registry };
        // every entity has a transform and a tag component
        e.AddComponent<Transform>();
        e.AddComponent<Tag>(tag);
        directory.emplace(e.id, e.name);
        return e;
    }
    void Scene::DestroyEntity(Entity e) {
        CORE_TRACE("Destroying entity: {0}", e.name);
        directory.erase(e.id);
        registry.destroy(e.id);
    }
    void Scene::AddUBO(GLuint shader_id) {
        const GLenum props[] = { GL_BUFFER_BINDING };
        GLint n_blocks = 0;
        (*gl)->glGetProgramInterfaceiv(shader_id, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &n_blocks);
        for (int idx = 0; idx < n_blocks; ++idx) {
            GLint binding_index = 0;
            (*gl)->glGetProgramResourceiv(shader_id, GL_UNIFORM_BLOCK, idx, 1, props, 1, NULL, &binding_index);
            GLuint binding_point = static_cast<GLuint>(binding_index);
            // uniform blocks >= 10 are reserved for internal use only
            if (binding_point < 10) {
                UBOs.try_emplace(binding_point, shader_id, idx);  // construct UBO in-place
            }
        }
    }
    void Scene::AddFBO(GLuint width, GLuint height) {
        GLuint key = FBOs.size();
        FBOs.try_emplace(key, width, height);  // construct FBO in-place
    }

    // this base class is being used to render our welcome screen
    void Scene::Init() {

    }

    void Scene::OnSceneRender() {
        Renderer::Clear();
    }

    void Scene::OnImGuiRender() {

    }

}
