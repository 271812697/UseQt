#ifndef SCENE01_H
#define SCENE01_H

#include"scene/scene.h"

namespace scene {
class scene01 : public Scene
{
public:
    using Scene::Scene;

    void Init() override;
    void OnSceneRender() override;
    void OnImGuiRender() override;

    asset_ref<Texture> irradiance_map;
    asset_ref<Texture> prefiltered_map;
    asset_ref<Texture> BRDF_LUT;
    Entity sphere;
    Entity mingyue;
    Entity zhaolinger;
    Entity camera;
    Entity skybox;
    Entity direct_light;
    Entity point_light;

    void PrecomputeIBL(const std::string& hdri);
    void SetupMaterial(Material& mat,int id);
};
}


#endif // SCENE01_H
