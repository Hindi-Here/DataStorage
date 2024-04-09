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

    constexpr size_t Count() { return Unit.size(); }

    template<typename = enable_if_t<is_arithmetic_v<Type>>>
    constexpr double Sum() {
        static double sum;
        for (const auto& s : Unit)
            sum += s;

        return sum;
    }

    template<typename = enable_if_t<is_arithmetic_v<Type>>>
    constexpr double Average() {
        static double avg;
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

};