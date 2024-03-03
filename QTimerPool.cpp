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
#include "QTimerPool.h"
#include <QDebug>

namespace EVEIntelMonitor {

        QTimerPool::QTimerPool(QObject *parent) : QObject(parent) {
        }

    const QTimer *QTimerPool::getRawTimer(const QString &timerName) const {
        return m_qmTimers.value(timerName, nullptr);
    }

    bool QTimerPool::addTimer(const QString &timerName, int interval, bool singleShot) {
        if (m_qmTimers.contains(timerName)) {
            return false;
        }

        auto timer = new QTimer(this);
        timer->setInterval(interval);
        timer->setSingleShot(singleShot);
        m_qmTimers.insert(timerName, timer);
        qInfo() << "Timer added: " << timerName;
        return true;
    }

    bool QTimerPool::startTimer(const QString &timerName, bool deleteOnTimeout) {
        if (!m_qmTimers.contains(timerName)) {
            qInfo() << "Timer not found: " << timerName;
            return false;
        }

        auto timer = m_qmTimers.value(timerName);
        if(deleteOnTimeout) {
            connect(timer, &QTimer::timeout, this, [this, timerName](){
                _deleteTimer(timerName);
            });
            qInfo() << "Timer will be deleted on timeout: " << timerName;
        }
        timer->start();
        qInfo() << "Timer started: " << timerName;
        return true;
    }

    bool QTimerPool::stopTimer(const QString &timerName) {
        if (!m_qmTimers.contains(timerName)) {
            qInfo() << "Timer not found: " << timerName;
            return false;
        }

        auto timer = m_qmTimers.value(timerName);
        timer->stop();
        qInfo() << "Timer stopped: " << timerName;
        return true;
    }

    void QTimerPool::_deleteTimer(const QString &timerName) {
        if (!m_qmTimers.contains(timerName)) {
            qInfo() << "Timer not found: " << timerName;
            return;
        }

        auto timer = m_qmTimers.value(timerName);
        timer->stop();
        m_qmTimers.remove(timerName);
        delete timer;
    }

    bool QTimerPool::abortDeleteOnTimeout(const QString &timerName) {
        auto timer = m_qmTimers.value(timerName);

        if(timer != nullptr) {
            disconnect(timer, &QTimer::timeout, this, nullptr);
            qInfo() << "Timer will not be deleted on timeout: " << timerName;
            return true;
        }
        return false;
    }

    bool QTimerPool::deleteOnTimeout(const QString &timerName) {
        auto timer = m_qmTimers.value(timerName);

        if(timer != nullptr) {

            // check if there is already a connection to deleteTimer
            connect(timer, &QTimer::timeout, this, [this, timerName]() {
                _deleteTimer(timerName);
            });
            qInfo() << "Timer will be deleted on timeout: " << timerName;
            return true;
        }

        return false;
    }

    bool QTimerPool::timerExists(const QString &timerName) const {
        return m_qmTimers.contains(timerName);
    }

    void QTimerPool::deleteTimer(const QString &timerName) {
            _deleteTimer(timerName);
    }
} // EVEIntelMonitor