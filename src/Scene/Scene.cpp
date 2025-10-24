#include "Scene.h"

#include <memory>

void Scene::Init(const std::shared_ptr<Window>& window, const std::shared_ptr<Settings>& setting)
{
    this->window = window;
    settings = setting;
}

void Scene::Update()
{
    // this function is virtual
}

void Scene::Event(sf::Event& event)
{
    // this function is virtual
}

void Scene::Render()
{
    // this function is virtual
}

Scene::~Scene() = default;
