#include "database.h"

DataBase::DataBase(string name) {
   amountOfNotes = 0;
   currentId = -1;
   first = -1;
   last = -1;
   next = -1;
   prev = -1;
   firstEmpty = 0;
   
   editMode = false;

   dbName = name;
   inOut.open(dbName, ios::app);
   inOut.close();
}

void DataBase::Open() {
   if (inOut.is_open())
      throw alreadyApplied();
   
   inOut.open(dbName, ios::in | ios::out | ios::binary);
   
   // Если файл уже был когда-то создан - считываем заголовок 
   // (кол-во записей, максимальный размер одной записи, номер первой записи, номер последней записи)
   if (inOut.peek() != EOF) { 
      int amout, size, start, end, empty;
      inOut >> amout >> size >> start >> end >> empty;

      firstByte = inOut.tellg();
      inOut.seekg(firstByte + 1, ios::beg);
      
      amountOfNotes = amout;
      currentId = 0;
      recordSize = size;
      first = start;
      last = end;
      firstEmpty = empty;
   }
   
   if (amountOfNotes > 1)
      next = 1; 
}

void DataBase::Close() {
   if (!inOut.is_open())
      throw alreadyApplied();
   
   inOut.seekg(0, ios::beg);
   inOut << amountOfNotes << ' ' << recordSize << ' ' << first << ' ' << last << ' ' << firstEmpty << '\n';
   inOut.close();
}

long DataBase::Id() {
   return currentId;
}

void DataBase::Goto(long id) {
   if (id < 0 || id >= amountOfNotes)
      throw out_of_range("Unable to navigate to the specified element.");

   if (id == 0)
      First();
   else if (id == amountOfNotes - 1)
      Last();
   else {
      currentId = id;
      next = id + 1;
      prev = id - 1;
   }
}

void DataBase::First() {
   if (amountOfNotes == 0)
      throw out_of_range("Command cannot be executed.");
   
   currentId = first;
   next = 1;
   prev = -1;
}

void DataBase::Next() {
   if (Eof()) {
      throw out_of_range("Command cannot be executed."); }

   currentId++;
   next = (next + 1 == amountOfNotes) ? -1 : next + 1;
   prev++;
}

void DataBase::Prev() {
   if (Bof())
      throw out_of_range("Command cannot be executed.");

   currentId--;
   next--;   
   prev = (prev - 1 < 0) ? -1 : prev - 1;
}

void DataBase::Last() {  
   if (amountOfNotes == 0)
      throw out_of_range("Command cannot be executed.");
   
   currentId = last;
   next = -1;
   prev = currentId - 1;
}

void DataBase::Post() {
   if (!editMode)
      throw editingImpossible();
   
   WriteData(inOut);
   editMode = false;
}

void DataBase::Cancel() {
   if (!editMode)
      throw editingImpossible();
   
   ReadData(inOut);
   editMode = false;
}

void DataBase::Insert() {
   if (!editMode)
      throw editingImpossible();
   
   int h = firstByte + (firstEmpty + 1) + recordSize * firstEmpty;   
   
   bool fl = false;
   for (int i = first; i < last; i++) {
      inOut.seekg(firstByte + (i + 1) + recordSize * i, ios::beg);
      if (inOut.peek() == ' ') {
         firstEmpty = i; 
         fl = true;
         break;
      }
   }
   
   if (!fl) {
      amountOfNotes++;
      firstEmpty = amountOfNotes;
      last = amountOfNotes - 1;
      currentId = last;
   }
   
   inOut.seekg(h, ios::beg);
}

void DataBase::Edit() {
   editMode = (!editMode) ? true : false;

   inOut.seekg(firstByte + (currentId + 1) + recordSize * currentId, ios::beg);
}

void DataBase::Delete() {
   if (!editMode)
      throw editingImpossible();
   
   inOut.seekg(firstByte + (currentId + 1) + recordSize * currentId, ios::beg);
   
   for (int i = 0; i < recordSize; i++)
      inOut << ' ';
      
   inOut << '\n';
}

bool DataBase::Eof() {
   return (inOut.peek() == EOF || next == -1) ? true : false;
}

bool DataBase::Bof() {
   return (inOut.peek() == EOF || prev == -1) ? true : false; 
}

long DataBase::count() {
   return amountOfNotes;
}

void Person::setLastName(const string &a) {
   if (a.size() > 30)
      throw length_error("The last name is too long.");

   fullName[0] = a;
}

void Person::setFirstName(const string &a) {
   if (a.size() > 30)
      throw length_error("The first name is too long.");

   fullName[1] = a;
}

void Person::setMiddleName(const string &a) {
   if (a.size() > 30)
      throw length_error("The middle name is too long.");

   fullName[2] = a;
}

string Person::getLastName() const {
   return fullName[0];
}

string Person::getFirstName() const {
   return fullName[1];
}

string Person::getMiddleName() const {
   return fullName[2];
}

void Person::setCountry(const string &a) {
   if (a.size() > 20)
      throw length_error("The country name is too long.");

   country = a;
}

string Person::getCountry() const {
   return country;
}

void Person::setDateOfBirth(const int a, const int b, const int c) {
   if ((a < 1 && a > 31) || (b < 1 && b > 12) || c < 1)
      throw out_of_range("Entered date of birth does not exist.");

   dayOfBirth = a;
   monthOfBirth = b;
   yearOfBirth = c;
}

string Person::getDateOfBirth() const {
   return to_string(dayOfBirth) + "." + to_string(monthOfBirth) + "." + to_string(yearOfBirth);
}


void Person::ReadData(fstream &f) {
   string s[5], s1;
   int a[3];

   for (int i = 0; i < 5; i++)
      f >> s[i];
   
   for (int i = 0; i < 4; i++)
      s[i].erase(s[i].find_first_of("_"));
   
   setLastName(s[0]);
   setFirstName(s[1]);
   setMiddleName(s[2]);
   setCountry(s[3]);
   
   for (int i = 0; i < 2; i++) {
      s1 = s[4].substr(0, s[4].find_first_of(".", 0));
      s[4].erase(0, s[4].find_first_of(".", 0) + 1);
      a[i] = stoi(s1);
   }
   a[2] = stoi(s[4]);

   setDateOfBirth(a[0], a[1], a[2]);
}

void Person::WriteData(fstream &f) {
   f.setf(ios::left);
   f << setfill('_') << setw(30) << getLastName() << ' ';
   f << setfill('_') << setw(30) << getFirstName() << ' ';
   f << setfill('_') << setw(30) << getMiddleName() << ' ';
   f << setfill('_') << setw(20) << getCountry() << ' ';
   f << setfill('_') << setw(10) << getDateOfBirth() << '\n'; 
}

int Person::Size() {
   return recordSize;
}
