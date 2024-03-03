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
#include "TurnurTheraConnections.h"
#include <QNetworkRequest>
#include <QUrlQuery>
#include <QEventLoop>
#include <QNetworkReply>
#include <QJsonArray>
#include <QJsonObject>
#include <QFuture>
#include <QtConcurrent>


// destructor
namespace EVEIntelMonitor::EveScout {
// constructor
TurnurTheraConnections::~TurnurTheraConnections() {
    if(m_pNetworkAccessManager != nullptr) {
        m_pNetworkAccessManager->deleteLater();
    }
}

TurnurTheraConnections::TurnurTheraConnections(QObject *parent) : QObject(parent) {
    m_pNetworkAccessManager = new QNetworkAccessManager(this);
}

QPair<short, unsigned int> TurnurTheraConnections::getClosestTheraConnection(unsigned long solarSystemId)  {
    return getClosestConnection(ConnectionType::Thera, solarSystemId);
}

QPair<short, unsigned int> TurnurTheraConnections::getClosestTurnurConnection(unsigned long solarSystemId) {
    return getClosestConnection(ConnectionType::Turnur, solarSystemId);
}

QPair<short, unsigned int> TurnurTheraConnections::getClosestConnection(ConnectionType connectionType, unsigned int solarSystemId)  {
    QJsonDocument jsonDocument = getEveScoutSignatures(connectionType);
    QPair<short, unsigned int> closestConnection;
    short closestDistance = -1;
    if(jsonDocument.isNull()) {
        return closestConnection;
    }

    QJsonArray jsonArray = jsonDocument.array();
    for(const auto &value : jsonArray) {
        QJsonObject jsonObject = value.toObject();

        unsigned int outSystemId = jsonObject["in_system_id"].toInt();

        auto distance = EVEIntelMonitor::RoutePlanner::getInstance()->getJumpsBetween(solarSystemId, outSystemId).size();

        // Filter out unreachable systems
        if(distance == 0) {
            if(solarSystemId != outSystemId) {
                continue;
            }
            if(solarSystemId == outSystemId) {
               closestConnection.first = 0;
               closestConnection.second = outSystemId;
               break;
            }
        }
        else if(closestDistance == -1 || static_cast<short>(distance) < closestDistance) {

            closestDistance = static_cast<short>(distance);
            closestConnection.first = closestDistance;
            closestConnection.second = outSystemId;
        }
    }

    return closestConnection;
}

QJsonDocument TurnurTheraConnections::getEveScoutSignatures(ConnectionType connectionType) {
   m_pNetworkAccessManager->clearAccessCache();
    if(m_pNetworkReply != nullptr) {
       m_pNetworkReply->close();
    }

   QNetworkRequest request;
   request.setUrl(QUrl(m_qsEveScoutEndpoint));


   switch (connectionType) {
   case ConnectionType::Turnur:
       qInfo() << "Getting Turnur connections";
       request.setUrl(request.url().toString() + "?system_name=" + "turnur");
       break;
   case ConnectionType::Thera:
       qInfo() << "Getting Thera connections";
       request.setUrl(request.url().toString() + "?system_name=" + "thera");
       break;
   }

   m_pNetworkReply = m_pNetworkAccessManager->get(request);
   qInfo() << "Waiting for Eve Scout response: " << m_pNetworkReply->url().toString();
   QEventLoop loop;
   connect(m_pNetworkReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
   loop.exec();

   if(m_pNetworkReply->error() == QNetworkReply::NoError) {
       return QJsonDocument::fromJson(m_pNetworkReply->readAll());
   } else {
       // print there was an error
       qInfo() << "Eve Scout error: " << m_pNetworkReply->errorString();
       return QJsonDocument();
   }
}

    QPair<short, unsigned int> TurnurTheraConnections::getClosestTheraConnection(const QString& solarSystemName) {
        unsigned long solarSystemId = EVEIntelMonitor::RoutePlanner::getInstance()->getSolarSystemId(solarSystemName.toStdString());
        return getClosestConnection(ConnectionType::Thera, solarSystemId);

    }

    QPair<short, unsigned int> TurnurTheraConnections::getClosestTurnurConnection(const QString& solarSystemName) {
        unsigned long solarSystemId = EVEIntelMonitor::RoutePlanner::getInstance()->getSolarSystemId(solarSystemName.toStdString());
        return getClosestConnection(ConnectionType::Turnur, solarSystemId);
    }
};
