#pragma once

#include <QtConcurrent/QtConcurrent>
#include "IntelChannel.h"
#include "Parser.h"
#include "PersistentSettings.h"

#ifdef __APPLE__
#define EVE_INTEL_DIR "/Users/lucas/Documents/EVE/logs/Chatlogs/"
#endif

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

        signals:
        void intelChannelAdded(QString channelName);
    };

}
