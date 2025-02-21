//
// Created by Michal_Marszalek on 15.12.2020.
//

#ifndef RTS_GAME_GLOBALSTATICVARIABLES_H
#define RTS_GAME_GLOBALSTATICVARIABLES_H

#include <vector>
#include <map>
#include <filesystem>
#include <unordered_set>
#include <set>
#include <queue>
#include <execution>
#include <GameClient/Unity/Core/Camera.h>
#include <SFML/Graphics/RenderTexture.hpp>
#include <Core/Renderer.h>
#include <GameClient/Unity/Core/Guid.h>
#include <Editor/OneGuidFile.h>
#include <box2d/b2_world.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include "ContactListener.h"

namespace fs = std::filesystem;

class b2Body;

class Transform;

class SceneData {
public:
    SceneData();

    ~SceneData();
    //SceneData(const SceneData&) = default;
    //SceneData& operator=(const SceneData&) = default;
    //SceneData(SceneData&&) = default;


    int buildIndex = 0;
    bool isLoaded = false;
    std::string name = "Scene";
    std::string path{};
    Unity::GUID guid{};

    std::vector<TPtr<GameObject>> root{};
    std::vector<TPtr<Component>> new_components{}; //To Start
    decltype(new_components) new_components_copy{}; //Used in loop std::swap()

    std::vector<TPtr<Component>> loading_awake{}; //used when loading

    uint32_t GetID(const TPtr<Object> &);

    TPtr<Object> GetObject(uint32_t);

    bool RegisterID(uint32_t, const TPtr<Object> &);

private:
    std::shared_ptr<const int> guard;

    std::unordered_map<uint32_t, TPtr<>> localIDObject;
    std::unordered_map<Object *, std::pair<uint32_t, TPtr<>>> ObjectLocalID;
};

struct FileTime {
    Unity::GUID guid{};

    fs::file_time_type asset_time{};
    fs::file_time_type meta_time{};

    uint64_t md5_asset = 0;
    uint64_t md5_time = 0;

    bool operator==(const FileTime &) = delete;

    bool operator!=(const FileTime &) = delete;
};

struct GlobalStaticVariables {
    GlobalStaticVariables() noexcept;

    ~GlobalStaticVariables();

    struct {
        ///Camera
        std::vector<TPtr<Camera>> m_cameras{};
        std::multimap<float, TPtr<Camera>> m_draw_order{};

        std::vector<TPtr<Renderer>> m_render{};

    private:
        sf::RenderTexture m_real_target;
    public:
        constexpr sf::RenderTexture &m_target() { return m_real_target; }

        std::string m_game_imGuiName{};

        sf::RenderWindow *m_window = nullptr;
    } rendering;

    struct {
        uint64_t max_id = 1; ///< Max used id
        std::map<uint64_t, SceneData> data{}; ///< Loaded scenes
        //std::vector<uint64_t> index_to_id{1}; ///< data_id[]
        uint64_t active_scene = 1; ///< Now active scene
        std::unordered_map<std::thread::id, uint64_t> on_load_active_id{};

        std::unordered_set<uint64_t> dirty{}; //Mark this scene as dirty

        std::vector<TPtr<Component>> all_comoponents{};
        std::vector<TPtr<Component>> components{}; //< Global components that are running

        std::vector<TPtr<GameObject>> dont_destroy{}; //< Don't destroy theses on load
    } scene;

    struct AssetDatabaseData {
        AssetDatabaseData() = default;

        AssetDatabaseData(const AssetDatabaseData &) = delete;

        AssetDatabaseData &operator=(const AssetDatabaseData &) = delete;

        std::map<Unity::GUID, OneGUIDFile> objects{};

        std::map<std::string, FileTime> path_files{};

        std::map<std::string, std::set<std::string>> dir_tree{};
    } assets;


    struct {
        ContactListener listener;
        b2World world{{0.f, -10.f}};
        b2Body *worldBody = nullptr;

        std::vector<TPtr<Transform>> transform_dirty{};
        bool transform_lock = false;
    } physics;

    struct Settings {

        /// Save global settings
        void Save();

        /// Load global settings
        void Load();

        /// Apply global settings
        void Apply();

        float timeScale = 1;
        float maximumDeltaTime = 1.f / 5.f;
        float fixedDeltaTime = 0.02;
        sf::Vector2f gravity = {0.f, -10.f};
        std::string scene_path{};
        bool gizmo = false;
        float old_orthographicSize = 3;

        struct {
            bool fullscreen = false;
            sf::Vector2u size = {1200, 800};
            bool auto_size = true;
            uint32_t fps = 120;
            uint32_t antialiasing = 0;

            std::string icon = "icon.ico";
        } window;
    } settings;

    struct {
        bool draw_gizmo = false;

        std::vector<std::function<void()>> before_draw;
    } mis;
};

extern GlobalStaticVariables global;


#endif //RTS_GAME_GLOBALSTATICVARIABLES_H
