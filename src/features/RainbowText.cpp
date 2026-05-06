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

static void applyRainbowTint(cocos2d::CCNode* node, float saturation, float value, unsigned int& index) {
    if (!node) {
        return;
    }

    if (auto sprite = typeinfo_cast<CCSprite*>(node)) {
        float startHue = std::fmod(static_cast<float>(index) * 36.0f, 360.0f);
        auto startColor = hsvToRgb(startHue, saturation, value);
        auto nextColor1 = hsvToRgb(startHue + 90.0f, saturation, value);
        auto nextColor2 = hsvToRgb(startHue + 180.0f, saturation, value);
        auto nextColor3 = hsvToRgb(startHue + 270.0f, saturation, value);

        sprite->setColor(startColor);
        sprite->stopAllActions();
        sprite->runAction(CCRepeatForever::create(
            CCSequence::create(
                CCTintTo::create(0.5f, nextColor1.r, nextColor1.g, nextColor1.b),
                CCTintTo::create(0.5f, nextColor2.r, nextColor2.g, nextColor2.b),
                CCTintTo::create(0.5f, nextColor3.r, nextColor3.g, nextColor3.b),
                CCTintTo::create(0.5f, startColor.r, startColor.g, startColor.b),
                nullptr)));
        ++index;
    }

    auto children = node->getChildren();
    if (!children) {
        return;
    }

    unsigned int count = children->count();
    for (unsigned i = 0; i < count; ++i) {
        auto child = typeinfo_cast<CCNode*>(children->objectAtIndex(i));
        if (child) {
            applyRainbowTint(child, saturation, value, index);
        }
    }
}

class $modify(RainbowText, CCLabelBMFont) {
    bool initWithString(const char* newString, const char* fntFile, float width = kCCLabelAutomaticWidth, CCTextAlignment alignment = kCCTextAlignmentLeft, CCPoint imageOffset = CCPointZero) {
        bool result = CCLabelBMFont::initWithString(newString, fntFile, width, alignment, imageOffset);
        if (!result) return false;
        if (!Mod::get()->getSettingValue<bool>("enableRainbowText")) return true;

        float saturation = Mod::get()->getSettingValue<float>("rainbowTextSaturation");
        float value = Mod::get()->getSettingValue<float>("rainbowTextValue");

        unsigned int index = 0;
        applyRainbowTint(this, saturation, value, index);

        return true;
    }

    void updateLabel() override {
        CCLabelBMFont::updateLabel();
        if (!Mod::get()->getSettingValue<bool>("enableRainbowText")) return;

        float saturation = Mod::get()->getSettingValue<float>("rainbowTextSaturation");
        float value = Mod::get()->getSettingValue<float>("rainbowTextValue");

        unsigned int index = 0;
        applyRainbowTint(this, saturation, value, index);
    }
};