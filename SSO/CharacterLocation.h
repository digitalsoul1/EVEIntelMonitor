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
#pragma once

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

}
