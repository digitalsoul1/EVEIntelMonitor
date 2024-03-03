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