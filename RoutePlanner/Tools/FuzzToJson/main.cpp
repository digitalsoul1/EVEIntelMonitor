/*
* Copyright (c) 2024 github.com/digitalsoul1
*
* This program is free software: you can redistribute it and/or modify
        * it under the terms of the GNU General Public License as published by
* the Free Software Foundation, version 3.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
        * General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
#include <QSqlQuery>
#include <QSqlRecord>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include "DbManager.h"
#include <QCoreApplication>
#include <iostream>
#include <QDir>

#include "../../SolarSystemsMap.h"


int main(int argc, char *argv[]) {

    QCoreApplication app (argc, argv);
    DbManager dbManager;
    auto solarSystemsMap = new HiveProtector::SolarSystemsMap();
    QSqlQuery query1("SELECT solarSystemID,solarSystemName,security FROM mapSolarSystems");

    auto t1 = std::chrono::high_resolution_clock::now();
    while (query1.next()) {
        unsigned long id = query1.value(0).toULongLong();
        std::string name = query1.value(1).toString().toStdString();
        float security = query1.value(2).toFloat();

        HiveProtector::SolarSystem solarSystem(id, name, security);
        solarSystemsMap->addSolarSystem(std::move(solarSystem));
    }
    QSqlQuery query2("SELECT fromSolarSystemID,toSolarSystemID FROM mapSolarSystemJumps");

    while (query2.next()) {
        unsigned long id = query2.value(0).toULongLong();
        unsigned long to = query2.value(1).toULongLong();

        solarSystemsMap->getSolarSystem(id)->addConnectedSystem(to);
    }

    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms"
              << std::endl;

    // Convert the SOLARYSTEMS class to JSON with QT library
    QJsonArray solarSystemsArray;

    for (auto &solarSystem: *solarSystemsMap->getSolarSystems()) {
        QJsonObject solarSystemObject;
        solarSystemObject["id"] = QString::number(solarSystem.getId());
        solarSystemObject["name"] = QString::fromStdString(solarSystem.getName());
        solarSystemObject["security"] = solarSystem.getSecurity();

        QJsonArray connectedSystemsArray;
        for (auto &connectedSystem: solarSystem.getConnectedSystems()) {
            connectedSystemsArray.append(QString::number(connectedSystem));
        }
        solarSystemObject["connectedSystems"] = connectedSystemsArray;

        solarSystemsArray.append(solarSystemObject);
    }

    QJsonDocument solarSystemsDocument(solarSystemsArray);

    QFile jsonFile("../../../Data/solarSystems.json");
    bool isOpen = jsonFile.open(QIODevice::WriteOnly | QIODevice::Text);

    // print current directory
    std::cout << "Current directory: " << QDir::currentPath().toStdString() << std::endl;
    if(!isOpen) {
        std::cout << "Error: cannot open file for writing: " << jsonFile.filesystemFileName().c_str() << std::endl;
        return 1;
    }
    jsonFile.write(solarSystemsDocument.toJson());
}
