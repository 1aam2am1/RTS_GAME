//
// Created by Michal_Marszalek on 15.10.2020.
//

#include "Selection.h"

TPtr<GameObject> Selection::activeGameObject{};

TPtr<Object> Selection::activeObject{};

TPtr<Transform> Selection::activeTransform{};

std::vector<Unity::GUID> Selection::assetGUIDs{};
