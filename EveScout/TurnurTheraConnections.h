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
#pragma once

#include <QPair>
#include <QObject>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QMutex>
#include "../RoutePlanner/RoutePlanner.h"

namespace EVEIntelMonitor::EveScout {
enum class ConnectionType {
    Turnur,
    Thera
};

class TurnurTheraConnections : public QObject {
    Q_OBJECT
public:
    explicit TurnurTheraConnections(QObject *parent = nullptr);
    QPair<short, unsigned int> getClosestTheraConnection(unsigned long solarSystemId);
    QPair<short, unsigned int> getClosestTurnurConnection(unsigned long solarSystemId);

    QPair<short, unsigned int> getClosestTheraConnection(const QString& solarSystemName);
    QPair<short, unsigned int> getClosestTurnurConnection(const QString& solarSystemName);
    ~TurnurTheraConnections() override;
private:
    // variables
    QNetworkAccessManager *m_pNetworkAccessManager;
    QNetworkReply *m_pNetworkReply;
    const QString m_qsEveScoutEndpoint = "https://api.eve-scout.com/v2/public/signatures";
    // functions
    QJsonDocument getEveScoutSignatures(ConnectionType connectionType);
    QPair<short, unsigned int> getClosestConnection(ConnectionType connectionType, unsigned int solarSystemId);
};
}