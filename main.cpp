#include "database.h"

int main() {
   string ss = "test.txt";
   Person p(ss);
   
   p.Open();

   p.Edit();
   p.Insert();
   p.setFirstName("Kataev");
   p.setLastName("Egor");
   p.setMiddleName("Konstantinovich");
   p.setCountry("Russia");
   p.setDateOfBirth(10, 10, 10);
   p.Post();
   
   p.Goto(1);
   p.Edit();
   p.Delete();
   p.Delete();
   
   p.Close();

   return 0;
}