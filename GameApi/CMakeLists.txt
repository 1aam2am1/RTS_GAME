set(SRC
        include/GameApi/Version.cpp
        include/GameApi/Version.h
        include/GameApi/BasicString.h
        include/GameApi/BasicString.cpp
        include/GameApi/StringFormatter.h
        include/GameApi/StringFormatter.cpp
        include/GameApi/Externalsstream.cpp
        include/GameApi/Externalsstream.h
        include/GameApi/Message.cpp
        include/GameApi/Message.h
        include/GameApi/LogLevel.cpp
        include/GameApi/LogLevel.h
        include/GameApi/ThreadId.cpp
        include/GameApi/ThreadId.h
        include/GameApi/MessageCreator.cpp
        include/GameApi/MessageCreator.h
        include/GameApi/LogSink.cpp
        include/GameApi/LogSink.hpp
        include/GameApi/LogSource.cpp
        include/GameApi/LogSource.h
        include/GameApi/GlobalLogSource.cpp
        include/GameApi/GlobalLogSource.h
        include/GameApi/Sink/OsLogSink.cpp
        include/GameApi/Sink/OSLogSink.h
        Config.h
        include/GameApi/TQueue.h
        include/GameApi/reverse.h
        include/GameApi/Synchronized/Synchronized.h
        include/GameApi/Synchronized/BaseSynchronized.h
        include/GameApi/Synchronized/Lock.h
        include/GameApi/Signal.h
        include/GameApi/overload.h
        )

configure_file(include/GameApi/Config.h.in ${CMAKE_CURRENT_BINARY_DIR}/Config.h @ONLY)

add_library(GameAPI SHARED ${SRC})


set_target_properties(GameAPI PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)

target_include_directories(GameAPI PUBLIC
        ${Protobuf_INCLUDE_DIRS}
        ${PROJECT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SRC
        message.proto)

list(TRANSFORM SRC PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/include/")

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${SRC}) #EXPORT_MACRO DLL_EXPORT


target_sources(GameAPI PRIVATE ${PROTO_HDRS} ${PROTO_SRCS})

target_link_libraries(GameAPI ${Protobuf_LIBRARIES} sfml-network)
add_versioning(GameAPI)
add_flags(GameAPI)


set_source_files_properties(${PROTO_HDRS} ${PROTO_SRCS} PROPERTIES COMPILE_FLAGS "-Wno-all \
-Wno-extra \
-Wno-pedantic \
-Wno-return-local-addr \
-Wno-attributes \
-Wno-deprecated")


### Copy Protobuf Dependency DLL ###
get_filename_component(P "${Protobuf_PROTOC_EXECUTABLE}" DIRECTORY)

if (WIN32)
    set(GameAPI_DLL "${P}/libprotobuf$<$<CONFIG:Debug>:d>.dll"
            "${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_FILE_NAME:GameAPI>"
            PARENT_SCOPE)
endif ()
