#include "Album.h"


Album::Album(const QString &name) :
    id_(-1),
    name_(name)
{
}

int Album::id() const
{
    return this->id_;
}

void Album::setId(int id)
{
    this->id_ = id;
}

QString Album::name() const
{
    return this->name_;
}

void Album::setName(const QString &name)
{
    this->name_ = name;
}
