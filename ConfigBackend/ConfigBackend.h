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
#include <QSettings>
#include <QMutexLocker>

namespace EVEIntelMonitor {
    class ConfigBackend : public QObject {
    Q_OBJECT
    public:
        static ConfigBackend *getInstance(QObject *parent = nullptr);
        void setValue(const QString& key, const QVariant& value);
        QVariant getValue(const QString& key, const QVariant& defaultValue = QVariant());
        QStringList getGroups(const QString& group);
        void removeGroup(const QString& group);
        [[nodiscard]] long long groupCount(const QString &key);
        [[maybe_unused]] void wipe();
        QString getEveIntelDirectory();
    private:
        explicit ConfigBackend(QObject *parent = nullptr);
        static ConfigBackend *m_pInstance;
        QSettings *m_pSettings;
        QMutex m_mutex;
        QMutex m_eveIntelDirectoryMutex;
        const QString EVE_INTEL_DIRECTORY;
    };


}
