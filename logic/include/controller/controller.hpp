#pragma once

#include <vector>

namespace Controller {
    class OfflineWindowController {
    public:
        OfflineWindowController() = default;

    private:
        std::vector <Data::Channel> channels_;
    };
} // namespace Controller