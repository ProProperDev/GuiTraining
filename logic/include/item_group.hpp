#pragma once

#include <string>

namespace Store {
class ItemGroup {
    public:
    ItemGroup(const std::string& name);
    /*TODO*/
    ~ItemGroup();
    private:
    std::string name_;    
};
} // namespace Store