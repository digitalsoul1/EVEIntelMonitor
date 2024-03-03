#pragma once
#include "Intel.h"


namespace EVEIntelMonitor::Intel {

    class IntelChannel {
    public:
        explicit IntelChannel(const QString &fileName);

        [[nodiscard]] QString getFileName() const;

        [[nodiscard]] QDateTime getModified() const;

        [[nodiscard]] QString getChannelName() const;

        void setEnabled(bool enabled);

        [[nodiscard]] QDateTime getEnabledSince() const;
        void setEnabledSince(const QDateTime &&enabledSince);

        [[nodiscard]] bool isEnabled() const;

        void getFileInfoFromFileName(const QString &fileName);

        [[nodiscard]] QDateTime getCreated() const;
        void setModified(const QDateTime &modified);

    private:
        QString m_qsFileName;
        QString m_qsChannelName;
        QDateTime m_qdtModified;
        QDateTime m_qdtCreated;
        QDateTime m_enabledSince;
        QList<Intel> m_qlIntel;
        bool m_bEnabled = true;
    };

}
