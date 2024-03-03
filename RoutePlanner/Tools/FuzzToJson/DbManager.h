//
// Created by Lukasz Klepacki on 20/09/2023.
//

#ifndef FUZZTOJSON_DBMANAGER_H
#define FUZZTOJSON_DBMANAGER_H

#include <QtSql/QSqlDatabase>


class DbManager {
public:
    DbManager();

private:
    QSqlDatabase m_db;
};


#endif //FUZZTOJSON_DBMANAGER_H
