#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include <initializer_list>

using namespace std;

template<class Type>
class ColumnContainer {
private:
	vector<Type> Unit;
	void Add(const Type param) { Unit.push_back(param); }
	template <class... Types> friend class DataStore;
	using if_t = enable_if_t<is_arithmetic_v<Type>>;

public:
	ColumnContainer() = default;
	constexpr ColumnContainer(const initializer_list<Type>& array) : Unit(array) {}
	~ColumnContainer() = default;

	constexpr size_t Count() const { return Unit.size(); }

	template<typename = if_t>
	constexpr double Sum() const {
		double sum = 0;
		for (const auto& s : Unit)
			sum += s;

		return sum;
	}

	template<typename = if_t>
	constexpr double Average() {
		double avg = 0;
		for (const auto& s : Unit)
			avg += s;

		return avg / Unit.size();
	}

	constexpr Type Max() const {
		return *max_element(Unit.begin(), Unit.end());
	}

	constexpr Type Min() const {
		return *min_element(Unit.begin(), Unit.end());
	}

	constexpr Type Median() const {
		return Unit[Unit.size() / 2];
	}

	constexpr bool Contains(const Type found) const {
		return find(Unit.begin(), Unit.end(), found) != Unit.end();
	}

	constexpr bool Contains(const initializer_list<Type>& found_list) const {
		return std::any_of(found_list.begin(), found_list.end(),
			[this](const Type& found) { return Contains(found); });
	}

	constexpr int ContainsCount(const Type found) const {
		return count(Unit.begin(), Unit.end(), found);
	}

	constexpr int ContainsCount(const initializer_list<Type>& found_list) const {
		int count = 0;
		for (const auto& s : found_list)
			count += ContainsCount(s);

		return count;
	}
};