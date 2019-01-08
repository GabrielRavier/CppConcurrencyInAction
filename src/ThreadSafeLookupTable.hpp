#pragma once
#include <functional>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/locks.hpp>

namespace ravier
{

template<typename Key, typename Value, typename Hash = std::hash<Key>> class ThreadSafeLookupTable
{
private:
    class bucketType
    {
    private:
        typedef std::pair<Key, Value> BucketValue;
        typedef std::list<bucketValue> BucketData;
        typedef typename bucketData::iterator BucketIterator;

        BucketData m_data;
        mutable boost::shared_mutex m_mutex;

        BucketIterator findEntryFor(const Key& key) const
        {
            return std::find_if(m_data.begin(), m_data.end(),
            [&](BucketValue const& item) 
            {
                return item.first == key;
            });
        }

    public:
        Value valueFor(const Key& key, const Value& defaultValue) const
        {
            boost::shared_lock<boost::shared_mutex> lock(m_mutex);
            const BucketIterator foundEntry = this->findEntryFor(key);
            return (foundEntry == m_data.end()) ? defaultValue : foundEntry->second;
        }
    }
}

}