/*
* Copyright (c) 2023 github.com/digitalsoul1
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
#include <algorithm>
#include "RoutePlanner.h"
#include "SolarSystemsMap.h"
#include "SolarSystem.h"
#include "ShortestRoute_BFS.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <iostream>

void EVEIntelMonitor::RoutePlanner::Initialize() {
    std::cout << "Initializing RoutePlanner" << std::endl;
    if (m_bInitialized)
        return;

    QJsonDocument jsonDoc;
    QFile file(":/data/solarSystems.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    if (!file.isOpen()) {
        qDebug() << "Error: cannot open file";
        return;
    }

    jsonDoc = QJsonDocument::fromJson(file.readAll());

    // Load solar systems
    for (QJsonValueRef jsonSolarSystem: jsonDoc.array()) {
        SolarSystem solarSystem;

        // Initialize solar system
        solarSystem.setId(jsonSolarSystem.toObject()["id"].toString().toULong());
        solarSystem.setName(jsonSolarSystem.toObject()["name"].toString().toStdString());
        solarSystem.setSecurity(jsonSolarSystem.toObject()["security"].toDouble());

        // Load connections
        for (QJsonValueRef jsonConnection: jsonSolarSystem.toObject()["connectedSystems"].toArray()) {
            solarSystem.addConnectedSystem(jsonConnection.toString().toULong());
        }
        m_SolarSystemsMap->addSolarSystem(std::move(solarSystem));

    }
    assert(m_SolarSystemsMap->getSolarSystemCount() > 0);
    std::cout << m_SolarSystemsMap->getSolarSystemCount() << " solar systems loaded" << std::endl;
    file.close();

    m_bInitialized = true;
}

std::vector<EVEIntelMonitor::SolarSystem *>
EVEIntelMonitor::RoutePlanner::getJumpsBetween(const std::string &start, const std::string &end) {
    if (!m_bInitialized) {
        Initialize();
    }

    auto point_a = m_SolarSystemsMap->getSolarSystem(start);
    auto point_b = m_SolarSystemsMap->getSolarSystem(end);

    if (point_a == nullptr || point_b == nullptr)
        return {};

    return EVEIntelMonitor::ShortestRoute_BFS::GetRoute(point_a, point_b);
}

std::vector<EVEIntelMonitor::SolarSystem *> EVEIntelMonitor::RoutePlanner::getJumpsBetween(unsigned long start, unsigned long end) {
    if (!m_bInitialized) {
        Initialize();
    }

    auto point_a = m_SolarSystemsMap->getSolarSystem(start);
    auto point_b = m_SolarSystemsMap->getSolarSystem(end);

    if (point_a == nullptr || point_b == nullptr)
        return {};

    return EVEIntelMonitor::ShortestRoute_BFS::GetRoute(point_a, point_b);
}

bool EVEIntelMonitor::RoutePlanner::solarSystemExists(const std::string &name) {
if (!m_bInitialized) {
        Initialize();
    }

    auto point_a = m_SolarSystemsMap->getSolarSystem(name);

    return point_a != nullptr;
}

std::string EVEIntelMonitor::RoutePlanner::getSolarSystemName(unsigned long id) const {
    if(m_SolarSystemsMap->getSolarSystem(id) == nullptr)
        return "Unknown";
    return m_SolarSystemsMap->getSolarSystem(id)->getName();
}

void EVEIntelMonitor::RoutePlanner::loadSolarSystems() {
        Initialize();
}

unsigned long EVEIntelMonitor::RoutePlanner::getSolarSystemId(const std::string& solarSystemName) {
    if (!m_bInitialized) {
        Initialize();
    }

    auto solarSystemId = m_SolarSystemsMap->getSolarSystem(solarSystemName);

    if(solarSystemId == nullptr)
        return 0;

    return solarSystemId->getId();
}

EVEIntelMonitor::RoutePlanner* EVEIntelMonitor::RoutePlanner::getInstance() {
    if(m_Instance == nullptr) {
        m_Instance = new RoutePlanner();
        m_Instance->m_SolarSystemsMap = std::make_unique<SolarSystemsMap>();
        m_Instance->loadSolarSystems();
    }
    return m_Instance;
}

EVEIntelMonitor::SolarSystemsMap *EVEIntelMonitor::RoutePlanner::getSolarSystemsMap() const {
    return m_SolarSystemsMap.get();
}


