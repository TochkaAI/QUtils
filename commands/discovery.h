#pragma once

#include "pproto/commands/base.h"

namespace pproto {
namespace command {

//----------------------------- Список команд --------------------------------

/**
  Команда используется для поиска экземпляров TochkaCV-сервисов. Команда
  отправляется как широковещательное сообщение, так же она  используется
  в качестве ответа
*/
extern const QUuidEx CtlDiscovery;

} // namespace command

//---------------- Структуры данных используемые в сообщениях ----------------

namespace data {

struct CtlDiscovery : Data<&command::CtlDiscovery,
                            Message::Type::Answer>
{
    // Краткая информация об экземпляре MaskCtl
    QString info;

    // Идентификатор приложения времени исполнения
    QUuidEx applId;

    // Идинтификатор времени жизни приложения
    QUuidEx procId;

    // Адрес сервиса в локальной сети
    HostPoint hostPoint;

    DECLARE_B_SERIALIZE_FUNC
};

} // namespace data
} // namespace pproto


