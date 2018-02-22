#include "deviceviewconfig.h"

DeviceViewConfig::DeviceViewConfig(const std::string &name,
    const std::string &descrption, const std::vector<DrawingConfig> &drawing)
    :_name(name), _description(descrption), _drawing(drawing) {}
