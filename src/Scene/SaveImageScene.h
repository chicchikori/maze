#pragma once

#include "../UI/Button.h"
#include "Scene.h"


#include "../Util/Variables.h"
class SaveAsScene : public Scene
{
public:
    SaveAsScene() = default;
    void Update() override;
    void Event(sf::Event& event) override;
    void Render() override;

private:
    Button::RoundedButton saveButton;
    Button::RoundedButton calcelButton;
    int roadSize{IMAGE_ROAD_SIZE};
    int wallSize{IMAGE_WALL_SIZE};
    int seeds{0};
};
