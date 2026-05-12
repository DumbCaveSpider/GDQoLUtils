#include <Geode/Geode.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

using namespace geode::prelude;

class $modify(HideFolderListLevelInfo, LevelInfoLayer) {
    bool init(GJGameLevel* level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge)) return false;

        auto otherMenu = this->getChildByID("other-menu");
        if (otherMenu) {
            auto listButton = otherMenu->getChildByID("list-button");
            auto folderButton = otherMenu->getChildByID("folder-button");

            if (listButton) listButton->setVisible(!Mod::get()->getSettingValue<bool>("hideListLevelInfo"));
            if (folderButton) folderButton->setVisible(!Mod::get()->getSettingValue<bool>("hideFolderLevelInfo"));
        }

        return true;
    }
};