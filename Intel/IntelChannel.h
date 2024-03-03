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
