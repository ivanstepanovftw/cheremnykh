# https://perm.hh.ru/vacancy/31985776 from https://perm.hh.ru/employer/3339945
A young, developing company is located in Switzerland. Engaged in marketing services for IT projects. The marketing company (the head office in Switzerland) is expanding the staff of online employees.


# Test units
На выполнение заданий сутки.

## Unit 1
Напишите программу, которая отсортирует числа формата double хранящихся в текстовом файле размером 1Гб (одно число в одной строке). 

Пример:

```
1.26192e+308
8.33891e+307
8.19572e+307
1.64584e+304
...
0
```

Программа должна использовать не более 100Мб оперативной памяти, и работать не дольше 25-30 минут (на 2Гц современном одноядерном процессоре).

Обязательные параметры: <имя файла неотсортированного> <имя файла отсортированного>
Также должен быть написан генератор неотсортированного 1Гб файла с числами формата double

## Unit 2
Написать собственную реализацию shared_mutex, поддерживающий рекурсию. Не используя C14. 
Использовать платформонезависимый код – средства stl и boost (но не использовать shared_mutex).

## Unit 3
Написать алгоритм поиска текста по маске с wildcards (можно просто функцию куда передается два параметра – строка поиска и строка с маской). Wildcards содержит символы `*` и `?`.

## Unit 4
Представим что есть интерфейс к БД: 
```cpp
struct i_db
{
    bool begin_transaction();
    bool commit_transaction();
    bool abort_transaction();
    std::string get(const std::string& key);
    std::string set(const std::string& key, const std::string& data);
    std::string del(const std::string& key);
}
```
Написать реализацию кеша для БД, обратить внимание на многопоточность и на транзакционную модель работы с БД. 

# Build
```bash
mkdir cmake-build-gcc && cd cmake-build-gcc \
 && /sbin/cmake -DCMAKE_CXX_COMPILER=/sbin/g++ -G "CodeBlocks - Unix Makefiles" .. \
 && /sbin/cmake --build . --target all
```

# Author
 - Ivan Stepanov <ivanstepanovftw@gmail.com> 

