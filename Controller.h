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
#include <QQmlEngine>
#include "SSO/Auth.h"
#include "SSO/Manager.h"
#include "Intel/Monitor.h"
#include "MainWindowView.h"
#include "EveScout/TurnurTheraConnections.h"
#include "Intel/PersistentSettings.h"
#include "QTimerPool.h"

namespace EVEIntelMonitor {
    class Controller : public QObject {
    Q_OBJECT

        QML_ELEMENT
    public:
        explicit Controller(QObject *parent = nullptr);

        // SSO Specific
        Q_INVOKABLE void SSOBeginAuth();
        Q_INVOKABLE QString SSOAuthUrl() const;

        // GUI specific
        Q_INVOKABLE  bool isFullyInitialized() const;
        Q_INVOKABLE void deleteCharacter();
        Q_INVOKABLE MainWindowView * getMainWindowView();

        // destructor
        ~Controller() override;

    public slots:
        void SSOCallbackServerStarted();
        void SSOCallbackServerFailed();
        void SSOAuthTokenPayloadReceived(Generic::Result<SSO::Auth::Status, SSO::User> response);
        void SSOAuthTimeout();
        void SSOAuthTokenPayloadTimeout();
        void theraConnectionsQuery();

    private:
        // classes
        SSO::Auth *m_auth;
        SSO::Manager *m_manager;
        Intel::Monitor *m_intelMonitor;
        EveScout::TurnurTheraConnections *m_turnurTheraConnections;
        QTimerPool *m_qtTimerPool;

        bool m_pEveScoutTimerFirstRun = true;

        SSO::CharacterLocation *m_characterLocation{};
        bool m_bIsFullyInitialized = false;

        // Mutexes
        QMutex m_qmIsFullyInitializedMutex;
        QMutex m_locationUpdateMutex;
        QMutex m_eveScoutQueryTimeMutex;
        bool m_bLocationObtained = false;

        // Views
        MainWindowView *m_mainWindowView{};

        // functions
        void startIntelMonitorAndParser();
        void createMainWindowView();

        // timer specific functions
        void characterLocationQuery();
        void toggleLocationObtained();

        // cleanup
        void stopThreadsAndTimers();

    signals:
        void ssoAuthStarted();
        void ssoAuthFailed(QString reason);
        void ssoIvalidRefreshToken();
        void ssoCallBackServerNotNeeded();
        void appIsPostAuth();
        void appIsPreAuth();
    };

}
