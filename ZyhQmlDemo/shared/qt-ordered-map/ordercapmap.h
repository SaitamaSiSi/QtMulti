#ifndef ORDERCAPMAP_H
#define ORDERCAPMAP_H

#include "orderedmap.h"

template <typename Key, typename T>
class OrderCapMap
{
public:
    OrderCapMap()
    {
    }
    OrderCapMap(int capacity)
        : cap_(capacity)
    {
    }

    int capacity() const
    {
        return cap_;
    }

    void setCapacity(int capacity)
    {
        cap_ = capacity;
        if (capacity < entries.size())
        {
            typename OrderedMap<Key, T>::Iterator it = entries.begin();
            while (entries.size() > capacity)
            {
                it = entries.erase(it);
            }
        }
    }

    void clear()
    {
        entries.clear();
    }

    int size() const
    {
        return entries.size();
    }

    bool contains(Key key) const
    {
        return entries.contains(key);
    }

    void insert(Key key, T value)
    {
        entries.insert(key, value);

        if (entries.size() > cap_)
        {
            entries.erase(entries.begin());
        }
    }

    T value(Key key)
    {
        T value = entries.value(key);
        //        if (entries.contains(key)) {
        //            // Refresh entry
        //            //entries.insert(key, value);
        //        }
        return value;
    }

    void remove(Key key)
    {
        entries.remove(key);
    }

    QList<Key> keys() const
    {
        return entries.keys();
    }

private:
    int cap_;
    OrderedMap<Key, T> entries;
};

#endif  // ORDERCAPMAP_H
