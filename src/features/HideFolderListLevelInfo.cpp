#include <Geode/Geode.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include "Geode/cocos/menu_nodes/CCMenu.h"

using namespace geode::prelude;

class $modify(HideFolderListLevelInfo, LevelInfoLayer) {
    bool init(GJGameLevel* level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge)) return false;

        auto otherMenu = typeinfo_cast<CCMenu*>(this->getChildByID("other-menu"));
        if (otherMenu) {
            auto listButton = typeinfo_cast<CCMenuItemSpriteExtra*>(otherMenu->getChildByID("list-button"));
            auto folderButton = typeinfo_cast<CCMenuItemSpriteExtra*>(otherMenu->getChildByID("folder-button"));

            if (listButton) listButton->setVisible(!Mod::get()->getSettingValue<bool>("hideListLevelInfo"));
            if (folderButton) folderButton->setVisible(!Mod::get()->getSettingValue<bool>("hideFolderLevelInfo"));
        }

        return true;
    }
};