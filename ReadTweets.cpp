// replace my comments with your own
// except: you can use my comment about one-word lookahead

// you may modify the code that I have supplied if you wish


using namespace std;

#include "ReadTweets.h"
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <iomanip>

string removePunct(string word)
{ // should return a copy of the word with all leading and trailing punctuation      
  // removed
  // punctuation elsewhere in the word should not be removed
  // note that a word could havce more than one leading/trailing punctuation symbols
  // you need to write this
   int x = 0;
   int y = word.size() - 1;
   while ( x <= y && !isalnum( word[x] ) ) {
    x++;
   }
   while ( x <= y && !isalnum( word[y] ) ) {
    y--;
   }
   if ( x > y ) {
    return "";
   } else {
    return word.substr( x, y - x + 1 );
   }
}
void ReadTweets::WriteTweet(int RegNo) {
  //function to write a tweet to the sample file with a 140 character limit
  //using built in time functions stored the current system time and date in short format matching the date/time format in file.
    string phrase, condition;
    bool exists;
    srand(time(NULL));
    while (wordfileOut.is_open()) {
        cout << endl << "Write a tweet in 140 characters: ";
        getline(cin >> ws, phrase);
        if (phrase.size() <= 140)
        {
          int randomID = (int)(((((double)std::rand()) / RAND_MAX) * 99999999) + 1);
          time_t ttime = time(0);
          tm *ltm = localtime(&ttime);
          int year = ltm -> tm_year + 1900;
          int month = ltm -> tm_mon + 1;
          int day = ltm -> tm_mday;
          int hour = ltm -> tm_hour;
          int minute = ltm -> tm_min;
          int second = ltm -> tm_sec;
          wordfileOut << endl << RegNo << "\t" << randomID << "\t" << phrase << "\t" << setfill('0') << setw(4) << year << "-" << setfill('0') << setw(2) <<  month << "-" << day << " " << hour << ":" << minute << ":" << second; 
          wordfileOut.close();
        } else
        {
          cout << endl << "Tweet exceeded 140 characters.";
        }
        
        
    }
}

ReadTweets::ReadTweets(const char *fname)
{   wordfileIn.open(fname);//open file in input mode
    wordfileOut.open(fname, ios::app);//open file in output mode
    if (!wordfileIn || !wordfileOut)
    {   cout << "Failed to open " << fname << endl;
        exit(1);
    }
    
    wordfileIn >> nextword;
    eoffound = false;
}

void ReadTweets::readblockedContent() {
  //function to read the blocked phrases and words written to the blocked content csv file
    string line, substr;
    ifstream FileIn("blocked_content.txt"); //input stream to csv file
    getline(FileIn, line); //gets first line
    stringstream ss {};
    ss << line;
    while (ss.good())
    {
        getline(ss, substr, ',');
        blockedContent.push_back(substr); //appends split content by comma to blocked content vector
    }
    if (!FileIn)
    {
      cout << "Failed to open blocked_content.txt";
      exit(1);
    }
    
    FileIn.close();
}


int ReadTweets::randomTweet(vector<User>&Users) {
  //outputs a random tweet from the tweet sample file
  last_tweet = "";
  string moderatedTweet;
  bool found;
  bool moderated = false;
  int random = rand() % tweets.size(); //random number within amount of tweets readable
  auto itr = tweets.begin(); //iterator to iterate through tweets multimap
  advance(itr, random); //moves iterator to the random number generated
  last_tweet = itr -> second; //stores random tweet from multimap to a string
  getAllWords(last_tweet); //splits tweet into words and stores into vector
  readblockedContent(); 

  for (int i = 0; i < tweetWords.size(); i++) //iterates through each word in tweet
  {
    found = false;
    string stripedWord = removePunct(tweetWords[i]); //strips leading and trailing punctuation from word
    for (int y = 0; y < blockedContent.size(); y++) //iterates through blocked words
    {
      if (stripedWord == blockedContent[y]) // if tweet word is equal to blocked word
      {
        moderated = true; //condition for moderated tweet tag
        string stars = "";
        for (int x = 0; x < stripedWord.size(); x++) { //builds a stars string equivalent to length of blocked word
          stars += '*';
        }
        moderatedTweet += " " + stars; //appends censored word to the moderated tweet
        for (int c = 0; c < Users.size(); c++) //iterates through users vector
        {
          if (itr -> first.getRegNo() == Users[c].getRegNo()) //finds a matching reg no with tweets user reg
          {
            Users[c].reportBlockedContent(last_tweet); //reduces tweet score of the user who wrote the tweet
            break;
          }
          
        }
        found = true;
        break;
      }
    }
    if (!found)
    {
      moderatedTweet += " " + tweetWords[i]; //appends normal word to moderated tweet if no moderation was needed
    }
  }
  if (moderated)
  {
    cout << endl << moderatedTweet << " #moderatedtweet"; //moderated tweet tag if tweet had to be moderated
  } else {
    cout << endl << moderatedTweet;
  }
  int RegNo = itr -> first.getRegNo(); //function returns the reg no of the user who wrote the tweet
  return RegNo;
}

void ReadTweets::getAllWords(string tweet) {
  //splits all words in a tweet to a vector
    tweetWords.clear();
    string substr;
    stringstream ss(tweet);
    while (ss.good())
    {
        getline(ss, substr, ' ');
        tweetWords.push_back(substr);
    }
}

string ReadTweets::getNextWord()
{ // should return next word in file, converted to lower case
  // empty string should be returned if next word contains no letters
  
  // uses a one-word lookahead to avoid any problems relating to when if end-of-file
  // is detected due to absence/presence of newline at end of file

  // incomplete

  string word = nextword;
  wordfileIn >> nextword;

  if (wordfileIn.eof())
     eoffound = true;
     return "";

  if (word.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"))
  {
    return "";
  } else if (eoffound == false)
  {
    word = removePunct(word);
    return word;
  }

  // need to check that word contains a letter, and if not return an empty string;
  // also need to convert to lower case before returning
}

bool ReadTweets::isNextWord()
{ return !eoffound;
}

void ReadTweets::getTweets(vector<User>&Users)
{
  //reads all tweets from sample file to users score multimap and tweets multimap
  string line, ID, tweetID, tweet;
  int userID;
  if (wordfileIn.is_open()) {
    while (getline(wordfileIn, line)) { //while a line exists in tweet file
      stringstream ss {};
      ss << line; //cast line to string stream
      getline(ss, ID, '\t'); //split string stream into attributes by tab delim
      stringstream oss {};
      oss << ID;
      oss >> userID;
      for (int i = 0; i < Users.size(); i++) //iterate through users vector
      {
        if (Users[i].getRegNo() == userID) //finds matching reg no with a tweet from sample file
        {
          getline(ss, tweetID, '\t'); //splits tweet and tweet ID by a tab delim
          getline(ss, tweet, '\t');
          Users[i].addScore(Users[i].getAverageScore(), tweet, false); //calls add score function in users object to add a tweet to score map
          tweets.insert({Users[i], tweet}); //inserts users tweet and users object into tweets map
          break;
        }
      }
    }
  }
}
void ReadTweets::close()
{ 
  wordfileIn.close();
  wordfileOut.close();
}
