#include <Geode/Geode.hpp>

using namespace geode::prelude;

class SuggestionPopup : public Popup {
public:
    static SuggestionPopup* create();

private:
    bool init() override;
    void onSubmit(CCObject* sender);

    async::TaskHolder<Result<std::string>> m_authTask;
    async::TaskHolder<web::WebResponse> m_submitTask;
    TextInput* m_suggestionName;
    TextInput* m_suggestionDesc;
    MDTextArea* m_suggestionText;
    UploadActionPopup* m_uploadPopup;
};