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

### Что я использовал?

* Google Tests <a href="https://github.com/google/googletest"> <img src="https://raw.githubusercontent.com/devicons/devicon/1119b9f84c0290e0f0b38982099a2bd027a48bf1/icons/google/google-original.svg" title="G" alt="gtests" width="40" height="20"/>&nbsp; </a>
* Библиотека JSON for Modern C++ (https://github.com/nlohmann/json)
* C++ STD=17 <img src="https://raw.githubusercontent.com/devicons/devicon/1119b9f84c0290e0f0b38982099a2bd027a48bf1/icons/cplusplus/cplusplus-original.svg" title="cpp" alt="cpp" width="50" height="20"/>&nbsp;
* g++ 11.4.0 <img src="https://raw.githubusercontent.com/devicons/devicon/1119b9f84c0290e0f0b38982099a2bd027a48bf1/icons/gcc/gcc-original.svg" title="gcc" alt="cpp" width="50" height="25"/>&nbsp;
* cmake 3.21 <img src="https://raw.githubusercontent.com/devicons/devicon/1119b9f84c0290e0f0b38982099a2bd027a48bf1/icons/cmake/cmake-original.svg" title="cmake" alt="cmake" width="60" height="25" />&nbsp;
---
### API

#### <font color=orange> "custom_functions.h" </font>
Определяет пространство имён custom, содержащее вспомогательные функции

| Return      | Name                                                                | Description                                                                                                                                                         |
|-------------|---------------------------------------------------------------------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `std::string`| `getFileText(const std::string &file_name)`                          | Передаёте путь к текстовому файлу - получаете его текст.                                                                                                            |
| `void`       | `formatString(std::string &s)`                                       | Из строки будут удалены: все знаки пунктуации, лишние пробелы. Буквы будут приведены к нижнему регистру                                                             |
| `size_t`     | `wordsCounter(const std::string &s)`                                 | Передаёте строку, получаете количество слов в ней. Под словом подразумевается непрерывная последовательность букв и цифр, окружённая пробелами или границами строки |
| `void`       | `toLowerCase(std::string &s)`                                        | Приводит все буквы в строке к нижнему регистру                                                                                                                      |
| `void`       | `deletePunctuationMarks(std::string &s)`                             | Удаляет из строки знаки пунктуации                                                                                                                                  |
| `void`       | `deleteExtraSpaces(std::string &s)`                                  | Удаляет из строки лишние пробелы                                                                                                                                    |
| `void`       | `writeJsonToFile(const nlohmann::json &json_text , const std::string &path)` | Записывает JSON объект в файл по переданному пути. Если файл не существует, он будет создан, иначе, файл будет перезаписан                                          |
| `std::string`| `std::string getFileName(std::string s)`                             | Возвращает имя файла по заданному пути. Удобно использовать, когда вам нужно имя файла, а не его расположение                                                       |
| `size_t`     | `countOccurrences(const std::string &text, const std::string &word)` | Возвращает количество вхождений слова word в тексте text                                                                                                            |
| `std::vector<std::string>`        | `getUniqueWords(const std::string &text)`                           | Возвращает массив содержащий все уникальные слова из текста                                                                                                         |

---
#### <font color="orange"> "converter_json.h" </font>
Здесь объявлен класс ConverterJSON, содержащий методы для работы с JSON файлами.
Все методы объявлены как статичные, поэтому класс можно использовать не создавая экземляров. Пути к файлам и их имена юерет из <font color=orange>"project_constants.h"</font>

| Return                     | Name                | Description                                                                                |
|----------------------------|---------------------|--------------------------------------------------------------------------------------------|
| `std::<std::string>`       | `GetTextDocuments()` | Возвращает массив содержимого текстовых файлов, указанных в config.json как область поиска |
| `int`                      | `GetResponsesLimit()` | Возвращает максимальное количество результатов на каждый запрос                            |
| `std::vector<std::string>` | `GetRequests()`      | Возвращает массив, содержащий запросы из requests.json                                     |
| `void`                      | `putAnswers(std::vector<std::vector<RelativeIndex>> answers)`                    | Помещает answers в json формате в файл answers.json                                        |

---
#### <font color="orange"> "inverted_index.h" </font>
Структура Entry нужна для индексации слов в документах
                
    struct Entry {
        size_t doc_id;
        size_t count;

        bool operator==(const Entry &other) const {
        return (doc_id == other.doc_id && count == other.count);
        }
    };

class InvertedIndex индексирует слова в текстах документов

    Сlass InvertedIndex {

        std::vector<std::string> docs;

        std::map<std::string, std::vector<Entry>> freq_dictionary;

        std::mutex dict_acces;

    };

| field           | Description                                                                                                |
|-----------------|------------------------------------------------------------------------------------------------------------|
| `docs`            | Хранит документы по которым происходит индексация слов                                                     |
| `freq_dictionary` | Класс формирует словарь содержащий частоту вхождений слова в каждом документе                              |
| `dict_acces`      | Так как, индексация слов происходит в нескольких потоках, мьютекс необходим для контроля доступа к словарю |

Constructors:
    
    InvertedIndex() = default;

    InvertedIndex(const InvertedIndex &other)
        : docs(other.docs), freq_dictionary(other.freq_dictionary) {};

Public methods

| Return | Name                | Description                                                                                                       |
|--------|---------------------|-------------------------------------------------------------------------------------------------------------------|
| `void`   | `UpdateDocumentBase(std::vector<std::string> input_docs)`                    | Запускает идексацию слов в массиве документов input_docs, реузльтатом работы является частотный словарь freq_dict |
| `std::vector<Entry>`       | `getWordCount(const std::string &word)`                    | Возвращает частоту вхождения слова word в массиве документов docs                                                 |

---
#### <font color="orange">"search_server.h</font>
Класс SearchServer осуществляет поиск по индексированным документам, принимает запросы, обрабатывает их и формирует итоговую структуру с результатами запросов. 

    class SearchServer {

        InvertedIndex _index;
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

Constructor

    explicit SearchServer(InvertedIndex &idx) : _index(idx) {};

Структура содержит один публичный метод, который и производи поиск

    std::vector<std::vector<RelativeIndex>> search(
            const std::vector<std::string> &queries_input);

queries_input представляет из себя вектор содержащий запросы. Запросы парсятся на уникальные слова, по ним в частотном словаре происходит поиск релевантных документов, формируется
список таких документов, после чего определяется их релевантность для данного запроса и так для всех запросов из queries_input

---

#### Как запустить?
Ниже приведена инструкция для сборки проекта в среде Linux, а именно Ubuntu 22.04.3 LTS. \
На MacOS, думаю, действия будут не сильно отличаться. Если же вы используете Windows, мне остается вам только позавидовать и пожелать удачи,
но на всякий случай обновите MinGW и cmake, всё должно быть ок. \
Приступим:
1. Скачайте архив
2. Распакуйте в удобную директорию
3. Соберите проект с помощью cmake. Если вы используйте IDE поддерживающую cmake просто откройте распакованный архив как проект, она все сделает за вас, можете сразу перейти к пункту (4). Если же вы ведёте интересную жизнь:
   * Открываем консоль и заходим в папку с распакованным проектом. Пишем в консоль:\
   `mkdir build && cd build` \
   Должно получиться как-то так
     ![first_step](https://imageup.ru/img137/4488563/first.jpg)

   * Теперь надо сконфигурировать проект. Снова пишем: \
   `cmake ../../search_engine-master/` \
   Если получилось как -то так, то вы на верном пути
   ![second_step](https://imageup.ru/img227/4488567/second.png)
   * Теперь самое интересное \
   `cmake --build .` \
   Надеюсь, что у вас получилось примерно как на картинке
     ![third_step](https://imageup.ru/img135/4488577/third.png)
   * Теперь вы можете обнаружить исполняемый файл
     ![imageup.ru](https://imageup.ru/img102/4488587/fourth.png) \
   Да, зеленёнький search_engine это оно. Также в паке test, лежит бинарник с тестами, можете запустить его сразу, чтобы проверить, что всё ок
   
4. Вы ещё здесь? Здорово, теперь расскажу, как всем этим пользоваться.
    * В корневой директории проекта находим файл config.json
      ![config](https://imageup.ru/img156/4488598/config.png) \
   Здесь, нас интересуют поля max_responses и массив files. \
   В поле max_responses вы можете указать максимальное количество результатов поиска. \
   В массиве files перечисленны пути к файлам, лежащим в папке resources, которая также находится в корневой директории проекта. \
   Если вы хотите добавить или удалить файл в котром будет происходить поиск, также учтите их в массиве files
   Файлы для поиска следует добавлять в папку resources
   Обратите внимание, что документ содержит не более 1000 слов и длина каждого слова не более 100 символов
    * В файле requests.json следует указывать ваши запросы, но не более 1000, ну и добавлю что не меньше 1, иначе было бы странно 
   ![requests](https://imageup.ru/img73/4488602/requests.png) \
   Просто указываем слова для поиска, в формате как на картинке. Запрос может состоять как из одного так и из нескольких слов, но не более 10
   * После запуска программы, она запишет в папку JSONs файл results.json, он выглядит как-то так
   ![results](https://imageup.ru/img244/4488606/results.png) \
   Это и есть интересующий нас результат.
   Наверно, стоит обратить внимание, что у тестов, своя папка с json файлами и текстом для поиска, они также лежат в папке test и содержат тестовые данные