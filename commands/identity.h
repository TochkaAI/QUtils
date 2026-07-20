#pragma once

#include "pproto/commands/base.h"

namespace pproto::data {

/**
  Структура используется для идентификации различных примитивов. Идентификация
  может осуществляться по одному из трех полей:  id, index, name. За выбор поля
  для идентификации примитива отвечает пользователь API
*/
struct Identity
{
    Identity() = default;
    Identity(const Identity&) = default;

    Identity(const QUuidEx& id ) : id(id),       findField(FindField::Id)    {}
    Identity(const int index   ) : index(index), findField(FindField::Index) {}
    Identity(const QString name) : name(name),   findField(FindField::Name)  {}
    Identity(const char*   name) : name(name),   findField(FindField::Name)  {}

    QUuidEx id;
    qint32  index = {-1};
    QString name;

    // Определяет поле, по которому нужно выполнять поиск/идентификацию
    enum class FindField : quint32
    {
        Id    = 0,
        Index = 1,
        Name  = 2
    };
    FindField findField = {FindField::Index};

    template<typename T> struct Find
    {
        int operator() (const QUuidEx* id, const T* item2) const
            {return QUuidEx::compare(*id, item2->identity.id);}

        int operator() (const qint32* index, const T* item2) const
            {return LIST_COMPARE_ITEM(*index, item2->identity.index);}

        int operator() (const QString* name, const T* item2) const
            {return name->compare(item2->identity.name);}

        int operator() (const Identity* item1, const T* item2) const
        {
            if (item1->findField == FindField::Id) {
                return operator() (&item1->id, item2);
            }
            else if (item1->findField == FindField::Index) {
                return operator() (&item1->index, item2);
            }
            else if (item1->findField == FindField::Name) {
                return operator() (&item1->name, item2);
            }
            return -1;
        }
    };

    bool isNull() const;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_OPT( "id"         , id        )
        J_SERIALIZE_MAP_OPT( "index"      , index     )
        J_SERIALIZE_MAP_OPT( "name"       , name      )
        J_SERIALIZE_MAP_OPT( "find_field" , findField )
    J_SERIALIZE_END
};

} // namespace pproto::data
