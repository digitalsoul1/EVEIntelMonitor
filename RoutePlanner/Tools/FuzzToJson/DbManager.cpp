//
// Created by Lukasz Klepacki on 20/09/2023.
//

#include "DbManager.h"
#include <QDebug>

DbManager::DbManager() {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("/Users/lucas/Documents/Dev/CPP/HiveProtector/RoutePlanner/Tools/FuzzToJson/Data/sqlite-latest.sqlite");
    if (!m_db.open()) {
        qDebug() << "Error: connection with database fail";
    } else {
        qDebug() << "Database: connection ok";
    }
}
