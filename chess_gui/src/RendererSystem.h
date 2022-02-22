#include <Graphics/Renderer.h>
#include <Application.h>
#include <ECS/CommonComponent.h>
#include <ECS/GraphicsComponent.h>

struct RendererStuff
{
	GBuffer iBuffer;
	GBuffer vBuffer;
	GBuffer texture;
};

class RendererSystem : public System
{
private:
    RendererSystem();
public:
    static RendererSystem* init(Graphics_API graphicsAPI);
    static RendererSystem* GetSingleton();
	void CreateGBufferMesh(Mesh* mesh, GBuffer* iBuffer, GBuffer* vBuffer);
    ~RendererSystem();
    void LoadScene(Scene* scene) override;
    void Update(float deltaTime) override;
private:
	Mat SetUpCamera(uint32_t entity);
	void LoadMaterial(Scene::Entities::iterator& itr);
	void LoadLights();
	void LoadLightColor(const LightColor& color, std::string name);
	void LoadTransform(Scene::Entities::iterator& itr);
	void LoadBuffer(GBuffer* buffer);
	void CreateRendererStuff(Mesh* mesh, RendererStuff* rendererStuff);
	void SetupDefaultTexture();
	Mat LookAt(const Vect3& pos, const Vect3& dir);
	Mat SetupPerspective(Camera& camera);
	Scene* GetScene();
private:
    std::unique_ptr<Renderer> renderer;
    Scene* scene;
    static RendererSystem* singleton;
	float animated;
	Vect2 resoltuion;
// Default Values
	Mat transformDefault;
	uint32_t mainCamera;
	Texture defaultTexture;
	GBuffer defaultTextureGBuffer;
	Material defaultMaterial;
// 
	uint32_t layout;
	Scene::IComponentArray* camera;
	std::vector<uint32_t> lights;
	std::unique_ptr<ShaderStageHandler> mainShaderStage;

};
