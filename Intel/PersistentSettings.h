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

