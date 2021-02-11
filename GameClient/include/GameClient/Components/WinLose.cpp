//
// Created by Michal_Marszalek on 2021-02-12.
//

#include <SceneManagement/SceneManager.h>
#include "WinLose.h"
#include "UserControls.h"
#include <Macro.h>
#include <GameClient/Unity/Editor/Menu.h>

ADD_USER_COMPONENT(WinLose)

void WinLose::Start() {
    signal = GetComponent<SignalSynchronizer>();
    if (signal) {
        signal->OnMessage.connect([](uint32_t id, auto) {
            if (id == 10) {
                SceneManager::LoadScene("Assets/menu/Lose.unity");
            }
            if (id == 20) {
                SceneManager::LoadScene("Assets/menu/Win.unity");
            }
        });
    }
}

void WinLose::Update() {
    if (enemy.empty()) {
        enemy = FindObjectsOfType<Enemy>();
    }
    std::erase_if(enemy, [](auto &ptr) { return ptr.expired(); });
    if (enemy.size() == 1) {
        if (enemy[0]->GetComponent<UserControls>()) {
            SceneManager::LoadScene(
                    "Assets/menu/Win.unity");

            if (signal) {
                signal->SendMessage(10, {});
            }
        } else {
            SceneManager::LoadScene("Assets/menu/Lose.unity");

            if (signal) {
                signal->SendMessage(20, {});
            }
        }
    }
}
