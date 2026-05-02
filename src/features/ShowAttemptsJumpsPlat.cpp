#include <Geode/Geode.hpp>
#include <Geode/modify/EndLevelLayer.hpp>

using namespace geode::prelude;

class $modify(PlatEndLevelLayer, EndLevelLayer) {
    void customSetup() {
        EndLevelLayer::customSetup();

        if (!m_playLayer->m_isPlatformer) return;
        // get attempts and jumps for platformer
        int attempts = m_playLayer->m_attempts;
        int jumps = m_playLayer->m_jumps;
        log::debug("goog {} {}", attempts, jumps);

        auto summaryContainer = typeinfo_cast<CCNode*>(this->m_mainLayer->getChildByID("summary-container"));

        if (summaryContainer) {
            auto jumpsLabel = CCLabelBMFont::create(
                ("Jumps: " + numToString(jumps)).c_str(), "goldFont.fnt");
            summaryContainer->addChild(jumpsLabel);
            
            auto attemptsLabel = CCLabelBMFont::create(
                ("Attempts: " + numToString(attempts)).c_str(), "goldFont.fnt");
            summaryContainer->addChild(attemptsLabel);

            summaryContainer->updateLayout();
        }
    }
};