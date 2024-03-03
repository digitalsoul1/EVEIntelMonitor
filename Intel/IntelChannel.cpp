#include "IntelChannel.h"


namespace EVEIntelMonitor::Intel {
    IntelChannel::IntelChannel(const QString &fileName) {
        m_qsFileName = fileName;
        getFileInfoFromFileName(fileName);
        m_bEnabled = false;
    }

    QString IntelChannel::getFileName() const {
        return m_qsFileName;
    }

    QDateTime IntelChannel::getModified() const {
        return m_qdtModified;
    }

    void IntelChannel::setEnabled(bool enabled) {
        if(enabled) {
            m_enabledSince = QDateTime::currentDateTime();
            m_bEnabled = true;
        } else {
            m_enabledSince = QDateTime::fromString("2000-01-01 00:00:00", "yyyy-MM-dd hh:mm:ss");
            m_bEnabled = false;
        }
    }

    QString IntelChannel::getChannelName() const {
        return m_qsChannelName;
    }

    bool IntelChannel::isEnabled() const {
        return m_bEnabled;
    }

    void IntelChannel::getFileInfoFromFileName(const QString &fileName) {
        // Time
        qsizetype pos1 = 0;
        // Date
        qsizetype pos2  = 0;
        // Channel name
        qsizetype pos3 = 0;

        for(qsizetype i = fileName.size() - 1; i >= 0; i--) {
            if (fileName.at(i) == '_') {
                if (pos1 == 0) {
                    pos1 = i;
                } else if (pos2 == 0) {
                    pos2 = i;
                } else {
                    pos3 = i;
                    break;
                }
            }
        }

        m_qsChannelName = fileName.sliced(0, pos3);
        QString date = fileName.sliced(pos3 + 1, pos2 - pos3 - 1);
        QString time = fileName.sliced(pos2 + 1, pos1 - pos2 - 1);
        m_qdtCreated = QDateTime::fromString(date + " " + time, "yyyyMMdd hhmmss");
    }

    void IntelChannel::setModified(const QDateTime &modified) {
        m_qdtModified = modified;
    }

    QDateTime IntelChannel::getCreated() const {
        return m_qdtCreated;
    }

    QDateTime IntelChannel::getEnabledSince() const {
        return m_enabledSince;
    }

    void IntelChannel::setEnabledSince(const QDateTime &&enabledSince) {
        m_enabledSince = enabledSince;
    }

} // EVEIntelMonitor
// Intel
