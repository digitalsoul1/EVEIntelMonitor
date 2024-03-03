/*
* Copyright (c) 2024 github.com/digitalsoul1
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

