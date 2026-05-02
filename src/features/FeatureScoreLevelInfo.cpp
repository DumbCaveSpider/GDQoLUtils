#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/ui/Button.hpp>
#include <Geode/utils/web.hpp>
#include <string>

using namespace geode::prelude;

class $modify(SFSLevelInfoLayer, LevelInfoLayer) {
    bool init(GJGameLevel* level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge)) return false;

        if (!Mod::get()->getSettingValue<bool>("enableShowFeatureScore")) return true;
        CCNode* likesLabel = typeinfo_cast<CCNode*>(this->getChildByID("likes-label"));
        log::debug("feature {} rating sum {}", level->m_featured, level->m_ratingsSum);

        if (likesLabel && level->m_featured != 0) {
            CCPoint originalPos = likesLabel->getPosition();
            likesLabel->setPositionY(likesLabel->getPositionY() + 6);  // move the original likes label up a bit to make room for the ratio
            CCLabelBMFont* ratioLabel = CCLabelBMFont::create("", "bigFont.fnt");
            ratioLabel->setString(fmt::format("{}", GameToolbox::pointsToString(level->m_featured)).c_str());
            ratioLabel->setColor({253, 255, 120});
            ratioLabel->setScale(0.35f);
            ratioLabel->setAnchorPoint(likesLabel->getAnchorPoint());
            ratioLabel->setPosition(originalPos - CCPoint(0, 8));
            this->addChild(ratioLabel);
        }

        return true;
    }
};