//
// Created by Michal_Marszalek on 11.08.2020.
//

#include "GameAPI/StringFormatter.h"

namespace GameApi {
    CacheableOSStream::CacheableOSStream(std::string &output) : CacheableOSStream() {
        stream.setOutput(output);
    }

    std::shared_ptr<CacheableOSStream> CacheableOSStream::acquire(std::string &output) {
        thread_local CacheableOSStream cache[128];

        for (auto &elem : cache) {
            if (!elem.flag.test_and_set()) {
                elem.stream.setOutput(output);
                return std::shared_ptr<CacheableOSStream>(&elem, [](auto p) { p->flag.clear(); });
            }
        }

        return std::shared_ptr<CacheableOSStream>(new CacheableOSStream{output});
    }

}
