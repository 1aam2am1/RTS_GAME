//
// Created by Michal_Marszalek on 2021-02-07.
//

#ifndef RTS_GAME_FULLSYNCHRONIZER_H
#define RTS_GAME_FULLSYNCHRONIZER_H


#include "Synchronizer.h"

class FullSynchronizer : public Synchronizer {
public:
    bool send = false;

    void IntUpdate() override;

private:
protected:
    void ReceiveMessage(const nlohmann::json &json) override;

    bool SendStatus();
};


#endif //RTS_GAME_FULLSYNCHRONIZER_H
