#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include <initializer_list>

template<class Type>
class ColumnContainer {
protected:
    vector<Type> Unit;
    void Add(const Type param) { Unit.push_back(param); }
    template <class... Types> friend class DataStore;
public:

    ColumnContainer() {}
    ColumnContainer(const initializer_list<Type> array) : Unit(array) { }
    ~ColumnContainer() {}

    typename vector<Type>::iterator begin() { return Unit.begin(); }
    typename vector<Type>::iterator end() { return Unit.end(); }

    Type& operator[](const size_t index) { return Unit[index]; }

    constexpr size_t Count() { return Unit.size(); }

    template<typename = enable_if_t<is_arithmetic_v<Type>>>
    constexpr double Sum() {
        double sum = 0;
        for (const auto& s : Unit)
            sum += s;

        return sum;
    }

    template<typename = enable_if_t<is_arithmetic_v<Type>>>
    constexpr double Average() {
        double avg = 0;
        for (const auto& s : Unit)
            avg += s;

        return avg / Unit.size();
    }

    constexpr Type Max() {
        static Type max = Unit[0];
        for (const Type& s : Unit)
            if (max < s) max = s;

        return max;
    }

    constexpr Type Min() {
        static Type min = Unit[0];
        for (const Type& s : Unit)
            if (min > s) min = s;

        return min;
    }

    constexpr Type Median() { return Unit[Unit.size() / 2]; }

    constexpr void Clear() { Unit.clear(); }

    constexpr bool Contains(const Type found) {
        return find(Unit.begin(), Unit.end(), found) != Unit.end();
    }

    constexpr bool Contains(const initializer_list<Type> found_list) {
        for (int i = 0; i < Unit.size(); i++)
            for (int j = 0; j < found_list.size(); j++)
                if (Unit[i] == *(found_list.begin() + j))
                    return true;

        return false;
    }

    constexpr int ContainsCount(const Type found) {
        return (int)count(Unit.begin(), Unit.end(), found);
    }

    constexpr int ContainsCount(const initializer_list<Type> found_list) {
        int count = 0;
        for (int i = 0; i < Unit.size(); i++)
            for (int j = 0; j < found_list.size(); j++)
                if (Unit[i] == *(found_list.begin() + j))
                    count++;

        return count;
    }

};