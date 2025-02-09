#include "Manager.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
Manager::Manager(const string &name, int regNo) {
    this->name = name;
	this->regNo = regNo;
}

int Manager::getRegNo() const { //returns reg no attribute
    return regNo;
}

string Manager::getName() const { //returns name attribute
    return name;
}

vector<string> Manager::readblockedContent() {
    //reads blocked phrases and words from the blocked content csv file into a vector
    vector<string> blockedContent;
    string line, substr;
    ifstream FileIn("blocked_content.txt"); //open file into input stream
    if (!FileIn)
    {
        cout << "failed to open blocked content file";
        exit(1);
    }
    
    getline(FileIn, line);
    stringstream ss(line);
    while (ss.good())
    {
        getline(ss, substr, ',');
        blockedContent.push_back(substr); //split line read from file by a comma delim and appended to the blocked content vector
    }
    FileIn.close(); //closes file
    return blockedContent; //returns vector of blocked words
}

void Manager::writeblockedContent() {
    string phrase, condition;
    bool exists = false;
    while (true) {
        cout << endl << "Enter a word or phrase you want to block: ";
        getline(cin >> ws, phrase);
        transform(phrase.begin(), phrase.end(), phrase.begin(), ::tolower);
        vector<string> blockedContent = readblockedContent();
        if (blockedContent.size() == 0)
        {
            for (int i = 0; i < blockedContent.size(); i++)
            {
                if (blockedContent[i] == phrase)
                {
                    exists = true;
                    break;
                }
            }
        }

        if (exists == false)
        {
            ofstream FileOut("blocked_content.txt", ios::app);
            FileOut << phrase << ",";
            cout << endl << phrase << " added to blocked content" << endl;
            cout << endl << name << ", would you like to enter another word or phrase? Yes/No ";
            getline(cin >> ws, condition);
            if (condition == "Yes")
            {
                continue;
            } else
            {
                FileOut.close();
                break;
            }
        }     
    }
}

void Manager::resetblockedContent() {
    //clears the file of content
    ofstream FileOut("blocked_content.txt", ios::trunc); //truncate file
    if (!FileOut)
    {
        cout << endl << "failed to open blocked content file";
        exit(1);
    }
    cout << endl << "reset blocked content file";
    
    FileOut.close();
}

void Manager::addManager(vector<Manager>&Managers) {
    //function to add a new manager to the users file
    srand(time(NULL)); //random seed for randomizer
    string name1, name2, name;
    cout << endl << "Enter new managers first name: "; //ask for name
    getline(cin >> ws, name1); //take first name as an input and store it in string name
    cout << endl << "Enter new managers second name: ";
    getline(cin >> ws, name2); //take second name as an input and store it in string name2

    int randomID = (int)(((((double)std::rand()) / RAND_MAX) * 99999999) + 1); //random ID generator with 8 digits
    name = name1 + " " + name2;
    Manager manager = Manager(name, randomID); //add new manager to the managers vector
    Managers.push_back(manager);
}