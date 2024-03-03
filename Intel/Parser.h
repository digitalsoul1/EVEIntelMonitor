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
#include <QtCore/qqueue.h>
#include <QtCore/qfuture.h>
#include "IntelChannel.h"
#include "RoutePlanner/RoutePlanner.h"
#include "../SSO/CharacterLocation.h"

namespace EVEIntelMonitor::Intel {
    class Parser : public QObject {
    Q_OBJECT


    public:
        explicit Parser(QObject *parent = nullptr);

        void startParserThread();
        void stopParserThread();

        void safeEnqueue(const IntelChannel * intelFile);

        ~Parser() override;

    private:
        IntelChannel* safeDequeue();

        bool m_bParserThreadIsNeeded;
        QQueue<IntelChannel *> m_qqIntelFileQueue;
        QMutex m_qmParserThreadMutex;
    signals:
        void newIntel(QString channelName, QDateTime intelTime, QString solarSystemName, short jumpsOut, QString originalMessage, QString characterName);
    };

}
