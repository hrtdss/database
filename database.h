#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <fstream>
#include <exception>
#include <stdexcept>
#include <iomanip>

using namespace std;

// ������� ����� ��� ������
struct dbError { 
   virtual ~dbError() {}; // ����������
   virtual const char *what() const = 0; // ��������� ��� ������
};

struct editingImpossible : dbError {
   const char *what() const { return "Editing mode has not been activated."; } // ��������� ��� ������
};

struct alreadyApplied : dbError {
   const char *what() const { return "Operation has already been applied."; } // ��������� ��� ������
};

// ������� �����
class DataBase {
   string dbName; // �������� �����
   fstream inOut; // ����� ��� �����/������

   bool editMode; // �������� �� ��������� ������ ��������� (0 - �� �������, 1 - �������)

   public:
      DataBase(string name); // ����������� �����
      void Open(); // ������� ����
      void Close(); // ������� ����
      long Id(); // ������������� ������ (�������� � �����)
      void Goto(long id); // ������� �� ������ � ��������������� id
      void First(); // ������� �� ������ ������ � �����
      void Next(); // ������� �� ��������� ������ � �����
      void Prev(); // ������� �� ���������� ������ � �����
      void Last(); // ������� �� ��������� ������ � �����
      void Post(); // �������� ���������
      void Cancel(); // �������� ���������, ���������� ������
      void Insert(); // �������� ����� ������
      void Edit(); // ����� � ����� ���������
      void Delete(); // ������� ������
      bool Eof(); // ���� ���� ��� ��������� ����� ����� ��� ���������� Next()
      bool Bof(); // ���� ���� ��� ���������� ������ ����� ��� ���������� Prev()
      long count(); // ���������� �������
   
   protected:
      int amountOfNotes; // ���������� �������
      long currentId; // ����� ������, ��������������� � ������� ������ ������� (������� � 0)
      int first, last; // ��������� �� ������ � ��������� ������
      int next, prev; // ��������� �� ��������� � ���������� ������
      int recordSize; // ������ ����� ������ (� ������)
      int firstByte; // ����� ����� � �������� �������� ������������(���������) ������ � �����
      int firstEmpty; // ��������� �� ������ ��� ������� ������
      
      virtual void ReadData(fstream &f) = 0; // ������ ����� �� �����
      virtual void WriteData(fstream &f) = 0; // ������ ����� � ����
      virtual int Size() = 0; // ������ ������ (��� ������ ��� �������� ������. �������)
};

// ����������� �����(�������� ������)
class Person : public DataBase {
   string fullName[3]; // �������, ��� � ��������
   string country; // ������ ����������
   int dayOfBirth, monthOfBirth, yearOfBirth; // ����, ����� � ��� ��������

   public:
      Person(string name): DataBase(name) {}; // �����������
      void setLastName(const string &a); // ������ �������
      void setFirstName(const string &a); // ������ ���
      void setMiddleName(const string &a); // ������ ��������
      string getLastName() const; // �������� �������
      string getFirstName() const; // �������� ���
      string getMiddleName() const; // �������� ��������
      void setCountry(const string &a); // ������ ������
      string getCountry() const; // �������� ������
      void setDateOfBirth(const int a, const int b, const int c); // ������ ����, ����� � ��� ��������
      string getDateOfBirth() const; // �������� ���� ��������
   
   protected:
      void ReadData(fstream &f);
      void WriteData(fstream &f);
      int Size();
};

#endif