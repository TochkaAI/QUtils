#include "discovery.h"
#include "pproto/commands/pool.h"

namespace pproto {
namespace command {

#define REGISTRY_COMMAND_SINGLPROC(COMMAND, UUID) \
    const QUuidEx COMMAND = command::Pool::Registry{UUID, #COMMAND, false};

#define REGISTRY_COMMAND_MULTIPROC(COMMAND, UUID) \
    const QUuidEx COMMAND = command::Pool::Registry{UUID, #COMMAND, true};

REGISTRY_COMMAND_SINGLPROC(CtlDiscovery, "8c44be36-a2cf-4311-aeee-b63d198247ed")

#undef REGISTRY_COMMAND_SINGLPROC
#undef REGISTRY_COMMAND_MULTIPROC

} // namespace command

namespace data {

void CtlDiscovery::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << info;
    stream << applId;
    stream << procId;
    stream << hostPoint;
    B_SERIALIZE_RETURN
}

void CtlDiscovery::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> info;
    stream >> applId;
    stream >> procId;
    stream >> hostPoint;
    B_DESERIALIZE_END
}

} // namespace data
} // namespace pproto
