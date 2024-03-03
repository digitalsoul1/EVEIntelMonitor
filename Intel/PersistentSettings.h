#pragma once

#include <QObject>
#include "IntelChannel.h"
#include "ConfigBackend/ConfigBackend.h"

namespace EVEIntelMonitor::Intel {

    class PersistentSettings : public QObject {
    Q_OBJECT

    public:
        explicit PersistentSettings(QObject *parent = nullptr);

        void saveSettings(IntelChannel &intelChannel);

        bool isEnabled(const IntelChannel &intelChannel);

        void clearSettings();

        QVector<IntelChannel> *loadSettings();

    private:
        unsigned long m_ulCharacterId;
        ConfigBackend *m_pConfigBackend;

        // Mutexes
        QMutex m_qmMutexSettings;
    };

} // Intel
// EVEIntelMonitor

