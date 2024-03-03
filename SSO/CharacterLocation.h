//
// Created by Lukasz Klepacki on 10/10/2023.
//

#ifndef HIVEPROTECTOR_CHARACTERLOCATION_H
#define HIVEPROTECTOR_CHARACTERLOCATION_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "Manager.h"
#include "Auth.h"

namespace EVEIntelMonitor::SSO {
    class CharacterLocation : public QObject {
    Q_OBJECT

    public:
        explicit CharacterLocation(QObject *parent = nullptr) = delete;
        explicit CharacterLocation(const Auth *auth, const Manager *manager, QObject *parent = nullptr);

        unsigned long getLocation(unsigned int characterId);

    private:
        void refreshToken(unsigned int characterId);
        Auth * getAuth();

        const SSO::Manager *m_pManager;
        const Auth *m_pAuth;
        QString m_qsLocation{};
        QMutex m_qmMutex;
    };

} // EVEIntelMonitor
// SSO

#endif //HIVEPROTECTOR_CHARACTERLOCATION_H
