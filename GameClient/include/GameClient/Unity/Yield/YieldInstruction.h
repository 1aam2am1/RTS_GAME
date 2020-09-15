//
// Created by Michal_Marszalek on 14.09.2020.
//

#ifndef RTS_GAME_YIELDINSTRUCTION_H
#define RTS_GAME_YIELDINSTRUCTION_H


#include <cstddef>
#include <variant>
#include <memory>

enum waitFor {
    WaitForFixedUpdate, ///< Waits until next fixed frame rate update function
    WaitForEndOfFrame ///< Waits until the end of the frame after Unity has rendered every Camera and GUI, just before displaying the frame on screen.
};

class YieldInstruction;

/// Return type for all coroutines
typedef std::variant<std::monostate, std::nullptr_t, std::unique_ptr<YieldInstruction>, waitFor> YieldReturn;

/// Base class for all yield instructions.
class YieldInstruction {
public:
    /// Destructor
    virtual ~YieldInstruction() = default;

protected:

    /// Return std::monostate to stop.
    /// Return nullptr to break
    /// Return YieldInstruction to wait for yield
    /// Return WaitFor to wait for xxx.
    virtual YieldReturn operator()() { return {}; };
};


#endif //RTS_GAME_YIELDINSTRUCTION_H
