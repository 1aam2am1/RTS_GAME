//
// Created by Michal_Marszalek on 19.10.2020.
//

#ifndef RTS_GAME_TEXTASSET_H
#define RTS_GAME_TEXTASSET_H

#include <GameClient/Unity/Core/Object.h>
#include <string>
#include <string_view>

class TextAsset : public Object {
public:
    /// Create a new TextAsset with the specified text contents.
    /// \details This constructor creates a TextAsset, which is not the same as a plain text file. When saved to disk
    /// using the AssetDatabase class, the TextAsset should be saved with the .asset extension.
    /// \param text The text contents for the TextAsset.
    TextAsset(std::string_view text);

    /// The raw bytes of the text asset.
    ///const std::vector<uint8_t> bytes;

    /// The text contents of the .txt file as a string.
    const std::string text;


};


#endif //RTS_GAME_TEXTASSET_H
