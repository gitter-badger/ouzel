// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include "gui/Widget.hpp"
#include "events/EventHandler.hpp"

namespace ouzel
{
    namespace gui
    {
        class Menu: public Widget
        {
        public:
            Menu();

            virtual void setEnabled(bool newEnabled) override;

            void addWidget(Widget* widget)
            {
                addChildWidget(widget);
            }
            template<class T> void addWidget(const std::unique_ptr<T>& widget)
            {
                addChildWidget(widget.get());
            }
            template<class T> void addWidget(std::unique_ptr<T>&& widget)
            {
                addChildWidget(widget.get());
                ownedChildren.push_back(std::move(widget));
            }

            void selectWidget(Widget* widget);
            virtual void selectNextWidget();
            virtual void selectPreviousWidget();

        protected:
            virtual void addChildWidget(Widget* widget);
            virtual bool removeChildActor(Actor* actor) override;

            virtual void enter() override;
            virtual void leave() override;

            bool handleKeyboard(Event::Type type, const KeyboardEvent& event);
            bool handleGamepad(Event::Type type, const GamepadEvent& event);
            bool handleUI(Event::Type type, const UIEvent& event);

            std::vector<Widget*> widgets;
            Widget* selectedWidget = nullptr;

            EventHandler eventHandler;
        };
    } // namespace gui
} // namespace ouzel
