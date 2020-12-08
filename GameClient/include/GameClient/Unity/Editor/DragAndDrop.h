//
// Created by Michal_Marszalek on 07.12.2020.
//

#ifndef RTS_GAME_DRAGANDDROP_H
#define RTS_GAME_DRAGANDDROP_H

#include <GameClient/TPtr.h>

class DragAndDrop {
public:

    enum DragAndDropVisualMode {
        None,    ///< No indication (drag should not be performed).
        Copy,    ///< Copy dragged objects.
        Link,    ///< Link dragged objects to target.
        Move,    ///< Move dragged objects.
        Generic,    ///< Generic drag operation.
        Rejected,    ///< Rejected drag operation.
    };

    /// The file names being dragged.
    static std::vector<std::string> paths;

    /// References to objects being dragged.
    static std::vector<TPtr<>> objectReferences;

    /// Clears drag & drop data.
    static void PrepareStartDrag();

    /// Get data associated with current drag and drop operation
    static TPtr<> GetGenericData(std::string_view type);

    /// Set data associated with current drag and drop operation.
    static void SetGenericData(std::string_view type, TPtr<> data);

    /// The visual indication of the drag.
    static DragAndDropVisualMode visualMode;

    static bool IsDragging();

    /// Accept a drag operation.
    static void AcceptDrag();

    /// Start a drag operation.
    static void StartDrag(std::string_view title);

};


#endif //RTS_GAME_DRAGANDDROP_H
