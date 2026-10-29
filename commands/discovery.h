/* clang-format off */
/*****************************************************************************
  The MIT License

  Copyright © 2026 TochkaAI

  Permission is hereby granted, free of charge, to any person obtaining
  a copy of this software and associated documentation files (the
  "Software"), to deal in the Software without restriction, including
  without limitation the rights to use, copy, modify, merge, publish,
  distribute, sublicense, and/or sell copies of the Software, and to
  permit persons to whom the Software is furnished to do so, subject to
  the following conditions:

  The above copyright notice and this permission notice shall be included
  in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*****************************************************************************/

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
