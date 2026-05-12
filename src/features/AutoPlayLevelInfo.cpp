#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <string>
#include <cmath>

using namespace geode::prelude;

class $modify(AutoPlayLevelInfo, LevelInfoLayer) {
public:
    struct Fields {
        int m_autoplayRemaining = -1;
    };

    void autoplayTimerTick(float dt) {
        auto counterTimer = typeinfo_cast<CCLabelBMFont*>(this->getChildByID("autoplay-timer"));
        if (!counterTimer) {
            m_fields->m_autoplayRemaining = -1;
            this->unschedule(schedule_selector(AutoPlayLevelInfo::autoplayTimerTick));
            return;
        }

        if (m_fields->m_autoplayRemaining <= 0) {
            LevelInfoLayer::onPlay(nullptr);
            m_fields->m_autoplayRemaining = -1;
            this->unschedule(schedule_selector(AutoPlayLevelInfo::autoplayTimerTick));
            return;
        }

        --m_fields->m_autoplayRemaining;
        counterTimer->setString(std::to_string(m_fields->m_autoplayRemaining).c_str());

        if (m_fields->m_autoplayRemaining <= 0) {
            LevelInfoLayer::onPlay(nullptr);
            m_fields->m_autoplayRemaining = -1;
            this->unschedule(schedule_selector(AutoPlayLevelInfo::autoplayTimerTick));
        }
    }

    void levelDownloadFinished(GJGameLevel* level) {
        LevelInfoLayer::levelDownloadFinished(level);

        if (!Mod::get()->getSettingValue<bool>("enableAutoPlay")) return;

        if (m_playBtnMenu && !m_playBtnMenu->getChildByID("autoplay-timer")) {
            float delayValue = Mod::get()->getSettingValue<int>("autoPlayDelay");
            int remainingSeconds = std::max<int>(1, static_cast<int>(std::ceil(delayValue)));

            auto counterTimer = CCLabelBMFont::create(std::to_string(remainingSeconds).c_str(), "goldFont.fnt");
            counterTimer->setID("autoplay-timer");
            counterTimer->setPosition(m_playBtnMenu->getPosition() + CCPoint(0, 50));
            counterTimer->setScale(0.5f);
            this->addChild(counterTimer, 10);

            m_fields->m_autoplayRemaining = remainingSeconds;
            this->schedule(schedule_selector(AutoPlayLevelInfo::autoplayTimerTick), 1.0f);
        }
    }
};