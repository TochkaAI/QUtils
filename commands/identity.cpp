#include "identity.h"

namespace pproto::data {

bool Identity::isNull() const
{
    return (id.isNull() && (index < 0) && name.isEmpty());
}

void Identity::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << id;
    stream << index;
    stream << name;
    stream << findField;
    B_SERIALIZE_RETURN
}

void Identity::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> id;
    stream >> index;
    stream >> name;
    stream >> findField;
    B_DESERIALIZE_END
}

} // namespace pproto::data
