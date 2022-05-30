#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <fstream>
#include <exception>
#include <stdexcept>
#include <iomanip>

using namespace std;

// Базовый класс для ошибок
struct dbError { 
   virtual ~dbError() {}; // деструктор
   virtual const char *what() const = 0; // сообщение для печати
};

struct editingImpossible : dbError {
   const char *what() const { return "Editing mode has not been activated."; } // сообщение для печати
};

struct alreadyApplied : dbError {
   const char *what() const { return "Operation has already been applied."; } // сообщение для печати
};

// Базовый класс
class DataBase {
   string dbName; // Название файла
   fstream inOut; // Поток для ввода/вывода

   bool editMode; // Проверка на активацию режима изменений (0 - не активен, 1 - активен)

   public:
      DataBase(string name); // Конструктор файла
      void Open(); // Открыть файл
      void Close(); // Закрыть файл
      long Id(); // идентификатор записи (смещение в файле)
      void Goto(long id); // перейти на запись с идентификатором id
      void First(); // перейти на первую запись в файле
      void Next(); // перейти на следующую запись в файле
      void Prev(); // перейти на предыдущую запись в файле
      void Last(); // перейти на последнюю запись в файле
      void Post(); // записать изменения
      void Cancel(); // отменить изменения, перечитать запись
      void Insert(); // добавить новую запись
      void Edit(); // войти в режим изменений
      void Delete(); // удалить запись
      bool Eof(); // файл пуст или обнаружен конец файла при выполнении Next()
      bool Bof(); // файл пуст или обнаружено начало файла при выполнении Prev()
      long count(); // количество записей
   
   protected:
      int amountOfNotes; // Количество записей
      long currentId; // Номер записи, рассматриваемой в текущий момент времени (начиная с 0)
      int first, last; // Указатель на первую и последнюю записи
      int next, prev; // Указатель на следующую и предыдущую записи
      int recordSize; // Размер одной записи (в байтах)
      int firstByte; // Номер байта с которого начинают записываться(храниться) записи в файле
      int firstEmpty; // Указатель на строку для вставки записи
      
      virtual void ReadData(fstream &f) = 0; // чтение полей из файла
      virtual void WriteData(fstream &f) = 0; // запись полей в файл
      virtual int Size() = 0; // размер записи (все записи для простоты одинак. размера)
};

// Производный класс(хранимый объект)
class Person : public DataBase {
   string fullName[3]; // Фамилия, имя и отчество
   string country; // Страна проживания
   int dayOfBirth, monthOfBirth, yearOfBirth; // День, месяц и год рождения

   public:
      Person(string name): DataBase(name) {}; // Конструктор
      void setLastName(const string &a); // Задать фамилию
      void setFirstName(const string &a); // Задать имя
      void setMiddleName(const string &a); // Задать отчество
      string getLastName() const; // Получить фамилию
      string getFirstName() const; // Получить имя
      string getMiddleName() const; // Получить отчество
      void setCountry(const string &a); // Задать страну
      string getCountry() const; // Получить страну
      void setDateOfBirth(const int a, const int b, const int c); // Задать день, месяц и год рождения
      string getDateOfBirth() const; // Получить дату рождения
   
   protected:
      void ReadData(fstream &f);
      void WriteData(fstream &f);
      int Size();
};

#endif