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

namespace EVEIntelMonitor::Generic {
    template<class S, class D>
    class Result {
    public:
        Result() = delete;
        Result(S sStatus, D dData);

        S getStatus() const;
        D getData();
    private:
        S m_sStatus;
        D m_dData;
    };

    template<class S, class D>
    Result<S, D>::Result(S sStatus, D dData) : m_sStatus(sStatus), m_dData(dData) {}

    template<class S, class D>
    S Result<S, D>::getStatus() const {
        return m_sStatus;
    }

    template<class S, class D>
    D Result<S, D>::getData() {
        return m_dData;
    }

}
