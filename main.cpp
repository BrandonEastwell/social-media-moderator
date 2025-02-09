#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "Manager.h"
#include "User.h"
#include "ReadTweets.h"
#include <iomanip>

using namespace std;

class Syslogin { //system login class
    public:
    vector<User> userProfiles; //user vectors store all users
    vector<Manager> managerProfiles; //user vectors store all managers
    template <typename T> 
    T Login(string fname, string sname) { //template function for login system
        userProfiles.clear();
        ifstream FileIn("users.txt"); //input stream for users file
        string line, userID, firstName, secondName, accumulativeScore, averageScore, name, name2;
        name = fname + " " + sname;
        bool found = false;

        if (FileIn.is_open()) {
            while (getline(FileIn, line)) { //while a line exists in file
                string accumulativeScore = "", averageScore = "";
                stringstream ss {};
                ss << line;
                getline(ss, userID, ' ');
                getline(ss, firstName, ' ');
                getline(ss, secondName, ' ');
                getline(ss, accumulativeScore, ' ');
                getline(ss, averageScore, ' ');
                transform(firstName.begin(), firstName.end(), firstName.begin(), ::tolower);
                transform(secondName.begin(), secondName.end(), secondName.begin(), ::tolower);
                name2 = firstName + " " + secondName;
                if (accumulativeScore != "") //if accumulative score has a value
                {
                    //create a complete user object with attributes from file and append to user profiles vector
                    User user = User(name2, stoi(userID), stoi(accumulativeScore), stoi(averageScore));
                    userProfiles.push_back(user);
                } else if (accumulativeScore == "")
                {
                    Manager manager = Manager(name2, stoi(userID));
                    managerProfiles.push_back(manager);
                }
                
                if (name == name2 && accumulativeScore == "") //if name match and accumulative score doesnt have a value
                {
                    //condition for manager 
                    found = true;
                }
            }

            if (found) //manager
            {
                //create and return pointer manager object
                Manager *manager_ptr = new Manager(name, stoi(userID));
                FileIn.close();
                return manager_ptr;
            }
            

            for (int i = 0; i < userProfiles.size(); i++) //iterate through users vector
            {
                if (userProfiles[i].getName() == name) { //find a matching name
                    //create and return pointer user object
                    User *user_ptr = new User(name, userProfiles[i].getRegNo(), userProfiles[i].getAccumulativeScore(), userProfiles[i].getAverageScore());
                    FileIn.close();
                    return user_ptr;
                }
            }
            if (!found) //no user or manager is found
            {
                //create output stream to user file and append a new user to the file
                ofstream FileOut("users.txt", ios::app);  
                int randomID = (int)(((((double)std::rand()) / RAND_MAX) * 99999999) + 1);
                cout << endl << randomID << " " << fname << " " << sname << " " << 100 << " " << 100;
                FileOut << endl << randomID << " " << fname << " " << sname << " " << 100 << " " << 100;
                FileOut.close();
                User *newuser = new User(name, randomID, 100, 100); //new pointer user object with new user added to vector and returned
                User mainuser = User(name, randomID, 100, 100);
                userProfiles.push_back(mainuser);
                return newuser;
            }
            
        } else
        {
            cout << endl << "failed to open users file";
            exit(1);
        }
        
    }
};

main()
{
    srand(time(NULL)); //generate a different seed for random numbers per program compilation
    Syslogin login; //login function template that holds the vector of users
    string name1, name2;
    int y, z;
    bool condition = true;

    ifstream FileIn("config.txt");
    string line;
    getline(FileIn, line);
    const char *fileName = line.c_str();
    FileIn.close();

    cout << endl << "Enter first name: "; //ask for name
    getline(cin >> ws, name1); //take first name as an input and store it in string name
    cout << endl << "Enter second name: ";
    getline(cin >> ws, name2); //take second name as an input and store it in string name2
    transform(name1.begin(), name1.end(), name1.begin(), ::tolower);
    transform(name2.begin(), name2.end(), name2.begin(), ::tolower);

    //dynamic casting to cast class object return value to a user and manager object
    Person *person_ptr = login.Login<Person*>(name1, name2);
    User *user = dynamic_cast<User*>(person_ptr);
    Manager *manager = dynamic_cast<Manager*>(person_ptr);

    while (manager != nullptr) { //nullptr is return value for dyanmic cast failure
        //Manager menu system
        cout << endl << endl << "Please select a number for the option of:" << endl << "1 - Reset Moderation Content" << endl << "2 - Add Blocked Content" << endl << "3 - Add Another Manager" << endl << "4 - Set The Sample File" << endl << "5 - Exit"  << endl << endl << "Option: ";
        cin >> y;
        if (y == 1)
        {
            manager->resetblockedContent(); //clears blocked content csv file
        } else if (y == 2)
        {
            manager->writeblockedContent(); //write to the blocked content csv file
        } else if (y == 3)
        {
            manager->addManager(login.managerProfiles); //add a new manager to the users txt file
        } else if (y == 4)
        {
            cout << endl << endl << "Enter the new name of tweet sample file: ";
            getline(cin >> ws, line);
            ofstream FileOut("config.txt"); //writes new name to config file to save file name
            line += ".txt"; //appends file format to the end of name
            FileOut << line; //writes to file
            FileOut.close();
                
        } else if (y == 5)
        {
            exit(1);
        } else
        {
            cout << endl << "Please enter a value between 1-5" << endl;
        }
    }

    //Opens tweet sample file
    ReadTweets file = ReadTweets(fileName);
    file.getTweets(login.userProfiles); //reads all tweets into user multimap corresponding to registration 

    while (user != nullptr) { //nullptr is return value for dyanmic cast failure
        //User menu system
        cout << endl << endl << "Please select a number for the option of:" << endl << "1 - Read/Write Tweet" << endl << "2 - Your Statistics" << endl << "3 - Display top 10 most dangerous users" << endl << "4 - Exit"  << endl << endl << "Option: ";
        cin >> y;
        if (y == 1) { //read or write a tweet from the sample file
            cout << endl << endl << "Please select a number for the option of:" << endl << "1 - Read Tweet" << endl << "2 - Write Tweet" << endl << endl << "Option: ";
            cin >> z;
            if (z == 1) //read a tweet
            {
                string condition;
                int RegNo;
                RegNo = file.randomTweet(login.userProfiles); //output a random tweet from sample file
                cout << endl << endl << "Would you like to report the tweet? Yes/No ";
                getline(cin >> ws, condition);
                if (condition == "Yes" ) {
                    for (int i = 0; i < login.userProfiles.size(); i++) //iterate through users vector
                    {
                        if (login.userProfiles[i].getRegNo() == RegNo) //finds a matching registration number with reported registration number
                        {
                            login.userProfiles[i].reportTweetScore(file.last_tweet); //deducts a score point from the users tweets score
                            cout << endl << "reported";
                        }    
                    }   
                }
            } else if (z == 2) //write a tweet
            {
                int RegNo = user->getRegNo(); //save logged in users reg no
                file.WriteTweet(RegNo); //write tweet to sample file function
            }

            for (int i = 0; i < login.userProfiles.size(); i++) //iterate through users vector
            {
                 //executed after all read and write functions that could modify scores
                login.userProfiles[i].updateAccumulativeScore(); //recalculate the accumulative score attribute
                login.userProfiles[i].updateAverageRepScore(); //recalculate the average score attribute
            }
                
        } else if (y == 2) { //output logged in users score details
            for (int i = 0; i < login.userProfiles.size(); i++) //iterate through users vector
            {
                if (login.userProfiles[i].getRegNo() == user->getRegNo()) //compare reg no with logged in user
                {
                    cout << login.userProfiles[i]; //output details with overidden << operator
                    break;
                }  
            }
        } else if (y == 3) { //finding 10 most dangerous users
            sort(login.userProfiles.begin(),login.userProfiles.end(),[](User& lhs, User& rhs) { //sort function with users vector
                if (lhs.getAverageScore() == rhs.getAverageScore()) //if average scores are equal
                {
                    return lhs.getModeratedTweets() < rhs.getModeratedTweets(); //order based on amount of moderated tweets
                }
                    
                return lhs.getAverageScore() < rhs.getAverageScore(); //order based on average score
                });
            cout << "Order from most dangerous to least dangerous being 10." << endl;
            for (int count = 1; count < 11; count++)
            {
                cout << count << ": " << login.userProfiles[count-1].getName() << endl; //output names of top 10 from sorted users vector
            }

        } else if (y == 4) { //exit out of system
            ofstream FileOut("users.txt", ios::out | ios::trunc);
            for (int i = 0; i < login.userProfiles.size(); i++)
            {   //writes all changes to the scores to users file.
                FileOut << login.userProfiles[i].getRegNo() << " " << login.userProfiles[i].getName() << " " << login.userProfiles[i].getAccumulativeScore() << " " << login.userProfiles[i].getAverageScore() << endl;
            }
            for (int i = 0; i < login.managerProfiles.size(); i++)
            {   //writes all managers to the user file.
                FileOut << setfill('0') << setw(4) << login.managerProfiles[i].getRegNo() << " " << login.managerProfiles[i].getName() << endl;
            }
            FileOut.close();
            file.close();
            exit(1);
            break;
        } else
        {
            cout << endl << "Please enter a value between 1-4" << endl;
        }
    }
};
