#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/utils/base64.hpp>

using namespace geode::prelude;

class $modify(ObjectDisplayInLevelInfo, LevelInfoLayer) {
    bool init(GJGameLevel* level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge)) return false;

        if (!Mod::get()->getSettingValue<bool>("enableObjectDisplayInLevelInfo")) return true;

        auto titleLabel = typeinfo_cast<CCLabelBMFont*>(this->getChildByID("title-label"));

        if (titleLabel) {
            size_t objectsEstimated = level->m_objectCount.value();
            // auto decodedLevelString = LevelTools::base64DecodeString(level->m_levelString);
            // if (!decodedLevelString.empty()) {
            //     auto unpackedLevelString = ZipUtils::decompressString(decodedLevelString, false, 0);
            //     if (!unpackedLevelString.empty()) {
            //         objectsEstimated = std::count(unpackedLevelString.begin(), unpackedLevelString.end(), ';');
            //     }
            // }
            // i have no idea how to get the actual object count when decompressing the level string, which causes to crash, betterinfo failed me again :sob:

            std::string objectsString = "Object Count: " + numToString(GameToolbox::pointsToString(objectsEstimated));
            auto objectsLabel = CCLabelBMFont::create(objectsString.c_str(), "bigFont.fnt");
            objectsLabel->limitLabelWidth(80.f, 0.35f, 0.2f);
            if (objectsEstimated <= 40000) {
                objectsLabel->setColor({0, 255, 0});
            } else if (objectsEstimated <= 80000) {
                objectsLabel->setColor({255, 165, 0});
            } else {
                objectsLabel->setColor({255, 0, 0});
            }
            objectsLabel->setID("objects-label");
            objectsLabel->setAnchorPoint(titleLabel->getAnchorPoint());
            objectsLabel->setPosition(titleLabel->getPosition() + CCPoint(0, -15));
            this->addChild(objectsLabel);
        }

        return true;
    }
};