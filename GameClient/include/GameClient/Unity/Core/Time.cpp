//
// Created by Michal_Marszalek on 09.12.2020.
//

#include "Time.h"

float Time::timeScale = 1;
float Time::maximumDeltaTime = 1.f / 5.f;
float Time::fixedDeltaTime = 0.02f;
float Time::m_Time = 0;
float Time::m_unscaled_Time = 0;
float Time::m_fixedTime = 0;
float Time::m_unscaled_fixedTime = 0;
float Time::m_deltaTime = 0;
float Time::m_unscaled_deltaTime = 0;

float Time::deltaTime() {
    return m_deltaTime;
}

float Time::unscaledDeltaTime() {
    return m_unscaled_deltaTime;
}

float Time::fixedUnscaledDeltaTime() {
    return m_fixedTime / timeScale;
}

float Time::time() {
    return m_Time;
}

float Time::unscaledTime() {
    return m_unscaled_Time;
}

float Time::fixedTime() {
    return m_fixedTime;
}

float Time::fixedUnscaledTime() {
    return m_unscaled_fixedTime;
}
