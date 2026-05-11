#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(CustomProgressBarPlayLayer, PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        if (!Mod::get()->getSettingValue<bool>("enableCustomProgressBarColor")) return true;

        auto progressFill = this->m_progressFill;
        auto progressBar = this->m_progressBar;
        if (progressFill && progressBar && !Mod::get()->getSettingValue<bool>("rainbowProgressBar")) {
            progressFill->setColor(Mod::get()->getSettingValue<ccColor3B>("progressBarColorFill"));
            progressBar->setColor(Mod::get()->getSettingValue<ccColor3B>("progressBarColorBorder"));
        }

        if (progressBar && progressFill && Mod::get()->getSettingValue<bool>("rainbowProgressBar")) {
            // make the progress bar rainbow by running a tint action on it
            float saturation = Mod::get()->getSettingValue<float>("rainbowProgressBarSaturation");
            float value = Mod::get()->getSettingValue<float>("rainbowProgressBarValue");
            float dur = Mod::get()->getSettingValue<float>("rainbowIntervalProgressBar");
            progressFill->runAction(CCRepeatForever::create(
                CCSequence::create(
                    CCTintTo::create(dur, 255, 0, 0),
                    CCTintTo::create(dur, 255, 255, 0),
                    CCTintTo::create(dur, 0, 255, 0),
                    CCTintTo::create(dur, 0, 255, 255),
                    CCTintTo::create(dur, 0, 0, 255),
                    CCTintTo::create(dur, 255, 0, 255),
                    nullptr)));
            progressBar->runAction(CCRepeatForever::create(
                CCSequence::create(
                    CCTintTo::create(dur, 255, 0, 0),
                    CCTintTo::create(dur, 255, 255, 0),
                    CCTintTo::create(dur, 0, 255, 0),
                    CCTintTo::create(dur, 0, 255, 255),
                    CCTintTo::create(dur, 0, 0, 255),
                    CCTintTo::create(dur, 255, 0, 255),
                    nullptr)));
        }

        return true;
    }

    void updateProgressbar() {
        PlayLayer::updateProgressbar();

        if (!Mod::get()->getSettingValue<bool>("enableCustomProgressBarColor")) return;

        auto progressFill = this->m_progressFill;
        auto progressBar = this->m_progressBar;
        if (progressFill && progressBar && !Mod::get()->getSettingValue<bool>("rainbowProgressBar")) {
            progressFill->setColor(Mod::get()->getSettingValue<ccColor3B>("progressBarColorFill"));
            progressBar->setColor(Mod::get()->getSettingValue<ccColor3B>("progressBarColorBorder"));
        }

        if (progressBar && progressFill && Mod::get()->getSettingValue<bool>("rainbowProgressBar")) {
            // make the progress bar rainbow by running a tint action on it
            float saturation = Mod::get()->getSettingValue<float>("rainbowProgressBarSaturation");
            float value = Mod::get()->getSettingValue<float>("rainbowProgressBarValue");
            float dur = Mod::get()->getSettingValue<float>("rainbowIntervalProgressBar");
            progressFill->runAction(CCRepeatForever::create(
                CCSequence::create(
                    CCTintTo::create(dur, 255, 0, 0),
                    CCTintTo::create(dur, 255, 255, 0),
                    CCTintTo::create(dur, 0, 255, 0),
                    CCTintTo::create(dur, 0, 255, 255),
                    CCTintTo::create(dur, 0, 0, 255),
                    CCTintTo::create(dur, 255, 0, 255),
                    nullptr)));
        }
    }
};