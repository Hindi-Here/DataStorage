#pragma once
#include <iostream>
#include <tuple>
#include <sstream>

#include <list>
#include <set>
#include <deque>

#include "ColumnContainer.h"

template <class... Types>
class DataStore {
protected:
	vector<tuple<Types...>> Unit;

	string _GetLine_(size_t line) {
		stringstream SS;
		apply([&SS](const auto&... args) {
			((SS << args << ' '), ...);
			}, Unit[line]);

		return SS.str();
	}


	auto CompareNote() { return [](const auto& a, const auto& b) { return a < b; }; }

	template<size_t Column> auto Compare() {
		return [](const auto& a, const auto& b) { return get<Column>(a) < get<Column>(b); };
	}

	template<size_t Column> auto Equals_Compare() {
		return [](const auto& a, const auto& b) { return get<Column>(a) == get<Column>(b); };
	}

public:

	DataStore() {}
	DataStore(const size_t size) : Unit(size) {}
	DataStore(const Types... type) { Unit.push_back({ type... }); }
	DataStore(const initializer_list<tuple<Types...>> type) : Unit(type) {}

	~DataStore() {}

	typename vector<tuple<Types...>>::iterator begin() { return Unit.begin(); }
	typename vector<tuple<Types...>>::iterator end() { return Unit.end(); }

	tuple<Types...>& operator[](const size_t index) { return Unit[index]; }

	size_t LineCount() { return Unit.size(); }
	size_t ColumnCount() { return sizeof...(Types); }
	DataStore& Resize(size_t size) { Unit.resize(size); return *this; }
	DataStore& Reverse() { reverse(Unit.begin(), Unit.end()); return *this; }
	void Clear() { Unit.clear(); }



	template<size_t Line, size_t Column>
	constexpr auto GetElement() { return get<Column>(Unit[Line]); }

	constexpr string First() { return _GetLine_(0); }
	constexpr string Last() { return _GetLine_(Unit.size() - 1); }

	template<size_t Line>
	constexpr string GetLine() {
		return _GetLine_(Line);
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

	DataStore& AddNote(const initializer_list<tuple<Types...>> note_list) {
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
		get<Column>(Unit[Line]) = notePart;
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



	template<size_t Line>
	DataStore& DeleteNote() {
		Unit.erase(Unit.begin() + Line);

		return *this;
	}

	DataStore& DeleteNote(const size_t index) {
		Unit.erase(Unit.begin() + index);

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

	constexpr bool Contains(const tuple<Types...> found) {
		return find(Unit.begin(), Unit.end(), found) != Unit.end();
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


	template<size_t Column>
	constexpr vector <decay_t<decltype(get<Column>(Unit[0]))>> ColumnToVector() {
		vector<decay_t<decltype(get<Column>(Unit[0]))>> arr;
		for (const auto& S : Unit)
			arr.push_back(get<Column>(S));

		return arr;
	}

	template<size_t Column>
	constexpr list<decay_t<decltype(get<Column>(Unit[0]))>> ColumnToList() {
		list<decay_t<decltype(get<Column>(Unit[0]))>> arr;
		for (const auto& S : Unit)
			arr.push_back(get<Column>(S));

		return arr;
	}

	template<size_t Column>
	constexpr deque<decay_t<decltype(get<Column>(Unit[0]))>> ColumnToDeque() {
		deque<decay_t<decltype(get<Column>(Unit[0]))>> arr;
		for (const auto& S : Unit)
			arr.push_back(get<Column>(S));

		return arr;
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