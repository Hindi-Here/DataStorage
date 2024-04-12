Небольшая библиотека для работы с табличными данными. DataStorage предоставляет инструменты для:
- работы с таблицами;
- изменения элементов и структуры таблиц;
- сортировки и создания запросов.
## Несколько примеров работы с DataStorage
#### Увеличение зарплаты
```C++
	DataStore<int, string, string, int, double> Teachers = { 
		{__COUNTER__, "Ekaterina", "mathematics", 43, 20500.33},
		{__COUNTER__, "Eliza", "literature", 25, 14300},
		{__COUNTER__, "Sergey", "programming", 17, 10200},
		{__COUNTER__, "Olga", "english", 11, 7800.50},
		{__COUNTER__, "Timm", "mathematics", 33, 17400.87},
	};

	Teachers.Where<3>([](int lesson) {return lesson > 20; })
                .ChangeColumn<4>([](double& salary) {return salary += 5000; })
                .OrderByDescending<4>();
 
	for (const auto& [ID, name, subject, lesson, salary] : Teachers)
		cout << name << ' ' << subject << ' ' << salary << '\n';
```
#### Запросы для получения сведений
```C++
	DataStore<int, FunctionalLIB_String::String, double, char> UserStats = {
		{24772, "user7098", 1200.3, 'A'},
                            /* ... */
	};

	auto TopRankerList = UserStats.OrderBy<3>().Reverse().Take(5);
			// получение пятерки лучших игроков

        UserStats.Restore(); // восстановление исходной таблицы

	auto MediumRankerTable = UserStats.Where<3>([](char rank) {return rank > 'A' && rank <= 'D'; });
			// разбиение общей таблицы на подтаблицы

	int LowPlayerTime = UserStats.Restore().ColumnToBox<2>().Min();
			// получение данных о пользователе с наименьшим количеством часов
```
#### Изменение структуры таблицы
```C++
	DataStore<int, int> SumTable = { {4,1}, {5,-2}, {10, 31} };

	SumTable.AddNote({
		{0,0}, {-11, 7}, {13, 2}
		});

	DataStore<int, int, int> TransformedTable =
		SumTable.AddColumn(NULL)
                        .ChangeColumn([&](tuple<int, int, int>& SUM) 
                                 {return get<2>(SUM) = get<0>(SUM) + get<1>(SUM); });

	cout << TransformedTable.GetColumn<2>();

	SumTable.~DataStore();
```
## Перечень основных функций
#### Получение данных
- `LineCount()`, `ColumnCount()`
- `First()`, `Last()`
- `Contains(list param)`, `Contains(note from variable)`
- `ColumnToBox()`, `ColumnToDeque()`, `ColumnToList()`, `ColumnToSet()`, `ColumnToVector()`
- `GetLine<line>()`, `GetColumn<column>()`, `GetElement<line, column>()`
- `GetType<column>()`
#### Трансформация таблицы
- `AddNote(list param)`, `AddColumn<column>(fill value)`
- `ChangeNote<line>(list param)`, `ChangeNote<line>(note from variable)`, `ChangeElement<line, column>()`,
- `ChangeColumn<column>(initializer_list or ColumnContainer)`, `ChangeColumn(lambda)`
- `DeleteNote(index)`, `DeleteNote(list param)`, `DeleteNote(note from variable)`
- `Restore()`
- `Resize()`, `Clear()`
- `Union()`
#### Сортировка
- `OrderBy()`, `OrderBy<column>()`
- `OrderByDescending()`, `OrderByDexcending<column>()`
- `Unique()`, `Unique<column>()`
- `Repeat()`, `Repeat<column>()`
- `Where(lambda)`, `Where_i(lambda)`
- `Reverse()`
- `Take(count or range)`
#### Агрегатные функции ColumnContainer
- `Sum()`
- `Average()`
- `Min()`, `Max()`
- `Count()`
- `Contains(list param)`
- `Median()`
- `Clear()`
