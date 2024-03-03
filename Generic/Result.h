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
