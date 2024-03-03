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
