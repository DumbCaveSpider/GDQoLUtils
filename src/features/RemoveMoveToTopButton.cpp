#include <Geode/Geode.hpp>
#include <Geode/modify/EditLevelLayer.hpp>

using namespace geode::prelude;

class $modify(RemoveEditLevelLayer, EditLevelLayer) {
    bool init(GJGameLevel* level) {
        if (!EditLevelLayer::init(level)) return false;
        if (!Mod::get()->getSettingValue<bool>("enableRemoveMoveToTop")) return true;

        auto actionMenu = this->getChildByID("level-actions-menu");
        if (actionMenu) actionMenu->getChildByID("move-to-top-button")->setVisible(false);

        return true;
    }
};