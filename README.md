<div id="header" align="center">
  <img src="https://imageup.ru/img114/4487079/readme2.png" height="71" width="450">
</div>

---

  ### Мой проект по разработке локального поискового движка.

* Цель проекта: создание локального поискового движка на языке программирования C++.
* Основные функции поискового движка: анализ текстовых документов, индексация слов,
* поиск по запросам пользователя, вывод результатов.


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
Прежде всего, вам необходима установленная и собранная библиотека icu (не ниже версии 60), 
для работы с unicode кодровкой. На некторых дистрибутивах Linux она может уже присутствовать. 
На Windows её можно установить посетив официальный сайт (см. в tools) или, например, через 
установщик Cygwin. 


Есть два способа собрать и установить проект. Использовать скрипты из директории install_files, 
либо самостоятельно, с использованием удобных вам инструментов.
В обоих случаях предпологается, что версия вашего cmake 3.21 и выше. Теперь по порядку:

#### Scripts

В каталоге install_files находятся два скрипта для сборки и установки проета - для windows и unix-like 
систем, на python и bash соответственно. Скрипты работают прямо из текущей директории, 
перемещать их никуда не нужно.

На windows, проще всего будет установить проект если присутствует cygwin.
Обратите внимание, что на windows вам понадобится интерпретатор python, а также убедитесь, что в 
системной переменной PATH указаны пути к интерпретатору и папке bin используемого вами набора 
компиляторов.

Ближе к делу:
1. Откройте эмулятор терминала
2. Переходим в каталог install_files в корне проекта
3. На unix-like системах делаем скрипт install_nix исполняемым `chmod +x install_nix`
4. Оба скрипта принимают единственный параметр, а именно: директорию установки. На windows
   пишем `python.exe install_win.py /path/to/install`. Для ~~нормальных~~ nix систем даем команду
   `./install_nix /path/to/install`  
   В install_win, также можно передать доп. параметры (компилятор, генератор конф. файлов), подробнее можно посмотреть в самом скрипте
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
После установки директория программы будет выглядеть как-то так
````
├── install/path
│── se <--------------- исполняемый файл
├── resources <-------- каталог с текстами по умолчанию, документы для поиска
│   ├── file001.txt     в принципе не обязаны находиться здесь, главное верно передать
│   ├── file002.txt     пути к ним в config.json
│   ├── file003.txt
│   ├── file004.txt
|   └── ...
├── help.json <-------- файл со списком команд
├── config.json <------ конфиругационный файл
└── requests.json <---- файл с запросами

````
Взаимодействие с поисковым движком осуществляется посредством json файлов.

`config.json` содержит техническую информацию о версии движка и его названии, а также список индексируемых документов (пути 
к ним могут быть, как относительными, так и абсолютными), укажите здесь желаемые для поиска документы

`requests.json` содержит список запросов, указываете их здесь в формате "your request", с соблюдением json нотации

`se` принимает на вход команды. С их списком можно ознакомиться здесь  [comands](#commands-list)

Сеанс работы может выглядеть, например, так:
`./se find -print` или `./se generate`

Для начала рекомендуется ознакомиться с возможными командами, кроме help файла это 
можно сделать дав команду `./se help` или `./se h -all`

---

### Tests

После сборки проекта, можно убедиться в работоспособности кода запустив исполняемый файл с тестами
 `test_main` из директории сборки. Исполняемый файл пренеосить никуда не нужно.
 Для тестирования используются файлы из папки `test` в корне проекта, программа сама их обнаружит.

---
### API

Подробную документацию сгенерированую с помощью doxygen можно посмотреть в папке docs.

Далее будут представлены лишь освновные сведения

### <font size="4" color="orange"> "converter_json.h" </font>
Здесь объявлен класс ConverterJSON, осуществляющий управление json файлами

#### Constructors:



    ConverterJSON(const PathType &jsons_dir);

    ConverterJSON(PathType conf_p, PathType req_p);

---
### <font size="4" color="orange"> "inverted_index.h" </font>

`class InvertedIndex` индексирует слова в текстах документов

    typedef std::unordered_map<uint16_t, uint32_t> freq_t;

    typedef std::unordered_map<std::string, freq_t> dict_t;
    
      class InvertedIndex {
    
      dict_t freq_dictionary;
      std::mutex dict_access; // mutex to manage access to freq_dictionary
      std::mutex print_access; // mutex to access to standard output
    
      public:
    
         static const std::vector<Entry> nfound;
      };


| field             | Description                                                                                                                        |
|-------------------|------------------------------------------------------------------------------------------------------------------------------------|
| `freq_dictionary` | Класс формирует словарь содержащий частоту вхождений слова в каждом документе                                                      |
| `dict_access`     | Так как, индексация слов происходит в нескольких потоках, мьютекс необходим для контроля доступа к словарю                         |
| `print_access`    | Мьютекс для предотвращения гонки за cout при выводе сообщений из потоков                                                           |
| `nfound`          | Пустой массив записей Entry, возвращается в качетве результата запроса к словарю, если указанное в запросе слово не индексированно |

   
#### Public methods

| Return                      | Name                                                             | Description                                                                                                       |
|-----------------------------|------------------------------------------------------------------|-------------------------------------------------------------------------------------------------------------------|
| `void`                      | `updateDocumentBase(const std::vector<std::string> &input_docs)` | Запускает идексацию слов в массиве документов input_docs, реузльтатом работы является частотный словарь freq_dict |
| `const std::vector<Entry>&` | `getWordCount(const std::string &word) const`                    | Возвращает частоту вхождения слова word в массиве документов docs                                                 |

---
### <font size="4" color="orange">"search_server.h</font>
Класс SearchServer осуществляет поиск по индексированным документам, принимает запросы,
обрабатывает их и формирует итоговую структуру с результатами запросов. 

    class SearchServer {

        const InvertedIndex &_index;
    };

Структура DocRelevance предоставляет удобный формат для хранения и обработки документов
и их релевантности
    
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

Структура RelativeIndex представляет собой запсь с ответом на запрос, содержащую
номер документа и его релевантность для данного запроса

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

Класс содержит один публичный метод, который и производит поиск, сам поиск происходит 
асинхронно

    std::vector<std::vector<RelativeIndex>> search(
            const std::vector<std::string> &queries_input);

queries_input представляет из себя вектор содержащий запросы. Запросы парсятся на 
уникальные слова, по ним в частотном словаре происходит поиск релевантных документов, 
формируется список таких документов, после чего определяется их релевантность для
данного запроса и так для всех запросов из queries_input

---

### <font size="4" color="orange"> "file_reader.h" </font>
Класс представляет из себя обертку над стандартным потоком чтения из файла, определяющий
дополнительный функционал для работы с файлами. В частности, проверку на наличие прав на
чтение из файла или запись в файл, а также позволяет форматировать извлеченный из файла
текст.

---

### <font size="4" color="orange"> "formatting.h" </font>
Заголовок объявляет, два пространства имён: utf и unicode. Для удобства, они объединены 
в один namespace format. Здесь объявлены функции для форматировани строк.

---
### <font size="4" color="orange">"shell.h</font>
`class Shell` Предоставляет интерфейс для работы с возможностями программы.

У объекта перегружен оператор вызова функции `operator()`, который и создает сессию работы с пользователем.
#### Commands list:
| Command                | Description                                                                  |
|------------------------|------------------------------------------------------------------------------|
| `generate` or `gen`    | generate default json files                                                  |
| `help` or `h`	      | prints this message. Use -all to see more                                    |
| `config` or `conf`     | print info about this program                                                |
| `find` or `f`          | searches for current queries in the current database                         |
| `index` or `idx`       | recursive searches for txt files in directory and adds them to indexed files |
| `print` or `p`         | print out the content of json file in standardoutput                                |
