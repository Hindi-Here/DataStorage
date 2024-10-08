#pragma once
#include <tuple>
#include <sstream>
#include <functional>

#include <list>
#include <set>
#include <deque>

#include "ColumnContainer.h"

using namespace std;

template <class... Types>
class DataStore {
protected:
	vector<tuple<Types...>> Unit;
	vector<tuple<Types...>> Unit_save = Unit;

	string GetLine_(size_t line) {
		stringstream SS;
		apply([&SS](const auto&... args) {
			((SS << args << ' '), ...);
			}, Unit[line]);

		return SS.str();
	}

	static auto CompareNote() { return [](const auto& a, const auto& b) { return a < b; }; }

	template<size_t Column>
	auto Compare() const
	{
		return [](const auto& a, const auto& b) { return get<Column>(a) < get<Column>(b); };
	}

	template<size_t Column>
	auto Equals_Compare() const
	{
		return [](const auto& a, const auto& b) { return get<Column>(a) == get<Column>(b); };
	}

public:

	DataStore() = default;
	DataStore(const size_t size) : Unit(size) {}
	DataStore(const Types... type) { Unit.push_back({ type... }); }
	DataStore(const initializer_list<tuple<Types...>>& type) : Unit(type) {}

	~DataStore() = default;

	typename vector<tuple<Types...>>::iterator begin() { return Unit.begin(); }
	typename vector<tuple<Types...>>::iterator end() { return Unit.end(); }

	tuple<Types...>& operator[](const size_t index) { return Unit[index]; }

	constexpr size_t LineCount() { return Unit.size(); }
	static constexpr size_t ColumnCount() { return sizeof...(Types); }
	DataStore& Remember() { Unit_save = Unit; return *this; }
	DataStore& Resize(size_t size) { Unit.resize(size); return *this; }
	DataStore& Reverse() { reverse(Unit.begin(), Unit.end()); return *this; }
	DataStore& Restore() { Unit = Unit_save; return *this; }
	constexpr void Clear() { Unit.clear(); }

	template<size_t Line, size_t Column>
	constexpr auto GetElement() { return get<Column>(Unit[Line]); }

	constexpr string First() { return GetLine_(0); }
	constexpr string Last() { return _GetLine_(Unit.size() - 1); }

	template<size_t Line>
	constexpr string GetLine() {
		return GetLine_(Line);
	}

	template<size_t Column>
	constexpr string GetColumn() {
		stringstream SS;
		for (const auto& S : Unit)
			SS << get<Column>(S) << '\n';

		return SS.str();
	}

	template<size_t Column>
	constexpr string GetType() {
		string type = typeid(decltype(get<Column>(Unit[0]))).name();
		type == "class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >" ?
			type = "string" : type = typeid(decltype(get<Column>(Unit[0]))).name();

		return type;
	}

	DataStore& AddNote(const Types... note) {
		Unit.push_back({ note... });
		return *this;
	}

	DataStore& AddNote(const tuple<Types...> note) {
		Unit.push_back(note);
		return *this;
	}

	DataStore& AddNote(const initializer_list<tuple<Types...>>& note_list) {
		for (const auto& s : note_list)
			Unit.push_back(s);

		return *this;
	}

	template<typename type>
	DataStore<Types..., type> AddColumn(const type Fill) {
		vector<tuple<Types..., type>> newUnit;
		for (const auto& s : Unit)
			newUnit.push_back(tuple_cat(s, make_tuple(Fill)));

		DataStore<Types..., type> newDataStore;
		for (const auto& s : newUnit)
			newDataStore.AddNote(s);

		return newDataStore;
	}

	template<size_t Line, size_t Column>
	DataStore& ChangeElement(const auto& notePart) {
		get<Column>(this->Unit[Line]) = notePart;
		return *this;
	}

	template<size_t Line>
	DataStore& ChangeNote(DataStore& variable_note) {
		Unit[Line] = variable_note[0];
		return *this;
	}

	template<size_t Line>
	DataStore& ChangeNote(DataStore& variable_note, const size_t index) {
		Unit[Line] = variable_note[index];
		return *this;
	}

	template<size_t Line>
	DataStore& ChangeNote(const Types... note) {
		Unit[Line] = { note... };
		return *this;
	}

	template<size_t Column>
	DataStore& ChangeColumn(initializer_list<decay_t<decltype(get<Column>(Unit[0]))>> list) {
		vector<decay_t<decltype(get<Column>(Unit[0]))>> vector_array = list;
		for (int i = 0; i < vector_array.size(); i++)
			get<Column>(this->Unit[i]) = vector_array[i];

		return *this;
	}

	template<size_t Column>
	DataStore& ChangeColumn(vector<decay_t<decltype(get<Column>(Unit[0]))>> list) {
		for (int i = 0; i < list.size(); i++)
			get<Column>(this->Unit[i]) = list[i];

		return *this;
	}

	template<size_t Column>
	DataStore& ChangeColumn(ColumnContainer<decay_t<decltype(get<Column>(Unit[0]))>> variable_array) {
		for (int i = 0; i < variable_array.Unit.size(); i++)
			get<Column>(this->Unit[i]) = variable_array[i];

		return *this;
	}

	DataStore& ChangeColumn(function<void(tuple<Types...>&)> change) {
		for (auto& A : Unit)
			change(A);

		return  *this;
	}

	template<size_t Column>
	DataStore& ChangeColumn(function<void(decay_t<decltype(get<Column>(Unit[0]))>&)> change) {
		for (auto& A : Unit)
			change(get<Column>(A));

		return  *this;
	}

	template<size_t Line>
	DataStore& DeleteNote() {
		Unit.erase(Unit.begin() + Line);
		return *this;
	}

	DataStore& DeleteNote(const size_t index) {
		Unit.erase(Unit.begin() + index);
		return *this;
	}

	DataStore& DeleteNote(const Types... note) {
		Unit.erase(remove_if(Unit.begin(), Unit.end(), [&](const auto& a) {
			return a == make_tuple(note...);
			}), Unit.end());

		return *this;
	}

	DataStore& DeleteNote(DataStore& variable_note) {
		Unit.erase(remove_if(Unit.begin(), Unit.end(), [&](const tuple<Types...>& a) {
			for (int j = 0; j < variable_note.LineCount(); j++)
				if (a == variable_note.Unit[j])
					return true;

			return false;
			}), Unit.end());

		return *this;
	}

	DataStore& DeleteNote(DataStore& variable_note, const size_t index) {
		Unit.erase(remove(Unit.begin(), Unit.end(), variable_note.Unit[index]), Unit.end());
		return *this;
	}

	DataStore& OrderBy() {
		sort(Unit.begin(), Unit.end(), CompareNote());
		return *this;
	}

	template<size_t Column>
	DataStore& OrderBy() {
		sort(Unit.begin(), Unit.end(), Compare<Column>());
		return *this;
	}

	DataStore& OrderByDescending() {
		sort(Unit.begin(), Unit.end(), [](const auto& a, const auto& b) { return a > b; });
		return *this;
	}

	template<size_t Column>
	DataStore& OrderByDescending() {
		auto compare = [](const auto& a, const auto& b) { return get<Column>(a) > get<Column>(b); };
		sort(Unit.begin(), Unit.end(), compare);
		return *this;
	}

	DataStore& Union(DataStore& variable_note) {
		for (int i = 0; i < variable_note.LineCount(); i++)
			Unit.push_back(variable_note[i]);

		return *this;
	}

	DataStore& Union(DataStore& variable_note, const size_t index) {
		Unit.push_back(variable_note[index]);
		return *this;
	}

	DataStore& Unique() {
		set<tuple<Types...>> unique_unit;
		for (const auto& s : Unit)
			unique_unit.insert(s);

		Unit.clear();
		for (const auto& s : unique_unit)
			Unit.push_back(s);

		return *this;
	}

	template<size_t Column>
	DataStore& Unique() {
		sort(Unit.begin(), Unit.end(), Compare<Column>());
		Unit.erase(unique(Unit.begin(), Unit.end(), Equals_Compare<Column>()), Unit.end());
		return *this;
	}

	DataStore& Repeat() {
		sort(Unit.begin(), Unit.end(), CompareNote());

		set<tuple<Types...>> unique_unit;
		for (int i = 0; i < Unit.size() - 1; i++)
			if (Unit[i] == Unit[i + 1])
				unique_unit.insert(Unit[i]);

		Unit.clear();
		for (const auto& s : unique_unit)
			Unit.push_back(s);

		return *this;
	}

	template<size_t Column>
	DataStore& Repeat() {
		sort(Unit.begin(), Unit.end(), Compare<Column>());

		vector<tuple<Types...>> newUnit;
		for (int i = 0; i < Unit.size() - 1; i++)
			if (get<Column>(Unit[i]) == get<Column>(Unit[i + 1]))
				newUnit.push_back(Unit[i]);

		Unit.clear();
		for (const auto& s : newUnit)
			Unit.push_back(s);

		Unit.erase(unique(Unit.begin(), Unit.end(), Equals_Compare<Column>()), Unit.end());

		return *this;
	}

	DataStore& Take(const size_t size) {
		vector<tuple<Types...>> newUnit = Unit;
		Unit.clear();
		for (int i = 0; i < size; i++)
			Unit.push_back(newUnit[i]);

		return *this;
	}

	DataStore& Take(const size_t start, const size_t end) {
		vector<tuple<Types...>> newUnit = Unit;
		Unit.clear();
		for (size_t i = start; i < end; i++)
			Unit.push_back(newUnit[i]);

		return *this;
	}

	DataStore& Where(function<bool(tuple<Types...>&)> sorted) {
		Unit.erase(remove_if(Unit.begin(), Unit.end(), [&](tuple<Types...>& A) {
			return !sorted(A);
			}), Unit.end());

		return *this;
	}

	template<size_t Column>
	DataStore& Where(function<bool(decay_t<decltype(get<Column>(Unit[0]))>)> sorted) {
		Unit.erase(remove_if(Unit.begin(), Unit.end(), [&](tuple<Types...>& A) {
			return !sorted(get<Column>(A));
			}), Unit.end());

		return *this;
	}

	DataStore& Where_i(function<bool(tuple<Types...>&)> sorted) {
		return Where([sorted](tuple<Types...>& A) { return !sorted(A); });
	}

	template<size_t Column>
	DataStore& Where_i(function<bool(decay_t<decltype(get<Column>(Unit[0]))>)> sorted) {
		return Where([sorted](tuple<Types...>& A) { return !sorted(get<Column>(A)); });
	}

	template<typename... AnotherTypes>
	DataStore<Types..., AnotherTypes...> Join(DataStore<AnotherTypes...> variable_note) {
		vector<tuple<AnotherTypes...>> newVector;
		for (const auto& s : variable_note)
			newVector.push_back(s);

		size_t maxIndex = max(Unit.size(), newVector.size());
		Unit.resize(maxIndex); newVector.resize(maxIndex);

		vector<tuple<Types..., AnotherTypes...>> newVector2;

		transform(Unit.begin(), Unit.end(), newVector.begin(), back_inserter(newVector2),
			[](const auto& A, const auto& B) {
				return tuple_cat(A, B);
			});

		DataStore<Types..., AnotherTypes...> newDataStore;
		for (const auto& s : newVector2)
			newDataStore.AddNote(s);

		return newDataStore;
	}

	constexpr bool Contains(const tuple<Types...> found) {
		return find(Unit.begin(), Unit.end(), found) != Unit.end();
	}

	constexpr bool Contains(const initializer_list<tuple<Types...>>& found_list) {
		vector<tuple<Types...>> foundList = found_list;
		for (int i = 0; i < Unit.size(); i++)
			for (int j = 0; j < foundList.size(); j++)
				if (Unit[i] == foundList[j])
					return true;

		return false;
	}

	constexpr bool Contains(DataStore& variable_note, const size_t index) {
		return find(Unit.begin(), Unit.end(), variable_note[index]) != Unit.end();
	}

	constexpr bool Contains(DataStore& variable_note) {
		for (int i = 0; i < Unit.size(); i++)
			for (int j = 0; j < variable_note.Unit.size(); j++)
				if (Unit[i] == variable_note.Unit[j])
					return true;

		return false;
	}

	constexpr int ContainsCount(const tuple<Types...> found) {
		return static_cast<int>(count(Unit.begin(), Unit.end(), found));
	}

	constexpr int ContainsCount(const initializer_list<tuple<Types...>>& found_list) {
		vector<tuple<Types...>> foundList = found_list; int count = 0;
		for (int i = 0; i < Unit.size(); i++)
			for (int j = 0; j < foundList.size(); j++)
				if (Unit[i] == foundList[j])
					count++;

		return count;
	}

	constexpr int ContainsCount(DataStore& variable_note, const size_t index) {
		return static_cast<int>(count(Unit.begin(), Unit.end(), variable_note[index]));
	}

	constexpr int ContainsCount(DataStore& variable_note) {
		int count = 0;
		for (int i = 0; i < Unit.size(); i++)
			for (int j = 0; j < variable_note.Unit.size(); j++)
				if (Unit[i] == variable_note.Unit[j])
					count++;

		return count;
	}

	template<size_t Column>
	constexpr vector <decay_t<decltype(get<Column>(Unit[0]))>> ColumnToVector() {
		vector<decay_t<decltype(get<Column>(Unit[0]))>> arr;
		for (const auto& S : Unit)
			arr.push_back(get<Column>(S));

		return arr;
	}

	template<size_t Column>
	constexpr std::list<decay_t<decltype(get<Column>(Unit[0]))>> ColumnToList() {
		vector<decay_t<decltype(get<Column>(Unit[0]))>> tempArr;
		for (const auto& S : Unit)
			tempArr.push_back(get<Column>(S));

		return list<decay_t<decltype(get<Column>(Unit[0]))>>(tempArr.begin(), tempArr.end());
	}

	template<size_t Column>
	constexpr deque<decay_t<decltype(get<Column>(Unit[0]))>> ColumnToDeque() {
		vector<decay_t<decltype(get<Column>(Unit[0]))>> tempArr;
		for (const auto& S : Unit)
			tempArr.push_back(get<Column>(S));

		return deque<decay_t<decltype(get<Column>(Unit[0]))>>(tempArr.begin(), tempArr.end());
	}

	template<size_t Column>
	constexpr  ColumnContainer<decay_t<decltype(get<Column>(Unit[0]))>> ColumnToBox() {
		ColumnContainer<decay_t<decltype(get<Column>(Unit[0]))>> arr;
		for (const auto& S : Unit)
			arr.Add(get<Column>(S));

		return arr;
	}

	template<size_t Column>
	constexpr set<decay_t<decltype(get<Column>(Unit[0]))>> ColumnToSet() {
		set<decay_t<decltype(get<Column>(Unit[0]))>> arr;
		for (const auto& S : Unit)
			arr.insert(get<Column>(S));

		return arr;
	}
};