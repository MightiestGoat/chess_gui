#include "RendererSystem.h"
#include "Graphics/opengl/GLRenderer.h"
#include <Graphics/GraphicsComponent.h>
#include <commoncomponent.h>

RendererSystem* RendererSystem::singleton = nullptr;

RendererSystem::RendererSystem()
{}

RendererSystem* RendererSystem::init(Graphics_API graphicsAPI)
{
    singleton = new RendererSystem();
    singleton->renderer.reset(new GLRenderer());
    return singleton;
}

RendererSystem* RendererSystem::GetSingleton()
{
    return singleton;
}

void RendererSystem::LoadScene(Scene* scene)
{
    this->scene = scene;
    Mesh* mesh;
    for(auto& i: scene->entities)
    {
        mesh = i.second->get<Mesh>();
        renderer->LoadBuffer(mesh->vertexBuffer.get());
        renderer->LoadBuffer(mesh->indexbuffer.get());
        renderer->LoadTexture(mesh->texture.get());
    }
}

void RendererSystem::update(float deltaTime)
{
    Mesh* mesh;
    Transform* transform;
    for(auto& i: scene->entities)
    {
        mesh = i.second->get<Mesh>();
        mesh->vertexBuffer->Bind();
        mesh->indexbuffer->Bind();
        mesh->texture->Bind();
        renderer->Draw(mesh->indexbuffer.get());
    }
}
