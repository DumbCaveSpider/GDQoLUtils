#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

class $modify(GYIMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        auto mainMenu = typeinfo_cast<CCMenu*>(this->getChildByID("main-menu"));
        if (!mainMenu) return true;

        auto playButtonNode = mainMenu->getChildByID("play-button");
        auto iconKitButtonNode = mainMenu->getChildByID("icon-kit-button");
        auto editorButtonNode = mainMenu->getChildByID("editor-button");

        if (playButtonNode) {
            if (auto playButton = playButtonNode->getChildByType<CCSprite>(0)) {
                playButton->setScale(Mod::get()->getSettingValue<float>("scalePlayButton"));
            }
        }
        if (iconKitButtonNode) {
            if (auto iconKitButton = iconKitButtonNode->getChildByType<CCSprite>(0)) {
                iconKitButton->setScale(Mod::get()->getSettingValue<float>("scaleIconKitButton"));
            }
        }
        if (editorButtonNode) {
            if (auto editorButton = editorButtonNode->getChildByType<CCSprite>(0)) {
                editorButton->setScale(Mod::get()->getSettingValue<float>("scaleEditorButton"));
            }
        }

        return true;
    }
};