# Исследование алгоритмов QUICK SORT и INTROSORT

## Описание задачи

В рамках работы выполнено экспериментальное исследование двух алгоритмов сортировки:
1. **QUICK SORT** — стандартная рекурсивная реализация с использованием случайного выбора опорного элемента.
2. **INTROSORT (QUICK+HEAP+INSERTION SORT)** — гибридная реализация, переключающаяся между алгоритмами QUICK SORT, HEAP SORT и INSERTION SORT по следующим правилам:
   - Переключение на HEAP SORT, если глубина рекурсии достигает `2 * log2(N)`.
   - Переключение на INSERTION SORT, если в сортируемой части массива остается менее 16 элементов.

### Цель
Сравнить производительность алгоритмов на массивах различного размера и структуры, определить влияние переключений на производительность гибридного алгоритма, а также провести анализ времени работы с использованием библиотеки `std::chrono`.

---

## Подготовка тестовых данных

### Реализация класса `ArrayGenerator`
Для генерации массивов был написан класс `ArrayGenerator`, позволяющий создавать три типа массивов:
1. **Случайные значения** — элементы массива находятся в диапазоне `[0, 6000]`.
2. **Обратный порядок** — элементы массива отсортированы по убыванию.
3. **Почти отсортированные массивы** — некоторое количество случайных пар элементов переставлено местами.

**Параметры тестовых данных:**
- Размеры массивов: от 5000 до 100000 элементов с шагом 1000 (увеличил исходные данные в 10 раз и замеры проводил в наноскундах в связи с быстрой работой моего компьютера, так как на меньших данных и единицах измерения времени появлялись результаты с времененем выполнения 0).
- Массивы берутся разной длины от 5000 до 100000 с шагом 1000.
- Диапазон случайных значений взят от 0 до 6000.

---

## Эмпирический анализ стандартного алгоритма QUICK SORT

### Методика
Замеры времени работы выполнялись для стандартного алгоритма QUICK SORT с усреднением по 10 запускам для каждого размера массива. Использовалась библиотека `std::chrono`.

## Эмпирический анализ гибридного алгоритма INTROSORT

### Реализация гибридного алгоритма
Гибридная реализация INTROSORT реализована с учетом указанных правил переключений:
- Переход на HEAP SORT при превышении глубины рекурсии `2 * log2(N)`.
- Переход на INSERTION SORT для массивов длиной менее 16.

### Методика
Замеры аналогичны предыдущему этапу: усреднение времени выполнения по 10 запускам для каждого размера массива.

### Результаты
Графики времени выполнения QUICK SORT для всех категорий массивов:

1. **Случайные массивы**  
   ![1](Random%20Array.png)

2. **Обратный порядок**  
   ![2](Reversed%20Array.png)

3. **Почти отсортированные массивы**  
   ![3](Near%20Sorted%20Array.png)

---

## Сравнительный анализ

### Итоги
- **Производительность:** INTROSORT показывает лучшую производительность на массивах, где QUICK SORT может работать медленно (например, массивы в обратном порядке). На случайных данных гибридный алгоритм немного уступает QUICK SORT из-за накладных расходов на переключения.
- **Структура массива:**
  - INTROSORT значительно эффективнее на массивах, отсортированных в обратном порядке, благодаря HEAP SORT.
  - Почти отсортированные массивы обрабатываются быстрее за счет INSERTION SORT.

### Выводы
1. INTROSORT универсален и минимизирует риск возникновения худшего случая QUICK SORT.
2. На реальных данных с неоднородной структурой массивов INTROSORT предпочтительнее благодаря гибкости переключений.

---

## Приложения

### Также реализован был класс SortTester, включающий функции эмпирического замера времени работы рассматриваемых алгоритмов сортировки.

### Ссылка на CodeForces
ID посылки задачи A3i: **[292819921]**

### Ссылка на репозиторий
[GitHub репозиторий с исходными данными и реализациями](https://github.com/Davonchik/Algo_HW_A3)
