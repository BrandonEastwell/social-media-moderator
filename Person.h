// include your own comments

#ifndef _PERSON_H_
#define _PERSON_H_

#include <string>

using namespace std;

class Person
{   public:
		Person();
    	Person(const string &name);
	   string getName() const;
	   virtual void changeName(const string &newName); //virtual function to make class polymorphic for dynamic casting
    protected:
	   string name;
};

#endif
