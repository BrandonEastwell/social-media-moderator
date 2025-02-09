// include your own comments

#ifndef _MANAGER_H_
#define _MANAGER_H_

#include <string>
#include <vector>
#include <stdexcept>
#include "Person.h"

using namespace std;

class Manager: public Person
{   public:
        // constructor should initialise name and registration number using arguments
        Manager(const string &name, int regNo);

	   // method to return registration number
		int getRegNo() const;

		string getName() const;

		// method to write banned words to blocked content file
		void writeblockedContent();

		// method to read blocked content from file and store in a vector
		vector<string> readblockedContent();

		//method to reset blocked content file
		void resetblockedContent();

		//method to add a new manager to the user file
		void addManager(vector<Manager>&Managers);
    private:
	   int regNo;
	   string name;
};

#endif
