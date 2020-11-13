//
// Created by Michal_Marszalek on 15.10.2020.
//

#include "Selection.h"

TPtr<GameObject> Selection::activeGameObject{nullptr};

TPtr<Object> Selection::activeObject{nullptr};

TPtr<Transform> Selection::activeTransform{nullptr};

std::vector<Unity::GUID> Selection::assetGUIDs;
