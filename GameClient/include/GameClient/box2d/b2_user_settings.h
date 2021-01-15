//
// Created by Michal_Marszalek on 15.01.2021.
//

#ifndef RTS_GAME_B2_USER_SETTINGS_H
#define RTS_GAME_B2_USER_SETTINGS_H


#include <box2d/b2_types.h>
#include <box2d/b2_api.h>
#include <stdarg.h>
#include <stdint.h>
#include <memory>

// Tunable Constants

/// You can use this to change the length scale used by your game.
/// For example for inches you could use 39.4.
#define b2_lengthUnitsPerMeter 1.0f

/// The maximum number of vertices on a convex polygon. You cannot increase
/// this too much because b2BlockAllocator has a maximum object size.
#define b2_maxPolygonVertices    8

// User data

struct Object;

/// You can define this to inject whatever data you want in b2Body
typedef std::weak_ptr<Object> b2BodyUserData;

/// You can define this to inject whatever data you want in b2Fixture
typedef std::weak_ptr<Object> b2FixtureUserData;

/// You can define this to inject whatever data you want in b2Joint
typedef std::weak_ptr<Object> b2JointUserData;

// Memory Allocation

/// Default allocation functions
void *b2Alloc_Default(int32 size);

void b2Free_Default(void *mem);

/// Implement this function to use your own memory allocator.
inline void *b2Alloc(int32 size) {
    return b2Alloc_Default(size);
}

/// If you implement b2Alloc, you should also implement this function.
inline void b2Free(void *mem) {
    b2Free_Default(mem);
}

/// Default logging function
void b2Log_Default(const char *string, va_list args);

/// Implement this to use your own logging.
inline void b2Log(const char *string, ...) {
    va_list args;
    va_start(args, string);
    b2Log_Default(string, args);
    va_end(args);
}

#endif //RTS_GAME_B2_USER_SETTINGS_H
