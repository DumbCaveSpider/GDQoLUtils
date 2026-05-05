#include <Geode/Geode.hpp>
#include <Geode/ui/Button.hpp>
#include "popup/SuggestionPopup.hpp"
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

class $modify(AWQOLMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;
        auto profileMenu = typeinfo_cast<CCMenu*>(this->getChildByID("profile-menu"));

        if (profileMenu) {
            auto suggestionSpr = CircleButtonSprite::create(CCSprite::create("wrenchIcon.png"_spr), CircleBaseColor::Blue, CircleBaseSize::Medium);
            auto suggestionBtn = geode::Button::createWithNode(suggestionSpr, [this](geode::Button* sender) {
                auto popup = SuggestionPopup::create();
                popup->show();
            });
            profileMenu->addChild(suggestionBtn);
            profileMenu->updateLayout();
        }

        return true;
    }
};