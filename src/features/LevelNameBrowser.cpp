#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/ui/Button.hpp>
#include <Geode/utils/web.hpp>
#include <string>

using namespace geode::prelude;

class $modify(GYILevelInfoLayer, LevelInfoLayer) {
    bool init(GJGameLevel* level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge)) return false;

        if (!Mod::get()->getSettingValue<bool>("enableClickableLevelName")) return true;

        auto titleLabel = typeinfo_cast<CCLabelBMFont*>(this->getChildByID("title-label"));

        if (titleLabel) {
            int levelID = level->m_levelID.value();
            auto buttonTitle = geode::Button::create([this, levelID](geode::Button* sender) {
                auto url = std::string("https://gdbrowser.com/") + numToString(levelID);
                web::openLinkInBrowser(url);
            });

            buttonTitle->setSelectCallback([titleLabel](geode::Button* sender) {
                titleLabel->setColor({125, 125, 125});
            });
            buttonTitle->setUnselectCallback([titleLabel](geode::Button* sender) {
                titleLabel->setColor({255, 255, 255});
            });
            buttonTitle->setScaleMultiplier(1.f);
            buttonTitle->setContentSize(titleLabel->getContentSize());
            buttonTitle->setPosition(titleLabel->getPosition());
            buttonTitle->setID("clickable-level-name-button");
            buttonTitle->setOpacity(0);
            buttonTitle->setScale(titleLabel->getScale());
            this->addChild(buttonTitle);
        }

        return true;
    }
};