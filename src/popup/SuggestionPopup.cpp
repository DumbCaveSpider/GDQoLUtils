#include <Geode/Geode.hpp>
#include <argon/argon.hpp>
#include <Geode/ui/Button.hpp>
#include <Geode/ui/MDTextArea.hpp>
#include "Geode/ui/Popup.hpp"
#include "SuggestionPopup.hpp"

using namespace geode::prelude;

inline std::string getResponseMessage(web::WebResponse const& response,
    std::string const& fallback) {
    auto message = response.string().unwrapOrDefault();
    if (!message.empty())
        return message;
    return fallback;
}

SuggestionPopup* SuggestionPopup::create() {
    auto ret = new SuggestionPopup();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool SuggestionPopup::init() {
    if (!Popup::init(360.f, 200.f, "GJ_square02.png")) {
        return false;
    }

    this->setTitle("Suggest a QoL Feature!");
    addSideArt(m_mainLayer, SideArt::All, SideArtStyle::PopupGold, false);

    // suggestion name input
    m_suggestionName = TextInput::create(180.f, "Provide a suggestion title", "chatFont.fnt");
    m_suggestionName->setCommonFilter(CommonFilter::Name);
    m_suggestionName->setTextAlign(TextInputAlign::Left);
    m_suggestionName->setLabel("Suggestion Title");
    m_mainLayer->addChildAtPosition(m_suggestionName, Anchor::Center, {-80.f, 30.f}, false);

    // suggestion description input
    m_suggestionDesc = TextInput::create(180.f, "Provide a suggestion description", "chatFont.fnt");
    m_suggestionDesc->setCommonFilter(CommonFilter::Any);
    m_suggestionDesc->setTextAlign(TextInputAlign::Left);
    m_suggestionDesc->setLabel("Suggestion Description");
    m_mainLayer->addChildAtPosition(m_suggestionDesc, Anchor::Center, {-80.f, -25.f}, false);

    // info button
    auto infoBtnSpr = CCSprite::createWithSpriteFrameName("GJ_infoBtn_001.png");
    infoBtnSpr->setScale(0.8f);
    auto infoBtnBtn = geode::Button::createWithNode(infoBtnSpr, [this](geode::Button* sender) {
        MDPopup::create(
            "What to suggest?",
            "<cf>ArcticWoof's QoL Utils</c> accepts <cg>any new suggestions and featues</c> that are <cl>lightweight to implement</c> and don't already exist on <cp>Geode</c>.\n\n"
            "If you have an idea for a <cl>QoL feature</c>, please <cg>submit</c> it using this suggestion popup!\n\n"
            "Please avoid suggesting <cr>Editor-related features</c> or <cr>gameplay changes</c>, as those are unlikely to be implemented here.\n\n"
            "All <cg>suggestions</c> will be reviewed by <cf>ArcticWoof</c> and <co>may be added</c> to the mod in a <cc>future update</c>.\n\n"
            "### <cr>Please don't suggest features that is commonly found in other mods especially mod menus like noclip accuracy and other mod menu features from Mega Hack, Eclipse or QOLMod!</c>\n\n"
            "### <co>Also this mod IS NOT RELATED TO QOLMOD, DO NOT SUGGEST FEATURES THAT ARE EXCLUSIVELY FOUND IN QOLMOD, AS I PROBABLY WON'T IMPLEMENT THEM!</c>\n\n"
            "You can only <cg>suggest one feature</c> at <co>certain time</c>, this is to <cr>prevent spamming</c>.\n\n",
            "OK")
            ->show();
    });
    m_buttonMenu->addChildAtPosition(infoBtnBtn, Anchor::TopRight, {-3, -3}, false);

    // about submission
    std::string placeholderText = "__Enter a description for your suggestion!__\n\n<cy>Provide as much detail as possible, the more detailed your suggestion is, the more likely it is to be implemented!</c>";
    m_suggestionText = MDTextArea::create(placeholderText, {140.f, 105.f});
    m_mainLayer->addChildAtPosition(m_suggestionText, Anchor::Right, {-85.f, 5.f}, false);

    m_suggestionDesc->setCallback([this, placeholderText](std::string const& text) {
        if (text.empty()) {
            if (m_suggestionText) {
                m_suggestionText->setString(placeholderText.c_str());
                m_suggestionText->updateLabel();
            }
            return;
        }
        if (m_suggestionText) {
            m_suggestionText->setString(text.c_str());
            m_suggestionText->updateLabel();
        }
    });

    // submit
    auto submitSpr = ButtonSprite::create("Submit Suggestion");
    auto submitButton = geode::Button::createWithNode(submitSpr, [this](geode::Button* sender) {
        if (m_suggestionName->getString().empty() || m_suggestionDesc->getString().empty()) {
            Notification::create("Provide a title and description before submitting!", NotificationIcon::Error)->show();
            return;
        }

        createQuickPopup("Submit Suggestion",
            "Are you sure you want to <cg>submit this suggestion</c>?\n<cy>Your suggestion will be reviewed by ArcticWoof.</c>",
            "No",
            "Yes",
            [this, sender](auto, bool yes) {
                if (!yes)
                    return;
                this->onSubmit(sender);
            });
    });
    m_buttonMenu->addChildAtPosition(submitButton, Anchor::Bottom, {0.f, 25.f}, false);

    return true;
}

void SuggestionPopup::onSubmit(CCObject* sender) {
    m_uploadPopup = UploadActionPopup::create(nullptr, "Submitting your suggestion...");
    m_uploadPopup->show();

    if (m_suggestionName->getString().empty() || m_suggestionDesc->getString().empty()) {
        m_uploadPopup->showFailMessage("Missing fields");
        return;
    }
    auto accountData = argon::getGameAccountData();
    m_authTask.spawn(
        argon::startAuth(std::move(accountData)),
        [this](Result<std::string> res) {
            if (res.isOk()) {
                auto token = std::move(res).unwrap();
                log::debug("Auth successful, token: {}", token);

                matjson::Value suggestionData = matjson::Value::object();
                suggestionData["accountId"] = GJAccountManager::get()->m_accountID;
                suggestionData["username"] = std::string(GJAccountManager::get()->m_username.c_str());
                suggestionData["argonToken"] = std::move(token);
                suggestionData["suggestionName"] = std::string(m_suggestionName ? m_suggestionName->getString().c_str() : "");
                suggestionData["suggestionDescription"] = std::string(m_suggestionDesc ? m_suggestionDesc->getString().c_str() : "");

                auto postReq = web::WebRequest();
                postReq.bodyJSON(suggestionData);

                m_submitTask.spawn(
                    postReq.post("https://gdqol.arcticwoof.xyz/submitSuggestion"),
                    [this](web::WebResponse response) {
                        if (response.ok()) {
                            m_uploadPopup->showSuccessMessage(getResponseMessage(response, "Suggestion submitted successfully!"));
                            this->onClose(nullptr);
                        } else {
                            m_uploadPopup->showFailMessage(getResponseMessage(response, "Failed to submit suggestion."));
                        }
                    });

            } else {
                auto err = res.unwrapErr();
                m_uploadPopup->showFailMessage(err);
                argon::clearToken();
                return;
            }
        });
}