//
// Created by Michal_Marszalek on 2021-01-19.
//

#include "AudioImporter.h"
#include <GameClient/Unity/Audio/AudioClip.h>
#include <filesystem>

namespace fs = std::filesystem;

EXPORT_IMPORTER(AudioImporter, (".wav", ".ogg", ".flac", ".mp3", -220))

void AudioImporter::OnImportAsset(AssetImportContext &ctx) {
    TPtr<AudioClip> clip = std::make_shared<AudioClip>();

    auto status = fs::directory_entry(ctx.assetPath);

    if (fs::exists(status)) {
        clip->path = ctx.assetPath;
        clip->name = ctx.name;
        ctx.AddObjectToAsset(0, clip);
        ctx.SetMainObject(clip);
    }
}
