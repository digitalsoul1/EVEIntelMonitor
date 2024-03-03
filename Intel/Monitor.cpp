#include <QDir>
#include "Monitor.h"

const unsigned long FILE_QUERY_INTERVAL = 2000;


namespace EVEIntelMonitor::Intel {
    Monitor::Monitor(QObject *parent) : QObject(parent) {
        // Instance variables
        auto documentsDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
        auto intelDir = documentsDir + "/EVE/logs/Chatlogs/";
        qInfo() << "Intel directory: " << intelDir;
        m_qFileSystemWatcher = new QFileSystemWatcher(this);
        m_qFileSystemWatcher->addPath(intelDir);
        m_qtFilesystemWatcherWorkaroundTimer = new QTimer(this);
        m_qParser = new Parser(this);
        m_qPersistentSettings = new PersistentSettings(this);

        // QFileSystemWatcher does not work as expected for monitoring files. Thus, I have written my own workaround.
        connect(m_qtFilesystemWatcherWorkaroundTimer, &QTimer::timeout, this, [this]() {
            QMutexLocker locker(&m_qmMutexIndexChannels);
            for (auto &intelFiles: m_qmIntelChannels) {
                for (auto &intelFile: intelFiles) {
                    const QFile tempFile(EVE_INTEL_DIR + intelFile.getFileName());
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
        QDir dir(EVE_INTEL_DIR);
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
} // EVEIntelMonitor
// Intel
