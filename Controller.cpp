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
#include "Controller.h"

#include <QGuiApplication>

#include "Exceptions/RefreshTokenInvalidException.h"
#include "Intel/Monitor.h"
#include "IntelTableView.h"
#include "RoutePlanner/RoutePlanner.h"
#include "EveScout/TurnurTheraConnections.h"

EVEIntelMonitor::RoutePlanner *EVEIntelMonitor::RoutePlanner::m_Instance = nullptr;
EVEIntelMonitor::ConfigBackend *EVEIntelMonitor::ConfigBackend::m_pInstance = nullptr;

namespace EVEIntelMonitor {
    Controller::Controller(QObject *parent) : QObject(parent) {
        m_auth = new SSO::Auth(this);
        m_manager = new SSO::Manager(this);
        m_qtTimerPool = new QTimerPool(this);
        m_turnurTheraConnections = new EveScout::TurnurTheraConnections(this);

        m_qtTimerPool->addTimer("authTimeoutTimer", 60000, true);
        m_qtTimerPool->registerTimeoutCallback("authTimeoutTimer", this, &Controller::SSOAuthTimeout);

        m_intelMonitor = new Intel::Monitor(this);
        m_mainWindowView = new MainWindowView(this);

        m_characterLocation = nullptr;

        // SSO Signals
        // Callback server started
        connect(m_auth, &SSO::Auth::callbackServerStarted, this,
                &Controller::SSOCallbackServerStarted);
        // Callback server failed
        connect(m_auth, &SSO::Auth::callbackServerFailed, this,
                &Controller::SSOCallbackServerFailed);

        // Auth code received
        connect(m_auth, &SSO::Auth::accessTokenResponseRead, this,
                &Controller::SSOAuthTokenPayloadReceived);
        // Auth payload timed out
        connect(m_auth, &SSO::Auth::accessTokenTimeout, this,
                &Controller::SSOAuthTokenPayloadTimeout);
        // Callback server not needed anymore
        connect(this, &Controller::ssoCallBackServerNotNeeded, m_auth,
                &SSO::Auth::shutdownCallbackServer);

        // Delete character signal handler
        connect(this, &Controller::ssoIvalidRefreshToken, this, &Controller::deleteCharacter);

        connect(m_intelMonitor, &::EVEIntelMonitor::Intel::Monitor::intelChannelIndexingComplete, this, &Controller::initIntelChannelList);


        // Format QtLogger
        qSetMessagePattern(
                "[%{time yyyy-MM-dd hh:mm:ss.zzz}] [%{function}] [%{type}] %{message}");

        // checking if a user credentials are stored, if they are, we can skip the
        // auth process and start the intel monitor and parser
        if (m_manager->userCount() > 0) {
            qInfo() << "User credentials found, skipping auth process.";
            this->startIntelMonitorAndParser();
            this->createMainWindowView();
            emit appIsPostAuth();
        }
    }

    QString Controller::SSOAuthUrl() const { return m_auth->getAuthUrl(); }

    void Controller::SSOBeginAuth() {
        m_qtTimerPool->stopTimer("authTimeoutTimer");

        m_auth->startCallbackServer();
    }

    void Controller::SSOCallbackServerStarted() {
        m_qtTimerPool->startTimer("authTimeoutTimer");
        emit ssoAuthStarted();
    }

    void Controller::SSOCallbackServerFailed() {
        emit ssoAuthFailed(
                QString("Failed to start callback server. Please try again."));
        emit ssoCallBackServerNotNeeded();
    }

    void Controller::SSOAuthTimeout() {
        qInfo() << "Authentication timed out.";
        emit ssoAuthFailed(QString("Authentication timed out. Please try again."));
        emit ssoCallBackServerNotNeeded();
    }

    void Controller::SSOAuthTokenPayloadReceived(
            Generic::Result<SSO::Auth::Status, SSO::User> response) {
        QString status = SSO::Auth::authStatusToString(response.getStatus());
        if (response.getStatus() == SSO::Auth::Status::PAYLOAD_ERROR) {
            qInfo() << "Authentication failed with status: " << status;
            emit ssoAuthFailed(
                    QString("Authentication failed:" + status + ". Please try again."));
            emit ssoCallBackServerNotNeeded();
            return;
        }
        qInfo() << "Authentication finished with status: " << status;
        m_qtTimerPool->stopTimer("authTimeoutTimer");
        m_manager->storeUser(response.getData());

        QMutexLocker locker(&m_qmIsFullyInitializedMutex);

        // Main Window View has to be created first
        this->startIntelMonitorAndParser();
        this->createMainWindowView();

        auto callbackServerNotNeededTimer = m_qtTimerPool->getRawTimer("callbackServerNotNeededTimer");
        if (callbackServerNotNeededTimer == nullptr) {
            m_qtTimerPool->addTimer("callbackServerNotNeededTimer", 60000, true);
            m_qtTimerPool->registerTimeoutCallback("callbackServerNotNeededTimer", this, &::EVEIntelMonitor::Controller::ssoCallBackServerNotNeeded);
        } else
            m_qtTimerPool->stopTimer("callbackServerNotNeededTimer");

        m_qtTimerPool->startTimer("callbackServerNotNeededTimer", true);

        emit appIsPostAuth();
    }

    void Controller::SSOAuthTokenPayloadTimeout() {
        qInfo() << "Auth token payload timed out.";
        emit ssoAuthFailed(
                QString("Auth token payload timed out. Please try again."));
        emit ssoCallBackServerNotNeeded();
    }

    bool Controller::isFullyInitialized() const { return m_bIsFullyInitialized; }

    void Controller::startIntelMonitorAndParser() {
        auto characterId = m_manager->getCharactersIDs().at(0).toULongLong();
        m_intelMonitor->addIntelChannelUser(
                characterId);
        m_intelMonitor->getParser()->startParserThread();
        m_intelMonitor->startFilesystemWatcherWorkaroundTimer();
        m_bIsFullyInitialized = true;
    }

    void Controller::deleteCharacter() {
        stopThreadsAndTimers();

        m_manager->deleteUserData();
        m_qtTimerPool->deleteTimer("locationUpdateTimer");

        disconnect(m_intelMonitor->getParser(), &Intel::Parser::newIntel,
                   m_mainWindowView->getIntelTableModel(), &IntelTableView::newIntel);

        //delete m_mainWindowView;
        m_bIsFullyInitialized = false;
        m_mainWindowView->getIntelChannelsListModel()->clear();
        emit appIsPreAuth();
    }

    void Controller::createMainWindowView() {
        auto characterId = m_manager->getCharactersIDs().at(0).toULongLong();
        m_mainWindowView->setCharacterId(characterId);
        m_mainWindowView->setCharacterName(m_manager->getCharacterName(characterId));
        m_characterLocation = new SSO::CharacterLocation(m_auth, m_manager, this);
        m_mainWindowView->downloadCharacterPortrait();

        // connect parser thread with intel table view
        connect(
                m_intelMonitor->getParser(), &Intel::Parser::newIntel,
                dynamic_cast<IntelTableView *>(m_mainWindowView->getIntelTableModel()),
                &IntelTableView::newIntel);

        // Update character location immediately
        characterLocationQuery();
        toggleLocationObtained();

        m_qtTimerPool->addTimer("locationUpdateTimer", 10000, false);
        m_qtTimerPool->registerTimeoutCallback("locationUpdateTimer", this, &Controller::characterLocationQuery);
        m_qtTimerPool->startTimer("locationUpdateTimer", false);

        // Intel Channel List View signal handler
        connect(m_mainWindowView->getIntelChannelsListModel(), &IntelChannelsListView::toggleChannel, m_intelMonitor,
                &Intel::Monitor::toggleIntelChannelEnabled);

        connect(m_intelMonitor, &::EVEIntelMonitor::Intel::Monitor::intelChannelAdded, m_mainWindowView, &MainWindowView::newIntelChannelAdded);
    }

    void Controller::characterLocationQuery() {
        auto characterId = m_manager->getCharactersIDs().at(0).toULongLong();

        try {
            auto location = m_characterLocation->getLocation(
                    characterId);
            auto locationName =
                    RoutePlanner::getInstance()->getSolarSystemName(location);
            if (location != 0) {

                m_mainWindowView->setCharacterLocation(
                        QString::fromStdString(locationName));
                emit
            } else {
                m_mainWindowView->setCharacterLocation(QString("Unknown"));


                qInfo() << "Failed to get character location. Is TQ down?";
            }

        } catch (Exceptions::RefreshTokenInvalidException &e) {
            emit ssoIvalidRefreshToken();
        }
    }

    void Controller::theraConnectionsQuery() {
        if (m_bLocationObtained) {
            auto theraConnections = m_turnurTheraConnections->getClosestTheraConnection(m_mainWindowView->getCharacterLocation());
            auto turnurConnections = m_turnurTheraConnections->getClosestTurnurConnection(m_mainWindowView->getCharacterLocation());


            auto closestTheraConnectionName = RoutePlanner::getInstance()->getSolarSystemName(theraConnections.second);
            auto closestTurnurConnectionName = RoutePlanner::getInstance()->getSolarSystemName(
                    turnurConnections.second);

            m_mainWindowView->setTheraClostestConnection(theraConnections.first,
                                                         QString::fromStdString(closestTheraConnectionName));
            m_mainWindowView->setTurnurClostestConnection(turnurConnections.first,
                                                          QString::fromStdString(closestTurnurConnectionName));
        }
    }

    void Controller::toggleLocationObtained() {
        QMutexLocker locker(&m_locationUpdateMutex);
        if (m_mainWindowView->getCharacterLocation() == "Unknown" ||
            m_mainWindowView->getCharacterLocation().size() == 0) {
            m_bLocationObtained = false;
            // stop the timer
            m_qtTimerPool->stopTimer("theraConnectionsTimer");
        } else {
            if (!m_bLocationObtained) {
                // Proper location obtained, start the timer every 10 minutes
                qInfo() << "Starting Eve Scout connections timer";
                m_bLocationObtained = true;
                m_qtTimerPool->addTimer("theraConnectionsTimer", 60000, false);
                m_qtTimerPool->registerTimeoutCallback("theraConnectionsTimer", this,
                                                      &Controller::theraConnectionsQuery);

                m_qtTimerPool->startTimer("theraConnectionsTimer");
            }
        }
    }

    void Controller::stopThreadsAndTimers() {
        // stop the parser thread
        m_intelMonitor->getParser()->stopParserThread();
        // stop the filesystem watcher workaround timer
        m_intelMonitor->stopFilesystemWatcherWorkaroundTimer();
        // stop the location update timer
        m_qtTimerPool->stopTimer("locationUpdateTimer");
        // stop the thera connections timer
        m_qtTimerPool->stopTimer("theraConnectionsTimer");
        // stop the callback server not needed timer
        m_qtTimerPool->stopTimer("callbackServerNotNeededTimer");
        // stop the auth timeout timer
        m_qtTimerPool->stopTimer("authTimeoutTimer");

    }

    MainWindowView *Controller::getMainWindowView() { return m_mainWindowView; }

    Controller::~Controller() {
        delete RoutePlanner::getInstance();

        if(ConfigBackend::getInstance() != nullptr)
            ConfigBackend::getInstance()->deleteLater();
    }

    void Controller::initIntelChannelList() {
        m_mainWindowView->getIntelChannelsListModel()->initIntelChannels(m_intelMonitor->getIntelChannelsVector());
    }

} // namespace EVEIntelMonitor
