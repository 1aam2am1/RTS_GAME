//
// Created by Michal_Marszalek on 2021-01-18.
//

#include <GameClient/Unity/Core/MonoBehaviour.h>
#include <GameApi/GlobalLogSource.h>
#include <Macro.h>
#include <GameClient/Unity/Editor/Menu.h>

class Emitter : public MonoBehaviour {
public:

    void Awake() override {
        GameApi::log(INFO << "Awake");
    }

    void FixedUpdate() override {
        ///GameApi::log(INFO << "FixedUpdate");
    }

    void LateUpdate() override {
        ///GameApi::log(INFO << "LateUpdate");
    }

    void OnCollisionEnter2D(const Collision2D &other) override {
        GameApi::log(INFO << "OnCollisionEnter2D " << other.gameObject->name);
    }

    void OnCollisionExit2D(const Collision2D &other) override {
        GameApi::log(INFO << "OnCollisionExit2D " << other.gameObject->name);
    }

    void OnCollisionStay2D(const Collision2D &other) override {
        GameApi::log(INFO << "OnCollisionStay2D " << other.gameObject->name);
    }

    void OnDestroy() override {
        GameApi::log(INFO << "OnDestroy");
    }

    void OnDisable() override {
        GameApi::log(INFO << "OnDisable");
    }

    void OnEnable() override {
        GameApi::log(INFO << "OnEnable");
    }

    void OnTriggerEnter2D(TPtr<const Collider2D> other) override {
        GameApi::log(INFO << "OnTriggerEnter2D " << other->gameObject()->name);
    }

    void OnTriggerExit2D(TPtr<const Collider2D> other) override {
        GameApi::log(INFO << "OnTriggerExit2D " << other->gameObject()->name);
    }

    void OnTriggerStay2D(TPtr<const Collider2D> other) override {
        GameApi::log(INFO << "OnTriggerStay2D " << other->gameObject()->name);
    }

    void Start() override {
        GameApi::log(INFO << "Start");
    }

    void Update() override {
        ///GameApi::log(INFO << "Update");
    }
};

ADD_COMPONENT_MENU(Emitter)
