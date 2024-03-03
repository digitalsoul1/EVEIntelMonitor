#pragma once
#include <QQueue>


namespace EVEIntelMonitor::Intel {
    template<class T>
    class QQueueSizeConstrained {
    public:
        explicit QQueueSizeConstrained(int maxSize) {
            m_qqQueue.reserve(maxSize);
        }

        T dequeue() {
            return m_qqQueue.dequeue();
        }

        void enqueue(const T &&t) {
            if (m_qqQueue.size() == m_qqQueue.capacity()) {
                m_qqQueue.dequeue();
            }
            m_qqQueue.enqueue(t);
        }

        [[nodiscard]] bool isEmpty() const {
            return m_qqQueue.isEmpty();
        }

        [[nodiscard]] int size() const {
            return m_qqQueue.size();
        }

        [[nodiscard]] int capacity() const {
            return m_qqQueue.capacity();
        }

        [[nodiscard]] T first() const {
            return m_qqQueue.first();
        }

        [[nodiscard]] T last() const {
            return m_qqQueue.last();
        }

        [[nodiscard]] T at(int i) const {
            return m_qqQueue.at(i);
        }

        [[nodiscard]] T operator[](int i) const {
            return m_qqQueue[i];
        }

        void clear() {
            m_qqQueue.clear();
        }

        // begin
        [[nodiscard]] typename QQueue<T>::iterator begin() {
            return m_qqQueue.begin();
        }

        [[nodiscard]] typename QQueue<T>::const_iterator begin() const {
            return m_qqQueue.begin();
        }

        [[nodiscard]] typename QQueue<T>::const_iterator cbegin() const {
            return m_qqQueue.cbegin();
        }

        // end
        [[nodiscard]] typename QQueue<T>::iterator end() {
            return m_qqQueue.end();
        }

        [[nodiscard]] typename QQueue<T>::const_iterator end() const {
            return m_qqQueue.end();
        }

        [[nodiscard]] typename QQueue<T>::const_iterator cend() const {
            return m_qqQueue.cend();
        }

    private:
        QQueue<T> m_qqQueue;
    };

}
