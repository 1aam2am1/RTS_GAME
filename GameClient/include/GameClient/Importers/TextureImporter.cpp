//
// Created by Michal_Marszalek on 02.11.2020.
//

#include <GameClient/Unity/Editor/AssetImporter.h>
#include <GameClient/Unity/Macro.h>

class TextureImporter : public AssetImporter {

};

EXPORT_IMPORTER(TextureImporter, (".bmp", ".png", ".tga", ".jpg", ".gif", ".psd", ".hdr", ".pic", -100))
