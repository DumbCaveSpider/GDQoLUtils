#include <Geode/Geode.hpp>
#include <Geode/modify/CCLabelBMFont.hpp>
#include <cmath>

using namespace geode::prelude;

static bool RainbowTextLoaded = []() {
    log::info("RainbowText patch compiled and loaded");
    return true;
}();

static cocos2d::ccColor3B hsvToRgb(float h, float s, float v) {
    h = std::fmod(h, 360.0f);
    if (h < 0.0f) {
        h += 360.0f;
    }

    float c = v * s;
    float x = c * (1.0f - std::fabs(std::fmod(h / 60.0f, 2.0f) - 1.0f));
    float m = v - c;

    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;

    if (h < 60.0f) {
        r = c;
        g = x;
        b = 0.0f;
    } else if (h < 120.0f) {
        r = x;
        g = c;
        b = 0.0f;
    } else if (h < 180.0f) {
        r = 0.0f;
        g = c;
        b = x;
    } else if (h < 240.0f) {
        r = 0.0f;
        g = x;
        b = c;
    } else if (h < 300.0f) {
        r = x;
        g = 0.0f;
        b = c;
    } else {
        r = c;
        g = 0.0f;
        b = x;
    }

    return cocos2d::ccColor3B{
        static_cast<GLubyte>((r + m) * 255.0f),
        static_cast<GLubyte>((g + m) * 255.0f),
        static_cast<GLubyte>((b + m) * 255.0f)};
}

class $modify(RainbowText, CCLabelBMFont) {
    bool initWithString(const char* newString, const char* fntFile, float width = kCCLabelAutomaticWidth, CCTextAlignment alignment = kCCTextAlignmentLeft, CCPoint imageOffset = CCPointZero) {
        bool result = CCLabelBMFont::initWithString(newString, fntFile, width, alignment, imageOffset);
        if (!result) return false;
        if (!Mod::get()->getSettingValue<bool>("enableRainbowText")) return true;

        auto children = this->getChildren();
        if (!children) return true;

        unsigned int count = children->count();
        if (count == 0) return true;

        for (unsigned i = 0; i < count; ++i) {
            auto child = typeinfo_cast<CCSprite*>(children->objectAtIndex(i));
            if (!child) continue;

            float startHue = std::fmod(static_cast<float>(i) * 36.0f, 360.0f);
            auto startColor = hsvToRgb(startHue, 1.0f, 1.0f);
            auto nextColor1 = hsvToRgb(startHue + 90.0f, 1.0f, 1.0f);
            auto nextColor2 = hsvToRgb(startHue + 180.0f, 1.0f, 1.0f);
            auto nextColor3 = hsvToRgb(startHue + 270.0f, 1.0f, 1.0f);

            child->setColor(startColor);
            child->runAction(CCRepeatForever::create(
                CCSequence::create(
                    CCTintTo::create(0.5f, nextColor1.r, nextColor1.g, nextColor1.b),
                    CCTintTo::create(0.5f, nextColor2.r, nextColor2.g, nextColor2.b),
                    CCTintTo::create(0.5f, nextColor3.r, nextColor3.g, nextColor3.b),
                    CCTintTo::create(0.5f, startColor.r, startColor.g, startColor.b),
                    nullptr)));
        }

        return true;
    }
};