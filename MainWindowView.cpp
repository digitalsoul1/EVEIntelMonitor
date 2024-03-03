#include "MainWindowView.h"

#include <utility>
#include <QtCore/qstandardpaths.h>
#include <QFile>
#include <Utility/FileDownloader.h>
#include <QtCore/qdir.h>

#include "IntelTableView.h"

namespace EVEIntelMonitor {
    MainWindowView::MainWindowView(QObject *parent) : QObject(parent) {
        qInfo() << "MainWindowView created";
        this->m_qsCharacterName = "";

        // init the shared pointer
        // -- REPLACE WITH UNIQUE PTR --
        m_qsCharacterLocationPtr = QSharedPointer<QString>(new QString());
        *m_qsCharacterLocationPtr = "";

        m_ullCharacterID = 0;
        m_intelTableModel = new IntelTableView(this);
        m_intelChannelsListModel = new IntelChannelsListView(this);
    }


    QString MainWindowView::getCharacterName() const {
        return m_qsCharacterName;
    }

    QString MainWindowView::getCharacterLocation() const {
        return *m_qsCharacterLocationPtr;
    }

    IntelTableView *MainWindowView::getIntelTableModel() {
        return m_intelTableModel;
    }

    void MainWindowView::setCharacterName(QString characterName) {
        this->m_qsCharacterName = std::move(characterName);
    }

    void MainWindowView::setCharacterLocation(QString characterLocation) {
        if (*m_qsCharacterLocationPtr != characterLocation) {
            *m_qsCharacterLocationPtr = std::move(characterLocation);
            m_intelTableModel->setLocation(m_qsCharacterLocationPtr.toWeakRef());
            qInfo() << "Character location changed to: " << *m_qsCharacterLocationPtr;
            emit characterLocationChanged();
        }
    }

    void MainWindowView::downloadCharacterPortrait() {
        if (QFile::exists(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/portraits/" +
                          QString::number(m_ullCharacterID) + ".jpg")) {
            qInfo() << "Portrait for character id: " << m_ullCharacterID << " found in cache";

            m_qsCharacterPortraitUrl = QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/portraits/" +
                                                           QString::number(m_ullCharacterID) + ".jpg");
            setCharacterPortraitUrl(m_qsCharacterPortraitUrl);
        } else {
            auto *fileDownloader = new Utility::FileDownloader(
                    QUrl("https://images.evetech.net/characters/" + QString::number(m_ullCharacterID) +
                         "/portrait?size=256"),
                    (QObject *) this);
            connect(fileDownloader, &Utility::FileDownloader::downloaded, [this, fileDownloader]() {
                // save it to the user writable directory
                QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/portraits/");
                if (!dir.exists()) {
                    dir.mkpath(".");
                }
                QFile file(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/portraits/" +
                           QString::number(m_ullCharacterID) + ".jpg");
                if (file.open(QIODevice::WriteOnly)) {
                    file.write(fileDownloader->downloadedData());
                    file.close();
                }

                m_qsCharacterPortraitUrl = QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/portraits/" +
                                                               QString::number(m_ullCharacterID) + ".jpg");
                setCharacterPortraitUrl(m_qsCharacterPortraitUrl);
            });
        }
    }

    void MainWindowView::setCharacterId(qulonglong characterID) {
        this->m_ullCharacterID = characterID;
    }

    void MainWindowView::setTheraClostestConnection(short jumps, QString solarSystemName) {
        QMutexLocker locker(&m_qmEveScoutMutex);
        if (m_sTheraClosestConnectionJumps != jumps || m_qsTheraClosestConnectionName != solarSystemName) {
            m_sTheraClosestConnectionJumps = jumps;
            m_qsTheraClosestConnectionName = std::move(solarSystemName);
            emit newClosestTheraConnection();
        }
    }

    void MainWindowView::setTurnurClostestConnection(short jumps, QString solarSystemName) {
        QMutexLocker locker(&m_qmEveScoutMutex);
        if (m_sTurnurClosestConnectionJumps != jumps || m_qsTurnurClosestConnectionName != solarSystemName) {
            m_sTurnurClosestConnectionJumps = jumps;
            m_qsTurnurClosestConnectionName = std::move(solarSystemName);
            emit newClosestTurnurConnection();
        }
    }


    short MainWindowView::getTurnurClosestConnectionJumps() {
        return m_sTurnurClosestConnectionJumps;
    }

    QString MainWindowView::getTurnurClosestConnectionName() {
        return m_qsTurnurClosestConnectionName;
    }

    short MainWindowView::getTheraClosestConnectionJumps() {
        return m_sTheraClosestConnectionJumps;
    }

    QString MainWindowView::getTheraClosestConnectionName() {
        return m_qsTheraClosestConnectionName;
    }

    IntelChannelsListView *MainWindowView::getIntelChannelsListModel() {
        return m_intelChannelsListModel;
    }

    void MainWindowView::newIntelChannelAdded(const QString &channelName) {
        m_intelChannelsListModel->addIntelChannel(channelName);
    }

    QUrl MainWindowView::getCharacterPortraitUrl() const {
        return m_qsCharacterPortraitUrl;
    }

    void MainWindowView::setCharacterPortraitUrl(const QUrl &characterPortraitUrl) {
        m_qsCharacterPortraitUrl = characterPortraitUrl;
        emit characterPortraitReady();
    }

    MainWindowView::~MainWindowView() = default;
} // EVEIntelMonitor
