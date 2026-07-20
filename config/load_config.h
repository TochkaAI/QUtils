#pragma once

#include "shared/config/yaml_config.h"
#include "commands/identity.h"

bool loadIdentity(const YamlConfig* conf, const YAML::Node& baseNode,
                  const std::string& nodeName, pproto::data::Identity& identity);
