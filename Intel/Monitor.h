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

#include <QtConcurrent/QtConcurrent>
#include "IntelChannel.h"
#include "Parser.h"
#include "PersistentSettings.h"

namespace EVEIntelMonitor::Intel {

    class Monitor : public QObject {
    Q_OBJECT

    public:
        explicit Monitor(QObject *parent = nullptr);

        Parser *getParser() const;

        void startFilesystemWatcherWorkaroundTimer();
        void stopFilesystemWatcherWorkaroundTimer();
        void addIntelChannelUser(unsigned long characterId);

        [[maybe_unused]] QMap<unsigned long, QVector<IntelChannel>> * getIntelChannelsMap();
        QVector<IntelChannel> * getIntelChannelsVector();
        bool isFirstRun();
        // destructor
        ~Monitor() override;
    public slots:
        void toggleIntelChannelEnabled(const QString& channelName, bool enabled);

    private:
        void indexIntelChannels();
        void addIntelChannel(unsigned long id, IntelChannel &&intelFile);
        void replaceIntelChannelIfNewer(unsigned long id, IntelChannel &&intelFile);
        qint64 m_intelChannelCharacterId{};
        QMap<unsigned long, QVector<IntelChannel>> m_qmIntelChannels;

        // Timers
        QTimer *m_qtFilesystemWatcherWorkaroundTimer;

        // Pointers to other classes
        Parser *m_qParser;
        QFileSystemWatcher *m_qFileSystemWatcher;
        PersistentSettings *m_qPersistentSettings;

        // Mutexes
        QMutex m_qmMutexIndexChannels;
        QMutex m_qmMutexIntelChannels;

        // variables
        bool m_bFirstRun = true;
        void setFirstRun(bool firstRun);

        signals:
        void intelChannelAdded(QString channelName);
        void intelChannelIndexingComplete();
    };

}
