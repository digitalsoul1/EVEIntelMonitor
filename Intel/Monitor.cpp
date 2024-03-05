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
#include <QDir>
#include "Monitor.h"

constexpr unsigned long FILE_QUERY_INTERVAL = 2000;

namespace EVEIntelMonitor::Intel {
    Monitor::Monitor(QObject *parent) : QObject(parent) {
        // Instance variables
        qInfo() << "Intel directory: " << EVEIntelMonitor::ConfigBackend::getInstance()->getEveIntelDirectory();
        m_qFileSystemWatcher = new QFileSystemWatcher(this);
        m_qFileSystemWatcher->addPath(EVEIntelMonitor::ConfigBackend::getInstance()->getEveIntelDirectory());
        m_qtFilesystemWatcherWorkaroundTimer = new QTimer(this);
        m_qParser = new Parser(this);
        m_qPersistentSettings = new PersistentSettings(this);
        setFirstRun(true);

        // QFileSystemWatcher does not work as expected for monitoring files. Thus, I have written my own workaround.
        connect(m_qtFilesystemWatcherWorkaroundTimer, &QTimer::timeout, this, [this]() {
            QMutexLocker locker(&m_qmMutexIndexChannels);
            for (auto &intelFiles: m_qmIntelChannels) {
                for (auto &intelFile: intelFiles) {
                    const QFile tempFile(EVEIntelMonitor::ConfigBackend::getInstance()->getEveIntelDirectory() + intelFile.getFileName());
                    const QFileInfo tempFileInfo(tempFile);
                    if (tempFile.exists()) {
                        if (tempFileInfo.lastModified() > intelFile.getModified()) {
                            intelFile.setModified(tempFileInfo.lastModified());

                            if (intelFile.isEnabled())
                                m_qParser->safeEnqueue(&intelFile);
                        }
                    }
                }
            }
        });


        connect(m_qFileSystemWatcher, &QFileSystemWatcher::directoryChanged, this, [this] {
            indexIntelChannels();
        });
    }

    void Monitor::indexIntelChannels() {
        QMutexLocker locker(&m_qmMutexIndexChannels);

        QDir dir(EVEIntelMonitor::ConfigBackend::getInstance()->getEveIntelDirectory());
        QStringList filters;
        filters.append("*" + QString::number(m_intelChannelCharacterId) + ".txt");
        dir.setNameFilters(filters);
        dir.setFilter(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
        QFileInfoList list = dir.entryInfoList();

        for (auto &fileInfo: list) {
            IntelChannel intelFile(fileInfo.fileName());
            intelFile.setModified(fileInfo.lastModified());
            auto channelIsEnabled = m_qPersistentSettings->isEnabled(intelFile);
            intelFile.setEnabled(channelIsEnabled);
            intelFile.setEnabledSince(QDateTime::currentDateTime());

            bool channelExists = std::any_of(m_qmIntelChannels[m_intelChannelCharacterId].begin(), m_qmIntelChannels[m_intelChannelCharacterId].end(),
                                             [&intelFile](IntelChannel &f) {
                                                 return f.getChannelName() == intelFile.getChannelName();
                                             });

            if (!channelExists)
                addIntelChannel(m_intelChannelCharacterId, std::move(intelFile));
            else
                replaceIntelChannelIfNewer(m_intelChannelCharacterId, std::move(intelFile));
        }

        if(isFirstRun()) {
            qInfo() << "First run detected. Emitting intelChannelIndexingComplete signal.";
            setFirstRun(false);
            emit intelChannelIndexingComplete();
        }
    }


    Monitor::~Monitor() {
        // Remove all paths from the filesystem watcher
        m_qFileSystemWatcher->removePaths(m_qFileSystemWatcher->directories());
        m_qFileSystemWatcher->deleteLater();

        // Stop the timer
        m_qtFilesystemWatcherWorkaroundTimer->stop();
        m_qtFilesystemWatcherWorkaroundTimer->deleteLater();

    }


    Parser *Monitor::getParser() const
    {
        return m_qParser;
    }

    void Monitor::startFilesystemWatcherWorkaroundTimer() {
        qInfo() << "Starting filesystem watcher workaround timer with interval " << FILE_QUERY_INTERVAL << " ms";
        m_qtFilesystemWatcherWorkaroundTimer->start(FILE_QUERY_INTERVAL);
    }

    void Monitor::stopFilesystemWatcherWorkaroundTimer() {
        qInfo() << "Stopping filesystem watcher workaround timer";
        if(m_qtFilesystemWatcherWorkaroundTimer != nullptr && m_qtFilesystemWatcherWorkaroundTimer->isActive())
            m_qtFilesystemWatcherWorkaroundTimer->stop();
        m_qmIntelChannels.clear();
        m_qmIntelChannels[m_intelChannelCharacterId].clear();
        qInfo() << "Intel channels map cleared. Size: " << m_qmIntelChannels.size();
    }

    void Monitor::addIntelChannelUser(unsigned long characterId) {
        m_intelChannelCharacterId = characterId;
        indexIntelChannels();
    }

    void Monitor::addIntelChannel(unsigned long id, IntelChannel &&intelFile) {
        QMutexLocker locker(&m_qmMutexIntelChannels);
        auto intelChannel = m_qmIntelChannels[id].emplaceBack(intelFile);
        emit intelChannelAdded(intelChannel.getChannelName());
    }

    [[maybe_unused]] QMap<unsigned long, QVector<IntelChannel>> * Monitor::getIntelChannelsMap() {
        QMutexLocker locker(&m_qmMutexIntelChannels);
        return &m_qmIntelChannels;
    }

    void Monitor::replaceIntelChannelIfNewer(unsigned long id, IntelChannel &&intelFile) {
        QMutexLocker locker(&m_qmMutexIntelChannels);
        for (auto &i: m_qmIntelChannels[id]) {
            if (i.getChannelName() == intelFile.getChannelName()) {
                if(i.getCreated() < intelFile.getCreated()) {
                    i = intelFile;
                }
            }
        }
    }

    QVector<IntelChannel> *Monitor::getIntelChannelsVector() {
        QMutexLocker locker(&m_qmMutexIntelChannels);
        return &m_qmIntelChannels[m_intelChannelCharacterId];
    }

    void Monitor::toggleIntelChannelEnabled(const QString& channelName, bool enabled) {
        QMutexLocker locker(&m_qmMutexIntelChannels);
        for (auto &intelFile: m_qmIntelChannels[m_intelChannelCharacterId]) {
            if (intelFile.getChannelName() == channelName) {
                intelFile.setEnabled(enabled);
                m_qPersistentSettings->saveSettings(intelFile);
                break;
            }
        }
    }

    void Monitor::setFirstRun(bool firstRun) {
        QMutexLocker locker(&m_qmMutexIntelChannels);
        m_bFirstRun = firstRun;
    }

    bool Monitor::isFirstRun() {
        QMutexLocker locker(&m_qmMutexIntelChannels);
        return m_bFirstRun;
    }
} // EVEIntelMonitor
// Intel
