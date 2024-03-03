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
#include "ConfigBackend.h"

namespace EVEIntelMonitor {
    ConfigBackend::ConfigBackend(QObject *parent) : QObject(parent) {
        m_pSettings = new QSettings(QSettings::NativeFormat,QSettings::UserScope, "github.digitalsoul1", "EVEIntelMonitor", this);
    }

    void ConfigBackend::setValue(const QString &key, const QVariant &value) {
        QMutexLocker locker(&m_mutex);
        m_pSettings->setValue(key, value);
    }

    QVariant ConfigBackend::getValue(const QString &key, const QVariant &defaultValue) {
        QMutexLocker locker(&m_mutex);
        return m_pSettings->value(key, defaultValue);
    }

    QStringList ConfigBackend::getGroups(const QString &group) {
        QMutexLocker locker(&m_mutex);
        m_pSettings->beginGroup(group);
        QStringList keys = m_pSettings->childGroups();
        m_pSettings->endGroup();
        return keys;
    }

    long long ConfigBackend::groupCount(const QString &key) {
        QMutexLocker locker(&m_mutex);
        m_pSettings->beginGroup(key);
        long long count = m_pSettings->childGroups().count();
        m_pSettings->endGroup();
        return count;
    }

    [[maybe_unused]] void ConfigBackend::wipe() {
        QMutexLocker locker(&m_mutex);
        m_pSettings->clear();
    }

    void ConfigBackend::removeGroup(const QString &group) {
        QMutexLocker locker(&m_mutex);
        m_pSettings->remove(group);
    }

    ConfigBackend *ConfigBackend::getInstance(QObject *parent) {
        if(m_pInstance == nullptr) {
            m_pInstance = new ConfigBackend(parent);
            return m_pInstance;
        }

        return m_pInstance;
    }

} // EVEIntelMonitor
// ConfigBackend