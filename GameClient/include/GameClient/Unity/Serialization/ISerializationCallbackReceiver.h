//
// Created by Michal_Marszalek on 01.11.2020.
//

#ifndef RTS_GAME_ISERIALIZATIONCALLBACKRECEIVER_H
#define RTS_GAME_ISERIALIZATIONCALLBACKRECEIVER_H


struct ISerializationCallbackReceiver {

    /// Implement this method to receive a callback before Unity serializes your object.
    virtual void OnBeforeSerialize() = 0;

    /// Implement this method to receive a callback after Unity deserializes your object.
    virtual void OnAfterDeserialize() = 0;
};


#endif //RTS_GAME_ISERIALIZATIONCALLBACKRECEIVER_H
