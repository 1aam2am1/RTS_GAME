//
// Created by Michal_Marszalek on 2021-01-19.
//

#include "AudioListener.h"
#include <GameClient/Unity/Macro.h>
#include <GameClient/Unity/Editor/Menu.h>
#include <Core/Attributes.h>

ADD_COMPONENT_MENU(AudioListener)
ADD_ATTRIBUTE(AudioListener, DisallowMultipleComponent)

static std::vector<TPtr<AudioListener>> listener;
decltype(AudioListener::pause) AudioListener::pause = false;
decltype(AudioListener::volume) AudioListener::volume{[](auto v) { sf::Listener::setGlobalVolume(v * 100.f); },
                                                      []() { return sf::Listener::getGlobalVolume() / 100.0f; }};

void AudioListener::OnEnable() {
    auto it = std::find_if(listener.begin(), listener.end(), [](auto &&p) { return !p.expired(); });
    if (it != listener.end()) {
        GameApi::log(ERR << "You can only have one AudioListener");
    }

    listener.emplace_back(static_pointer_cast<AudioListener>(shared_from_this()));
}

void AudioListener::OnDisable() {

}

void AudioListener::OnDestroy() {
    std::erase_if(listener, [this](auto &&p) { return p.expired() || p.get() == this; });
    //Don't erase form listener TPtr is null
}

void AudioListener::Update() {
    auto p = transform()->localPosition.get();
    p.z = 0;
    sf::Listener::setPosition(p);
}

