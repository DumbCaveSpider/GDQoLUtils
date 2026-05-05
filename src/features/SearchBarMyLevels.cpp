#include <Geode/Geode.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>

using namespace geode::prelude;

class $modify(SearchBarMyLevels, LevelBrowserLayer) {
    struct Fields {
        SetTextPopup* m_setTextPopup;
    };
    bool init(GJSearchObject* searchObject) {
        if (!LevelBrowserLayer::init(searchObject)) return false;

        auto searchMenu = this->getChildByID("search-menu");

        if (searchMenu && getSearchTitle() == "My Online Levels") {
            auto findBtnSpr = CCSprite::createWithSpriteFrameName("gj_findBtn_001.png");
            auto findBtnBtn = CCMenuItemSpriteExtra::create(findBtnSpr, this, menu_selector(SearchBarMyLevels::onSearch));

            searchMenu->addChild(findBtnBtn);
        }
        return true;
    }

    void onSearch(CCObject* sender) {
        m_fields->m_setTextPopup = SetTextPopup::create(
            "",
            "Search",
            0,
            "Find Level",
            "OK",
            true,
            60.f);
        m_fields->m_setTextPopup->m_delegate = this;
        m_fields->m_setTextPopup->show();
    }
};