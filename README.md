<div id="header" align="center">
  <img src="https://imageup.ru/img114/4487079/readme2.png" height="71" width="450">
</div>

---

  ### Мой учебный проект по разработке локального поискового движка.

* Цель проекта: создание локального поискового движка на языке программирования C++.
* Основные функции поискового движка: анализ текстовых документов, индексация слов, поиск по запросам пользователя, вывод результатов.
* Процесс работы поискового движка: при запуске происходит сканирование директории с текстовыми документами, далее происходит анализ каждого документа, индексация слов и сохранение данных в базу данных. При запросе пользователя поисковый движок ищет совпадения в базе данных и выводит результаты поиска.
* Ожидаемые результаты: создание функционального локального поискового движка, способного эффективно работать с большим количеством текстовых документов и обеспечивать точный и быстрый поиск по запросам пользователей.

---

- [Что я использовал?](#tools)
- [Как установить?](#install)
- [Как пользоваться?](#usage)
- [API](#api)
- []

---
### Tools

* Google Tests <a href="https://github.com/google/googletest"> <img src="https://raw.githubusercontent.com/devicons/devicon/1119b9f84c0290e0f0b38982099a2bd027a48bf1/icons/google/google-original.svg" title="G" alt="gtests" width="40" height="20"/>&nbsp; </a>
* Библиотека JSON for Modern C++ (https://github.com/nlohmann/json)
* Библиотека termcolor (https://github.com/ikalnytskyi/termcolor)
* C++ STD=17 <img src="https://raw.githubusercontent.com/devicons/devicon/1119b9f84c0290e0f0b38982099a2bd027a48bf1/icons/cplusplus/cplusplus-original.svg" title="cpp" alt="cpp" width="50" height="20"/>&nbsp;
* g++ 11.4.0 <img src="https://raw.githubusercontent.com/devicons/devicon/1119b9f84c0290e0f0b38982099a2bd027a48bf1/icons/gcc/gcc-original.svg" title="gcc" alt="cpp" width="50" height="25"/>&nbsp;
* cmake 3.21 <img src="https://raw.githubusercontent.com/devicons/devicon/1119b9f84c0290e0f0b38982099a2bd027a48bf1/icons/cmake/cmake-original.svg" title="cmake" alt="cmake" width="60" height="25" />&nbsp;
---

### Install

Есть два способа собрать и установить проект. Использовать скрипты из директории install_files,
либо вы можете сделать это самостоятельно, с использованием удобных вам инструментов.
В обоих случаях предпологается, что версия вашего cmake 3.21 и выше. Теперь по порядку:

#### Scripts

В каталоге install_files находятся два скрипта для сборки и установки проета - для windows и unix-like систем,
на python и bash соответственно. Скрипты работают прямо из текущей директории, перемешать их никуда не нужно.
Оба скрипта предполагают у вас наличие, опять же cmake 3.21 и выше, а также
генератора makefile'ов. В любом дистрибутиве основаном на ядре linux, они с верояность близкой к единице уже имеются,
на macos если у вас установлен cmake то поддержка Unix Makefiles, тоже имеется. На windows, я бы рекомендовал использовать
иструментарий MinGW-w32, либо любой из пакетов MinGW-w64 (например, w64devkit https://www.mingw-w64.org/downloads/#w64devkit)
Обратите внимание, что на windows вам понадобится интерпретатор python, а также убедитесь, что в системной переменной
PATH указаны пути к интерпретатору и папке bin используемого вами набора компиляторов

Ближе к делу:
1. Откройте эмулятор терминала
2. Переходим в каталог install_files в корне проекта
3. На unix-like системах делаем скрипт install_nix исполняемым `chmod +x install_nix`
3. Оба скрипта принимают единственный параментр, а именно: директорию установки. На windows
   пишем `python.exe install_win.py /path/to/install`. Для ~~нормальных~~ nix систем даем команду
   `./install_nix /path/to/install` В windows если вы хотите использовать альтернативный генератор файлов,
   измените 33 строку скрипта указав предпочтение после флага -G
4. Вот и всё, вы молодец.

#### Самостоятельно:
1. Скачайте архив с проектом и распакуйте в удобную директорию
2. Теперь необходимо собрать проект, если вы используете какую либо современную IDE, то скорее всего она
  всё сделает за вас, вам нужно лишь явно указать папку для установки проекта, это 40-я строка в корневом
  cmakelist'e, расскоментируйте и укажите предпочтительный каталог. Если собираете вручную:
    * Cоздайте в корне проекта директорию для сборки (например, build).
        Сделать это можно командой `mkdir build` в эмуляторе терминала
    * Перейдите в созданную директорию с помощью команды 
      `cd build`, затем пишем  
         `cmake -DINSTALL_GMOCK:BOOL=OFF -DCMAKE_INSTALL_PREFIX:PATH=/path/to/install ../`
         Здесь мы конфигурируем проект, первая директива, просит записать в кэш, что устанваливать 
         тесты не нужно, вторая явно задает папку для установки. Также, можете первым параметром указать
         генератор файлов, например `cmake -G "Unix Makefiles"`. Если что-то пойдет не так, не переживайте,
         всё это потом можно будет отредактировать вручную в каталоге сборки, в файлах cmakecache и cmakeinstall
    * Мы почти у цели, даём команду `cmake --build .` Произойдет сборка проекта.
3. Осталось установка. В зависимости от вашего генератора конфигурационных файлов произведите установку.
  Например, если вы используйте ninja, GNU Makefiles и т.д. в директории сборки дайте команду `make install/local`  

---

### Usage
После установки директория программы будет выглядеть так
````
├── install/path
│── search_engine_session_similator <-- файл запускате эмуляцию сеанса работы с поисковым движком
│── search_engine <---- выполнение простого поиска и выдача результата в answers.json
├── resources <------------- каталог с текстами по умолчанию, документы для поиска
│   ├── file001.txt          в принципе не обязаны находиться здесь, главное верное передать
│   ├── file002.txt          пути к ним в config.json
│   ├── file003.txt
│   └── file004.txt
└── jsons <----------------- каталог с конфиругационными файлами по умолчанию
    ├── config.json
    └── requests.json
````
При запуске, эмулятор предложит вам ввести кталог для поиска конфиругационных json файлов (так что вы
можете расположить их где угодно, главное верно передать путь),
либо дать ему отыскать их самому в текущем дереве каталога, simple же версия будет пытаться обнаружить
эти файлы в текущем дереве, поэтому они должны находиться в его досягаемости. Результат поиска записывается в
файл в том же каталоге, что и запущенная программа.

`config.json` содержит техническую информацию о версии движка и его названии, а также список индексируемых документов (пути 
к ним могут быть, как относительными, так и абсолютными), укажите здесь желаемые для поиска документы

`requests.json` содержит список запросов, указываете их здесь в формате "your request", с соблюдением json нотации

Установленные файлы, для демонстрации работы, предлагают файлы json и тексты для поиска по умолчанию,
можете использовать их или использовать свои

В эмуляторе сеанса работы, можно более налядно посмотреть, что происходит, а также выполнить несколько поисков
в одном сеансе (редактируя файлы json прямо в время исполнения). Подробнее можно узнать здесь: [comands](#commands-list)

---

### Tests

После сборки проекта, можно убедиться в работоспособности кода запустив исполняемый файл с тестами
`test_main.cpp` из директории сборки.
Тесты к установке не предлагаются, т.к. установка предназначена для конечного пользователя, тесты - 
для разработчиков. Для тестирования используются файлы из папки `test` в корне проекта, программа сама их обнаружит.

---
### API

### <font size="4" color=orange> "custom_functions.h" </font>
Определяет пространство имён custom, содержащее вспомогательные функции

| Return                     | Name                                                                         | Description                                                                                                                                                         |
|----------------------------|------------------------------------------------------------------------------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `std::string`              | `getFileText(const std::string &file_name)`                                  | Передаёте путь к текстовому файлу - получаете его текст. Если файл не существует или недоступен для - чтения бросает исключение                                     |
| `void`                     | `formatString(std::string &s)`                                               | Из строки будут удалены: все знаки пунктуации, лишние пробелы. Буквы будут приведены к нижнему регистру                                                             |
| `size_t`                   | `wordsCounter(const std::string &s)`                                         | Передаёте строку, получаете количество слов в ней. Под словом подразумевается непрерывная последовательность букв и цифр, окружённая пробелами или границами строки |
| `void`                     | `toLowerCase(std::string &s)`                                                | Приводит все буквы в строке к нижнему регистру                                                                                                                      |
| `void`                     | `deletePunctuationMarks(std::string &s)`                                     | Удаляет из строки знаки пунктуации                                                                                                                                  |
| `void`                     | `deleteExtraSpaces(std::string &s)`                                          | Удаляет из строки лишние пробелы                                                                                                                                    |
| `void`                     | `writeJsonToFile(const nlohmann::json &json_text , const std::string &path)` | Записывает JSON объект в файл по переданному пути. Если файл не существует, он будет создан, иначе, файл будет перезаписан                                          |
| `std::string`              | `std::string getFileName(std::string s)`                                     | Возвращает имя файла по заданному пути. Удобно использовать, когда вам нужно имя файла, а не его расположение                                                       |
| `size_t`                   | `countOccurrences(const std::string &text, const std::string &word)`         | Возвращает количество вхождений слова word в тексте text                                                                                                            |
| `std::vector<std::string>` | `getUniqueWords(const std::string &text)`                                    | Возвращает массив содержащий все уникальные слова из текста                                                                                                         |
| `double`                   | `round(double num, int precision)`                                           | Округляет число num типа `double` до `precision` знаков после запятой, возвращает новое число                                                                       |          
| `bool`                     | `isReadable(const std::string &file_name)`                                   | Возвращает `true` если текущий пользователь имеет права на чтение данного файла, `false` иначе                                                                      |
| `bool`                     | `isWriteable(const std::string &file_name)`                                  | Возвращает `true` если текущий пользователь имеет права на запись в данный файл, `false` иначе                                                                      |
| `void`                     | `print_red(const std::string &msg)`                                          | Выводит сообщение `msg` в стандартный поток вывода в красном цвете                                                                                                  |
| `void`                     | `print_green(const std::string &msg)`                                        | Выводит сообщение `msg` в стандартный поток вывода в зеленом цвете                                                                                                  |
| `void`                     | `print_yellow(const std::string &msg)`                                       | Выводит сообщение `msg` в стандартный поток вывода в жёлтом цвете                                                                                                   |
| `void`                     | `print_blue(const std::string &msg)`                                         | Выводит сообщение `msg` в стандартный поток вывода в синем цвете                                                                                                    |

---
### <font size="4" color="orange"> "converter_json.h" </font>
Здесь объявлен класс ConverterJSON, осуществляющий управление json файлами

#### Constructors
При создании объекта класса, эекземпляр попытается найти файлы config.json и requests.json в текущем поддереве каталога.
При неудачном поиске - выбрасывает исключение. Вы можете явно задать директорию, в поддереве которой следует
осуществлять поиск

      ConverterJSON(const std::string &jsons_dir);

Если директория не задана, по умолчанию, поиск будет производиться в текущем каталоге и его поддереве

       ConverterJSON() : ConverterJSON(
            std::filesystem::current_path().string()) {}

#### Public members

| Return                     | Name                                                          | Description                                                                                                                                                                                                                                                                                                                       |
|----------------------------|---------------------------------------------------------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `const nlohmann::json&`    | `getConfig() const`                                           | Возвращает текущий `config.json` в виде json объекта                                                                                                                                                                                                                                                                              |
| `std::string`              | `getConfigPath() const`                                       | Возвращает путь к текщему файлу `config.json`                                                                                                                                                                                                                                                                                     | 
| `std::string`              | `getRequestsPath() const`                                     | Возвращает путь к текущему файлу `requests.json`                                                                                                                                                                                                                                                                                  |
| `std::<std::string>`       | `getTextDocuments()`                                          | Возвращает массив путей к файлам указанным в `config.json`                                                                                                                                                                                                                                                                        |
| `int`                      | `getResponsesLimit()`                                         | Возвращает максимальное количество результатов на каждый запрос                                                                                                                                                                                                                                                                   |
| `std::vector<std::string>` | `getRequests()`                                               | Возвращает массив, содержащий запросы из `requests.json`                                                                                                                                                                                                                                                                          |
| `void`                     | `putAnswers(std::vector<std::vector<RelativeIndex>> answers)` | Помещает answers в json формате в файл `answers.json`. Если файл с таким именем существует - он будет перезаписан, если нет - будет создан. Если файл не доступен для записи, метод попытается создать файл `answers_safe.json` чтобы избежать потери результатов, если эта попытка также провалится - будет выброшено исключение |
| `void`                     | `updateConfig()`                                              | Если в файл `config.json` были внесены изменения во время работы программы, метод попытается его обновить                                                                                                                                                                                                                         |
| `void`                     | `updateRequests()`                                            | Если в файл `requests.json` были внесены изменения во время работы программы, метод попытается его обновить                                                                                                                                                                                                                       |

#### Static members

| Return           | Name                                                             | Description                                                                                                                                            |
|------------------|------------------------------------------------------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------|
| `nlohmann::json` | `openJson(const std::string &path)`                              | Преобразует содержимое файла по указаному пути в json объект, в случае неудачи выбрасывает исключение                                                  |
| `std::string`    | `findFile(const std::string &file_name)`                         | Ищет файл по указаному имени в текущем дереве каталога, возвращает абсолютный путь к найденному файлу, если файл не найден, возвращает пустую строку   |
| `std::string`    | `findFile(const std::string &dir, const std::string &file_name)` | Ищет файл по указаному имени в дереве каталога `dir`, возвращает абсолютный путь к найденному файлу, если файл не найден, возвращает пустую строку     |


---
### <font size="4" color="orange"> "inverted_index.h" </font>
Структура `Entry` необходима для индексации слов в документах
                
    struct Entry {
        size_t doc_id;
        size_t count;

        bool operator==(const Entry &other) const {
        return (doc_id == other.doc_id && count == other.count);
        }
    };

`class InvertedIndex` индексирует слова в текстах документов

    Сlass InvertedIndex {

       std::vector<std::string> docs_texts;
       std::unordered_map<std::string, std::vector<Entry>> freq_dictionary;
       std::mutex dict_access;

    public:

       static const std::vector<Entry> nfound;
    };


| field             | Description                                                                                                                        |
|-------------------|------------------------------------------------------------------------------------------------------------------------------------|
| `docs`            | Хранит документы по которым происходит индексация слов                                                                             |
| `freq_dictionary` | Класс формирует словарь содержащий частоту вхождений слова в каждом документе                                                      |
| `dict_access`     | Так как, индексация слов происходит в нескольких потоках, мьютекс необходим для контроля доступа к словарю                         |
| `nfound`          | Пустой массив записей Entry, возвращается в качетве результата запроса к словарю, если указанное в запросе слово не индексированно |

#### Constructors:
    
    InvertedIndex() = default;

    InvertedIndex(const InvertedIndex &other)
        : docs(other.docs), freq_dictionary(other.freq_dictionary) {};

    InvertedIndex(const InvertedIndex &other)
        : docs_texts(other.docs_texts)
        , freq_dictionary(other.freq_dictionary) {};

    InvertedIndex(InvertedIndex&& other) noexcept
        : docs_texts(std::move(other.docs_texts))
        , freq_dictionary(std::move(other.freq_dictionary)) {};
#### Assignment
    InvertedIndex& operator=(const InvertedIndex &right);

    InvertedIndex& operator=(InvertedIndex&& right) noexcept;
   
#### Public methods

| Return                      | Name                                                             | Description                                                                                                       |
|-----------------------------|------------------------------------------------------------------|-------------------------------------------------------------------------------------------------------------------|
| `void`                      | `updateDocumentBase(const std::vector<std::string> &input_docs)` | Запускает идексацию слов в массиве документов input_docs, реузльтатом работы является частотный словарь freq_dict |
| `const std::vector<Entry>&` | `getWordCount(const std::string &word) const`                    | Возвращает частоту вхождения слова word в массиве документов docs                                                 |

---
### <font size="4" color="orange">"search_server.h</font>
Класс SearchServer осуществляет поиск по индексированным документам, принимает запросы, обрабатывает их и формирует итоговую структуру с результатами запросов. 

    class SearchServer {

        const InvertedIndex &_index;
    };

Структура DocRelevance предоставляет удобный формат для хранения и обработки документов и их релевантности
    
    struct DocRelevance {
        size_t doc_id;
        size_t relevance;

        DocRelevance() : doc_id(0), relevance(0) {}

        explicit DocRelevance(const std::pair<size_t, size_t> &pair)
        : doc_id(pair.first), relevance(pair.second) {}

        bool operator>(const DocRelevance &right) const {
            return relevance == right.relevance ?
                doc_id < right.doc_id : relevance > right.relevance;
        }
    };

Структура RelativeIndex представляет собой запсь с ответом на запрос, содержащую номер документа и его релевантность для данного запроса

    struct RelativeIndex {
        size_t doc_id;
        float rank;

        bool operator==(const RelativeIndex &other) const {
            return (doc_id == other.doc_id && rank == other.rank);
        }
    };

#### Constructors

    SearchServer() = delete;

    SearchServer(const SearchServer &other) = default;
    
    // recomended
    explicit SearchServer(const InvertedIndex &idx) : _index(idx) {};

Класс содержит один публичный метод, который и производит поиск, сам поиск происходит асинхронно

    std::vector<std::vector<RelativeIndex>> search(
            const std::vector<std::string> &queries_input);

queries_input представляет из себя вектор содержащий запросы. Запросы парсятся на уникальные слова, по ним в частотном словаре происходит поиск релевантных документов, формируется
список таких документов, после чего определяется их релевантность для данного запроса и так для всех запросов из queries_input

---
### <font size="4" color="orange">"screen_writer.h</font>
`class ScreenWriter` Предоставляет интерфейс для иммитации сеанса работы с сервером

#### Constructors
    ScreenWriter() : ScreenWriter(std::filesystem::current_path().string()) {}

    ScreenWriter(const std::string &path);
Конструктор принимает на вход путь к директории, которую передаст ConverterJSON
для поиска json файлов, если такой путь не передан, использует текущий каталог.

Класс создает сеанс работы, взаимодействие осуществляется через окно терминала, ниже приведен список команд:
#### Commands list:
| Command        | Description                                          |
|----------------|------------------------------------------------------|
| `update-db`    | updates data base if you changed indexed docs        |
| `update-rq`    | updates requests if you changed requests.json        |
| `help`         | prints the list of commands                          |
| `status`       | prints info about session                            |
| `find`         | searches for current queries in the current database |
| `print-rq `    | prints current requests from requests.json           |
| `print-db `    | prints current indexed docs from config.json         |
| `print-ans`    | prints cuurent search results from answers.json      |
| `quit` or `q`  | ends the session                                     |



