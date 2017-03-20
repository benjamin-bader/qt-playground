#include "DatabaseManager.h"

#include <QSqlDatabase>

DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager instance(DATABASE_FILENAME);
    return instance;
}

DatabaseManager::DatabaseManager(const QString &path) :
    db_(new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"))),
    albumDao(*db_),
    pictureDao(*db_)
{
    db_->setDatabaseName(path);
    db_->open();

    albumDao.init();
    pictureDao.init();
}

DatabaseManager::~DatabaseManager()
{
    db_->close();
    delete db_;
}
