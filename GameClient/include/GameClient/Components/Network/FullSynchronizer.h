//
// Created by Michal_Marszalek on 2021-02-07.
//

#ifndef RTS_GAME_FULLSYNCHRONIZER_H
#define RTS_GAME_FULLSYNCHRONIZER_H


#include "Synchronizer.h"

class FullSynchronizer : public Synchronizer {
public:
    bool send = false;

    void Update() override;

private:
protected:
    void ReceiveMessage(const nlohmann::json &json) override;
};


#endif //RTS_GAME_FULLSYNCHRONIZER_H
