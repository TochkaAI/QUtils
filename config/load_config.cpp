#include "load_config.h"

#include "shared/break_point.h"
#include "shared/logger/logger.h"
#include "shared/logger/format.h"
#include "shared/config/appl_conf.h"
#include "shared/qt/logger_operators.h"

#define log_error_m   alog::logger().error  (alog_line_location, "LoadConfig")
#define log_warn_m    alog::logger().warn   (alog_line_location, "LoadConfig")
#define log_info_m    alog::logger().info   (alog_line_location, "LoadConfig")
#define log_verbose_m alog::logger().verbose(alog_line_location, "LoadConfig")
#define log_debug_m   alog::logger().debug  (alog_line_location, "LoadConfig")
#define log_debug2_m  alog::logger().debug2 (alog_line_location, "LoadConfig")

bool loadIdentity(const YamlConfig* conf, const YAML::Node& baseNode,
                  const std::string& nodeName, pproto::data::Identity& identity)
{
    YAML::Node node = conf->node(baseNode, nodeName);
    if (!node)
        return false;

    if (node.IsNull())
    {
        identity = pproto::data::Identity();
        return true;
    }

    conf->getValue(node, "id",    identity.id,    false);
    conf->getValue(node, "index", identity.index, false);
    conf->getValue(node, "name",  identity.name , false);
    return true;
}
