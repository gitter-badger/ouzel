// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include <thread>
#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/looper.h>
#include "core/Engine.hpp"

namespace ouzel
{
    class EngineAndroid: public Engine
    {
    public:
        static const jint ORIENTATION_PORTRAIT = 0x00000001;
        static const jint ORIENTATION_LANDSCAPE = 0x00000002;

        EngineAndroid(JavaVM* aJavaVM);
        virtual ~EngineAndroid();

        void onCreate(jobject aMainActivity);
        void onSurfaceCreated(jobject newSurface);
        void onSurfaceDestroyed();
        void onConfigurationChanged(jobject newConfig);

        virtual int run() override;

        virtual void executeOnMainThread(const std::function<void(void)>& func) override;
        virtual bool openURL(const std::string& url) override;

        virtual void setScreenSaverEnabled(bool newScreenSaverEnabled) override;

        virtual bool setCurrentThreadName(const std::string& name) override;

        JavaVM* getJavaVM() const { return javaVM; }
        jobject getMainActivity() const { return mainActivity; }
        jobject getSurface() const { return surface; }
        AAssetManager* getAssetManager() const { return assetManager; }

        const std::string& getFilesDirectory() const { return filesDirectory; }
        const std::string& getCacheDirectory() const { return cacheDirectory; }

        void executeAll();

    private:
        virtual void main() override;

        JavaVM* javaVM = nullptr;
        jclass uriClass = nullptr;
        jmethodID parseMethod = nullptr;
        jclass intentClass = nullptr;
        jmethodID intentConstructor = nullptr;
        jobject mainActivity = nullptr;
        jmethodID startActivityMethod = nullptr;
        jclass configurationClass = nullptr;
        jfieldID orientationField = nullptr;
        jint orientation = 0;
        jobject surface = nullptr;
        AAssetManager* assetManager = nullptr;
        jobject androidWindow = nullptr;
        jmethodID addFlagsMethod = nullptr;
        jmethodID clearFlagsMethod = nullptr;
        ALooper* looper = nullptr;
        int fd[2];

        std::string filesDirectory;
        std::string cacheDirectory;

        std::queue<std::function<void(void)>> executeQueue;
        std::mutex executeMutex;
    };
}
