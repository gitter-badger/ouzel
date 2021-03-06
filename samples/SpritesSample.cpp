// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#include "SpritesSample.hpp"
#include "MainMenu.hpp"

using namespace std;
using namespace ouzel;

SpritesSample::SpritesSample():
    hideButton("button.png", "button_selected.png", "button_down.png", "", "Show/hide", "arial.fnt", 1.0f, Color::BLACK, Color::BLACK, Color::BLACK),
    wireframeButton("button.png", "button_selected.png", "button_down.png", "", "Wireframe", "arial.fnt", 1.0f, Color::BLACK, Color::BLACK, Color::BLACK),
    backButton("button.png", "button_selected.png", "button_down.png", "", "Back", "arial.fnt", 1.0f, Color::BLACK, Color::BLACK, Color::BLACK)
{
    eventHandler.gamepadHandler = bind(&SpritesSample::handleGamepad, this, placeholders::_1, placeholders::_2);
    eventHandler.uiHandler = bind(&SpritesSample::handleUI, this, placeholders::_1, placeholders::_2);
    eventHandler.keyboardHandler = bind(&SpritesSample::handleKeyboard, this, placeholders::_1, placeholders::_2);
    engine->getEventDispatcher()->addEventHandler(&eventHandler);

    camera.setScaleMode(scene::Camera::ScaleMode::SHOW_ALL);
    camera.setTargetContentSize(Size2(800.0f, 600.0f));
    cameraActor.addComponent(&camera);
    layer.addChild(&cameraActor);
    addLayer(&layer);

    // character
    characterSprite.init("run.json");
    characterSprite.play(true);

    character.addComponent(&characterSprite);
    layer.addChild(&character);
    character.setPosition(Vector2(-300.0f, 0.0f));

    move.reset(new scene::Move(4.0f, Vector2(300.0f, 0.0f)));
    character.addComponent(move);
    move->start();

    // fire
    fireSprite.init("fire.json");
    fireSprite.setOffset(Vector2(0.0f, 20.0f));
    fireSprite.play(true);

    fireActor.addComponent(&fireSprite);
    fireActor.setPosition(Vector2(-100.0f, -140.0f));
    layer.addChild(&fireActor);

    // triangle
    triangleSprite.init("triangle.json");

    triangleActor.addComponent(&triangleSprite);
    triangleActor.setPosition(Vector2(100.0f, -140.0f));
    layer.addChild(&triangleActor);

    guiCamera.setScaleMode(scene::Camera::ScaleMode::SHOW_ALL);
    guiCamera.setTargetContentSize(Size2(800.0f, 600.0f));
    guiCameraActor.addComponent(&guiCamera);
    guiLayer.addChild(&guiCameraActor);
    addLayer(&guiLayer);

    guiLayer.addChild(&menu);

    hideButton.setPosition(Vector2(-200.0f, 200.0f));
    menu.addWidget(&hideButton);

    wireframeButton.setPosition(Vector2(-200.0f, 160.0f));
    menu.addWidget(&wireframeButton);

    backButton.setPosition(Vector2(-200.0f, -200.0f));
    menu.addWidget(&backButton);
}

bool SpritesSample::handleGamepad(Event::Type type, const GamepadEvent& event)
{
    if (type == Event::Type::GAMEPAD_BUTTON_CHANGE)
    {
        if (event.pressed &&
            event.button == input::GamepadButton::FACE_RIGHT)
        {
            engine->getSceneManager()->setScene(std::unique_ptr<scene::Scene>(new MainMenu()));
        }
    }

    return true;
}

bool SpritesSample::handleUI(Event::Type type, const UIEvent& event)
{
    if (type == Event::Type::ACTOR_CLICK)
    {
        if (event.actor == &backButton)
        {
            engine->getSceneManager()->setScene(std::unique_ptr<scene::Scene>(new MainMenu()));
        }
        else if (event.actor == &hideButton)
        {
            character.setHidden(!character.isHidden());
        }
        else if (event.actor == &wireframeButton)
        {
            camera.setWireframe(!camera.getWireframe());
        }
    }

    return true;
}

bool SpritesSample::handleKeyboard(Event::Type type, const KeyboardEvent& event) const
{
    if (type == Event::Type::KEY_PRESS)
    {
        switch (event.key)
        {
            case input::KeyboardKey::ESCAPE:
            case input::KeyboardKey::MENU:
                engine->getSceneManager()->setScene(std::unique_ptr<scene::Scene>(new MainMenu()));
                break;
            default:
                break;
        }
    }

    return true;
}
