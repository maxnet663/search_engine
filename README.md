<div id="header" align="center">
  <img src="https://imageup.ru/img114/4487079/readme2.png" height="71" width="450">
</div>

---

  ### Мой проект по разработке локального поискового движка.

* Цель проекта: создание локального поискового движка на языке программирования C++.
* Основные функции поискового движка: анализ текстовых документов, индексация слов, поиск по запросам пользователя, вывод результатов.


Ищет быстро и на любых языках. Проверьте!

---

- [Что я использовал?](#tools)
- [Как установить?](#install)
- [Как пользоваться?](#usage)
- [Тесты](#tests)
- [API](#api)

---
### Tools

* Google Tests <a href="https://github.com/google/googletest"> <img src="https://raw.githubusercontent.com/devicons/devicon/1119b9f84c0290e0f0b38982099a2bd027a48bf1/icons/google/google-original.svg" title="G" alt="gtests" width="40" height="20"/>&nbsp; </a>
* Библиотека JSON for Modern C++ (https://github.com/nlohmann/json)
* Библиотека termcolor (https://github.com/ikalnytskyi/termcolor)
* Библиотека ICU (https://icu.unicode.org/)
* C++ STD=17 <img src="https://raw.githubusercontent.com/devicons/devicon/1119b9f84c0290e0f0b38982099a2bd027a48bf1/icons/cplusplus/cplusplus-original.svg" title="cpp" alt="cpp" width="50" height="20"/>&nbsp;
* g++ 11.4.0 <img src="https://raw.githubusercontent.com/devicons/devicon/1119b9f84c0290e0f0b38982099a2bd027a48bf1/icons/gcc/gcc-original.svg" title="gcc" alt="cpp" width="50" height="25"/>&nbsp;
* cmake 3.21 <img src="https://raw.githubusercontent.com/devicons/devicon/1119b9f84c0290e0f0b38982099a2bd027a48bf1/icons/cmake/cmake-original.svg" title="cmake" alt="cmake" width="60" height="25" />&nbsp;
---

### Install
Прежде всего, вам необходима установленная и собранная библиотека icu (не ниже версии 60), для работы с unicode коировкой. На некторых дистрибутивах Linux она может уже присутствовать. На Windows её можно установить, например через установщик Cygwin. 


Есть два способа собрать и установить проект. Использовать скрипты из директории install_files, либо самостоятельно, с использованием удобных вам инструментов.
В обоих случаях предпологается, что версия вашего cmake 3.21 и выше. Теперь по порядку:

#### Scripts

В каталоге install_files находятся два скрипта для сборки и установки проета - для windows и unix-like систем,
на python и bash соответственно. Скрипты работают прямо из текущей директории, перемещать их никуда не нужно.

На windows, проще всего будет установить проект если присутствует cygwin.
Обратите внимание, что на windows вам понадобится интерпретатор python, а также убедитесь, что в системной переменной
PATH указаны пути к интерпретатору и папке bin используемого вами набора компиляторов

Ближе к делу:
1. Откройте эмулятор терминала
2. Переходим в каталог install_files в корне проекта
3. На unix-like системах делаем скрипт install_nix исполняемым `chmod +x install_nix`
4. Оба скрипта принимают единственный параметр, а именно: директорию установки. На windows
   пишем `python.exe install_win.py /path/to/install`. Для ~~нормальных~~ nix систем даем команду
   `./install_nix /path/to/install`  
   В install_win, также можно передать доп. параметры (компилятор, генератор конф. файлов), подробнее можно посмотреть в самом скрипте)
5. Вот и всё, вы молодец.

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
│── search_engine <--------- выполнение простого поиска и выдача результата в answers.json
├── resources <------------- каталог с текстами по умолчанию, документы для поиска
│   ├── file001.txt          в принципе не обязаны находиться здесь, главное верно передать
│   ├── file002.txt          пути к ним в config.json
│   ├── file003.txt
│   ├── file004.txt
|   └── ...
└── jsons <----------------- каталог с конфиругационными файлами по умолчанию
    ├── config.json
    └── requests.json
````
Взаимодействие с поисковым движком осуществляется посредством json файлов.

`config.json` содержит техническую информацию о версии движка и его названии, а также список индексируемых документов (пути 
к ним могут быть, как относительными, так и абсолютными), укажите здесь желаемые для поиска документы

`requests.json` содержит список запросов, указываете их здесь в формате "your request", с соблюдением json нотации

Установленные файлы для демонстрации работы предлагают файлы json и тексты для поиска по умолчанию,
можете использовать их или создать свои

В директории установки есть два файла, search_engine представляет функционал для простого поиска,
search_engine_session_similator - 

В эмуляторе сеанса работы, можно более налядно посмотреть, что происходит, а также выполнить несколько поисков
в одном сеансе (редактируя файлы json прямо в время исполнения). Подробнее можно узнать здесь: [comands](#commands-list)

---

### Tests

После сборки проекта, можно убедиться в работоспособности кода запустив исполняемый файл с тестами
`test_main.cpp` из директории сборки. Исполняемый файл пренеосить никуда не нужно.
 Для тестирования используются файлы из папки `test` в корне проекта, программа сама их обнаружит.

---
### API

Подробную документацию сгенерированую с помощью doxygen можно посмотреть в папке docs.

Далее будут представлены лишь освновные сведения

### <font size="4" color="orange"> "converter_json.h" </font>
Здесь объявлен класс ConverterJSON, осуществляющий управление json файлами

#### Constructors:


    ConverterJSON() : config(nullptr), requests(nullptr) {};

    ConverterJSON(const PathType &jsons_dir);

    ConverterJSON(PathType conf_p, PathType req_p);

Есть возможность создать "пустой" объект, и привязать файлы потом. Либо можно указать директории
для поиска json файлов, объект сам попытается их обнаружить, также можно указать путь к
конфиругационным файлом явно.

---
### <font size="4" color="orange"> "inverted_index.h" </font>

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
#### Assignment:
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

### <font size="4" color="orange"> "file_reader.h" </font>
Класс представляет из себя обертку на стандартным потоком чтения из файла, определяющий дополнительный функционал для работы с файлами. В частности, проверку на наличие прав на чтение из файла или запись в файл, а также позволяет форматировать извлеченный из файла текст.

---

### <font size="4" color="orange"> "formatting.h" </font>
Заголовок объявляет, два пространства имён: utf и unicode. Для удобства, они объединены в один namespace format. Здесь объявлены функции для форматировани строк.

---
### <font size="4" color="orange">"screen_writer.h</font>
`class ScreenWriter` Предоставляет интерфейс для иммитации сеанса работы с сервером

#### Constructors
    ScreenWriter() : ScreenWriter(std::queue<std::string>()) {}

    explicit ScreenWriter(ArgsList args);
Конструктор принимает на вход очередь аргументов, которые будут переданы в конструктор ConverterJSON.
У класс перегружен оператор вызова функции, который и начинает сеанс работы.

У объекта перегружен оператор вызова функции `operator()`, который и создает сессию работы с пользователем.
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



