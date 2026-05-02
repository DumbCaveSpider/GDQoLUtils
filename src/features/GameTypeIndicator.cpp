#include <Geode/Geode.hpp>
#include <Geode/modify/LevelCell.hpp>

using namespace geode::prelude;

class $modify(GYILeveCell, LevelCell) {
    void loadFromLevel(GJGameLevel* level) {
        LevelCell::loadFromLevel(level);
        // log::debug("level cell {}", level->m_levelID);
        // log::debug("length {}", level->m_levelLength);

        if (!m_backgroundLayer) return;

        CCLayerGradient* customBackgroundLayer = CCLayerGradient::create({0, 0, 0, 255}, {0, 0, 0, 0}, ccp(1, 0));
        customBackgroundLayer->setPosition(m_backgroundLayer->getPosition());
        customBackgroundLayer->setID("gametype-indicator-background");
        customBackgroundLayer->setVisible(false);
        m_backgroundLayer->addChild(customBackgroundLayer, 0);

        auto lengthLabel = typeinfo_cast<CCLabelBMFont*>(this->m_mainLayer->getChildByID("length-label"));
        if (lengthLabel) lengthLabel->setColor({255, 255, 255});
        int directionX = Mod::get()->getSettingValue<int>("directionX");
        int directionY = Mod::get()->getSettingValue<int>("directionY");

        // classic
        if (level->m_levelLength >= 0 && level->m_levelLength <= 4) {
            customBackgroundLayer->initWithColor(Mod::get()->getSettingValue<ccColor4B>("classicBackgroundColorStart"), Mod::get()->getSettingValue<ccColor4B>("classicBackgroundColorEnd"), ccp(directionX, directionY));
            customBackgroundLayer->changeWidthAndHeight(this->m_width, this->m_height);
            if (lengthLabel) lengthLabel->setColor(Mod::get()->getSettingValue<ccColor3B>("classicTextColor"));
            customBackgroundLayer->setVisible(true);
        }

        // platformer
        if (level->m_levelLength == 5) {
            customBackgroundLayer->initWithColor(Mod::get()->getSettingValue<ccColor4B>("platformerBackgroundColorStart"), Mod::get()->getSettingValue<ccColor4B>("platformerBackgroundColorEnd"), ccp(directionX, directionY));
            customBackgroundLayer->changeWidthAndHeight(this->m_width, this->m_height);
            if (lengthLabel) lengthLabel->setColor(Mod::get()->getSettingValue<ccColor3B>("platformerTextColor"));
            customBackgroundLayer->setVisible(true);
        }
    }
};
