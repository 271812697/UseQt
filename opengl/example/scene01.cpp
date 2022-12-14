#include "scene01.h"
#include"../scene/renderer.h"
#include"clock.h"
namespace scene {
using namespace asset;
using namespace component;
using namespace utils;
void scene01::Init()
{
    Renderer::SetScene(this);
    resource_manager.Add(01,MakeAsset<Shader>("E:/C++/UseQt/opengl/res/es.glsl"));
    resource_manager.Add(02, MakeAsset<Material>(resource_manager.Get<Shader>(01)));
    sphere=CreateEntity("sphere");
    sphere.AddComponent<Mesh>(Primitive::Sphere);
    if(auto& mat=sphere.AddComponent<Material>(resource_manager.Get<Material>(02));true){
        mat.SetTexture(1, MakeAsset<Texture>("E:/C++/UseQt/opengl/res/earth.png"));

    }
    camera=CreateEntity("camera");
    camera.AddComponent<Camera>(View::Perspective);

    //Renderer::MSAA(true);
    Renderer::DepthTest(true);
    Renderer::AlphaBlend(true);
    Renderer::FaceCulling(false);
}

void scene01::OnSceneRender()
{
    Renderer::Clear();
    Renderer::Submit(sphere.id);
    Renderer::Render();

}

void scene01::OnImGuiRender()
{
    auto& c=camera.GetComponent<Camera>();
    c.Update(Clock::delta_time);
    if(auto& mat=sphere.GetComponent<Material>();true){
        auto p=c.GetProjectionMatrix();
        auto v=c.GetViewMatrix();
        auto vp=p*v;
        mat.SetUniform(4,vp);
    }
}

}

