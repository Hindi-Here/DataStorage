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
		cout << name + '\n';
```
