//
// Created by Michal_Marszalek on 09.11.2020.
//


#include <GameClient/Unity/Editor/AssetDatabase.h>
#include <filesystem>

#undef WARN
#undef INFO

#include "catch.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"

namespace fs = std::filesystem;

TEST_CASE("AssetDatabase") {

    std::shared_ptr<int> clear((int *) 1, [](auto &&) {
        fs::remove_all("Assets/__TEST");
        fs::remove("Assets/__TEST.meta");
    });

    SECTION("Refresh") {
        REQUIRE_NOTHROW(AssetDatabase::Refresh());
    }

    SECTION("Directory create") {
        auto ret_guid = AssetDatabase::CreateFolder("Assets", "__TEST");
        auto guid = AssetDatabase::AssetPathToGUID("Assets/__TEST");
        REQUIRE(!guid.empty());
        REQUIRE(ret_guid == guid);

        auto status = fs::directory_entry("Assets/__TEST");
        auto meta_status = fs::directory_entry("Assets/__TEST.meta");
        REQUIRE(status.is_directory());
        REQUIRE(meta_status.is_regular_file());

        auto v = AssetDatabase::GetSubFolders("Assets");
        REQUIRE((std::find_if(v.begin(), v.end(), [](auto &it) { return it == "__TEST"; }) != v.end()));

        ///--delete--///

        AssetDatabase::DeleteAsset("Assets/__TEST");
        guid = AssetDatabase::AssetPathToGUID("Assets/__TEST");
        REQUIRE(guid.empty());

        status.refresh();
        meta_status.refresh();
        REQUIRE(!status.exists());
        REQUIRE(!status.exists());

        v = AssetDatabase::GetSubFolders("Assets");
        REQUIRE((std::find_if(v.begin(), v.end(), [](auto &it) { return it == "__TEST"; }) == v.end()));
    }

    SECTION("Directory with meta") {
        fs::create_directory("Assets/__TEST");
        GameApi::saveFullFile("Assets/__TEST.meta", "{\n"
                                                    "  \"DirImporter\": {\n"
                                                    "    \"m_Name\": \"\",\n"
                                                    "    \"m_userData\": \"\"\n"
                                                    "  },\n"
                                                    "  \"guid\": \"26cd8c65fff00abd987a116401201265\"\n"
                                                    "}");
        AssetDatabase::ImportAsset("Assets/__TEST");
        auto guid = AssetDatabase::AssetPathToGUID("Assets/__TEST");
        REQUIRE(guid == Unity::GUID("26cd8c65fff00abd987a116401201265"));

        AssetDatabase::DeleteAsset("Assets/__TEST");
    }
}
