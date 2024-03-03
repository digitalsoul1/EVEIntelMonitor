#pragma once

#include <QObject>
#include <QTimer>
#include <QMap>

namespace EVEIntelMonitor {

    class QTimerPool : public QObject  {
    Q_OBJECT
    public:
        QTimerPool(QObject *parent = nullptr);
        bool addTimer(const QString& timerName, int interval, bool singleShot);
        template <typename T>
        bool registerTimeoutCallback(const QString& timerName, const QObject* context, T &&slot) {
            if (!m_qmTimers.contains(timerName)) {
                return false;
            }

            auto timer = m_qmTimers.value(timerName);
            QObject::connect(timer, &QTimer::timeout, context, slot);
            return true;

        }
        const QTimer* getRawTimer(const QString& timerName) const;
        bool startTimer(const QString& timerName, bool deleteOnTimeout = false);
        bool stopTimer(const QString& timerName);
        bool abortDeleteOnTimeout(const QString& timerName);
        bool deleteOnTimeout(const QString& timerName);
        bool timerExists(const QString& timerName) const;
        void deleteTimer(const QString& timerName);
    private:
        QMap<QString, QTimer*> m_qmTimers;
        private slots:
        void _deleteTimer(const QString& timerName);
    };

} // EVEIntelMonitor

