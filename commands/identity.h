#pragma once

#include "shared/logger/logger.h"
#include "pproto/commands/base.h"

#include <tuple>

namespace pproto::data {

/**
  Структура используется для идентификации различных примитивов. Идентификация
  может осуществляться по одному или паре полей:  id,  index,  name.  За выбор
  полей для идентификации примитива отвечает пользователь API
*/
struct Identity
{
    Identity() = default;
    Identity(const Identity&) = default;

    Identity(const QUuidEx& id ) : id(id),       findField(FindField::Id)    {}
    Identity(const int index   ) : index(index), findField(FindField::Index) {}
    Identity(const QString name) : name(name),   findField(FindField::Name)  {}
    Identity(const char*   name) : name(name),   findField(FindField::Name)  {}

    Identity(const QUuidEx& id, int index)
        : id(id), index(index), findField(FindField::IdIndex)
    {}

    Identity(const QUuidEx& id, const QString& name)
        : id(id), name(name), findField(FindField::IdName)
    {}
    Identity(const QUuidEx& id, const char* name)
        : id(id), name(name), findField(FindField::IdName)
    {}

    Identity(int index, const QString name)
        : index(index), name(name), findField(FindField::IndexName)
    {}
    Identity(int index, const char* name)
        : index(index), name(name), findField(FindField::IndexName)
    {}

    QUuidEx id;
    qint32  index = {-1};
    QString name;

    // Определяет поля по которым нужно выполнять поиск/идентификацию
    enum class FindField : quint32
    {
        Id        = 0, // По полю id
        Index     = 1, // По полю index
        Name      = 2, // По полю name
        IdIndex   = 3, // По полям id,index
        IdName    = 4, // По полям id,name
        IndexName = 5  // По полям index,name
    };
    FindField findField = {FindField::Index};

    template<typename T> struct Find
    {
        // FindField::Id
        int operator() (const QUuidEx* id, const T* item2) const
            {return QUuidEx::compare(*id, item2->identity.id);}

        // FindField::Index
        int operator() (const qint32* index, const T* item2) const
            {return LIST_COMPARE_ITEM(*index, item2->identity.index);}

        // FindField::Name
        int operator() (const QString* name, const T* item2) const
            {return name->compare(item2->identity.name);}

        // FindField::IdIndex
        int operator() (const std::tuple<QUuidEx /*id*/, qint32 /*index*/>* item1,
                        const T* item2) const
        {
            if (int res = QUuidEx::compare(std::get<0>(*item1), item2->identity.id))
                return res;
            return LIST_COMPARE_ITEM(std::get<1>(*item1), item2->identity.index);
        }

        // FindField::IdName
        int operator() (const std::tuple<QUuidEx /*id*/, QString /*name*/>* item1,
                        const T* item2) const
        {
            if (int res = QUuidEx::compare(std::get<0>(*item1), item2->identity.id))
                return res;
            return QString::compare(std::get<1>(*item1), item2->identity.name);
        }

        // FindField::IndexName
        int operator() (const std::tuple<qint32 /*index*/, QString /*name*/>* item1,
                        const T* item2) const
        {
            LIST_COMPARE_MULTI_ITEM(std::get<0>(*item1), item2->identity.index);
            return QString::compare(std::get<1>(*item1), item2->identity.name);
        }

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
            else if (item1->findField == FindField::IdIndex) {
                std::tuple t {item1->id, item1->index};
                return operator() (&t, item2);
            }
            else if (item1->findField == FindField::IdName) {
                std::tuple t {item1->id, item1->name};
                return operator() (&t, item2);
            }
            else if (item1->findField == FindField::IndexName) {
                std::tuple t {item1->index, item1->name};
                return operator() (&t, item2);
            }
            return -1;
        }
    };

    bool isNull() const;
    QString toString() const;

    DECLARE_B_SERIALIZE_FUNC
    DECLARE_J_SERIALIZE_FUNC

    // J_SERIALIZE_BEGIN
    //     J_SERIALIZE_MAP_OPT( "id"         , id        )
    //     J_SERIALIZE_MAP_OPT( "index"      , index     )
    //     J_SERIALIZE_MAP_OPT( "name"       , name      )
    //     J_SERIALIZE_MAP_OPT( "find_field" , findField )
    // J_SERIALIZE_END
};

template<typename This, typename Packer>
Packer& Identity::jserialize(const This* ct, Packer& p)
{
    This* t = const_cast<This*>(ct);

    p.startObject();

    if (p.isWriter())
    {
        if (!t->id.isNull())
            J_SERIALIZE_MAP_ITEM( "id", id )

        if (t->index >= 0)
            J_SERIALIZE_MAP_ITEM( "index", index )

        if (!t->name.isEmpty())
            J_SERIALIZE_MAP_ITEM( "name", name )

        if (!t->isNull())
            J_SERIALIZE_MAP_ITEM( "find_field", findField )
    }
    else if (p.isReader())
    {
        J_SERIALIZE_MAP_OPT( "id"         , id        )
        J_SERIALIZE_MAP_OPT( "index"      , index     )
        J_SERIALIZE_MAP_OPT( "name"       , name      )
        J_SERIALIZE_MAP_OPT( "find_field" , findField )
    }
    return p.endObject();
}

} // namespace pproto::data

namespace alog {

Line& operator<< (Line&, const pproto::data::Identity&);

} // namespace alog
