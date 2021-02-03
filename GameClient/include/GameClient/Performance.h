//
// Created by Michal_Marszalek on 2021-02-03.
//

#ifndef RTS_GAME_PERFORMANCE_H
#define RTS_GAME_PERFORMANCE_H


#include <chrono>

class Performance {
public:
    enum Break {
        Event = 0,
        WindowUpdate = 1,
        WindowDraw = 2,
        Display = 3,
        Invoke = 4,
        RunInitializer = 5,
        Start = 6,
        Update = 7,
        LateUpdate = 8,
        BeforeDraw = 9,
        SceneRendering = 10,
        FixedUpdate = 11,
        Box2dStep = 12,
        OnTriggerEnter2D = 13,
        OnTriggerStay2D = 14,
        OnTriggerExit2D = 15,
        OnCollisionEnter2D = 16,
        OnCollisionStay2D = 17,
        OnCollisionExit2D = 18,
        ContactListenerUpdate = 19,
        Synchronize = 20,
    };

    void RefreshData(Break b);

    std::chrono::microseconds GetData(Break b);

    void reset();

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> last;
    constexpr static size_t tab_size = 21;
    std::chrono::microseconds tab[tab_size] = {};
    std::chrono::microseconds tab2[tab_size] = {};
};

extern Performance performance;

#endif //RTS_GAME_PERFORMANCE_H
