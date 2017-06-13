// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include <memory>
#include <functional>
#include "scene/Component.h"
#include "core/UpdateCallback.h"

namespace ouzel
{
    namespace scene
    {
        class Node;

        class Animator: public Component
        {
            friend Node;
        public:
            Animator(float aLength);
            virtual ~Animator();

            virtual void update(float delta);

            virtual void start();
            virtual void play();

            virtual void resume();
            virtual void stop(bool resetAnimation = false);
            virtual void reset();

            bool isRunning() const { return running; }
            bool isDone() const { return done; }

            float getLength() const { return length; }
            float getCurrentTime() const { return currentTime; }

            float getProgress() const { return progress; }
            virtual void setProgress(float newProgress);

            Node* getTargetNode() const { return targetNode; }

            void addAnimator(Animator* animator)
            {
                addChildAnimator(animator);
            }
            template<class T> void addAnimator(const std::unique_ptr<T>& animator)
            {
                addChildAnimator(animator.get());
            }
            template<class T> void addAnimator(std::unique_ptr<T>&& animator)
            {
                addChildAnimator(animator.get());
                ownedAnimators.push_back(std::move(animator));
            }

            bool removeAnimator(Animator* animator)
            {
                return removeChildAnimator(animator);
            }
            template<class T> bool removeAnimator(const std::unique_ptr<T>& animator)
            {
                return removeChildAnimator(animator.get());
            }
            void removeAllAnimators();

            Animator* getParent() const { return parent; }
            void removeFromParent();

        protected:
            virtual void addChildAnimator(Animator* animator);
            virtual bool removeChildAnimator(Animator* animator);

            virtual void updateProgress();

            float length = 0.0f;
            float currentTime = 0.0f;
            float progress = 0.0f;
            bool done = false;
            bool running = false;

            Animator* parent = nullptr;
            Node* targetNode = nullptr;

            UpdateCallback updateCallback;

            std::vector<Animator*> animators;
            std::vector<std::unique_ptr<Animator>> ownedAnimators;
        };
    } // namespace scene
} // namespace ouzel
