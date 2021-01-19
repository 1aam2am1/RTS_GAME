//
// Created by Michal_Marszalek on 2021-01-19.
//

#include "AudioImporter.h"

EXPORT_IMPORTER(AudioImporter, (".wav", ".ogg", ".flac", ".mp3", -220))

void AudioImporter::OnImportAsset(AssetImportContext &ctx) {
    TPtr<AudioClip> clip = std::make_shared<AudioClip>();
    if (clip->buffer.loadFromFile(ctx.assetPath)) {
        clip->name = ctx.name;
        ctx.AddObjectToAsset(0, clip);
        ctx.SetMainObject(clip);
    }
}
