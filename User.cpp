#include "User.h"
#include <string>
#include <sstream>
#include <iostream>
#include <math.h>
User::User(const string &name, int regNo) {
    this->name = name;
	this->regNo = regNo;
}

User::User(const string &name, int regNo, int accumulative_reputation_score, int average_reputation_score) {
    this->name = name;
	this->regNo = regNo;
    this->accumulative_reputation_score = accumulative_reputation_score;
    this->average_reputation_score = average_reputation_score;
    this->reputation_scores = {};
}

int User::getRegNo() const {
    return regNo;
}

string User::getName() const {
    return name;
}

float User::getAverageScore() {
    return average_reputation_score;
}

float User::getAccumulativeScore() {
    return accumulative_reputation_score;
}

int User::getModeratedTweets() {
    //returns count for moderated tweets
    int scoreTotal = 0;
    for (auto itr = reputation_scores.begin(); itr != reputation_scores.end(); itr++)  //iterator to iterate through tweet scores
    {
        if (itr -> second = true) //checks of moderated value is true
        {
            scoreTotal =+ 1; // +1 for every moderated tweet
        }
    }
    return scoreTotal;
}

void User::addScore(float score,string tweet, bool moderated) {
    //inserts score and tweet with moderation value to the reputation scores multimap
    reputation_scores.insert({{tweet, score}, moderated});
}

void User::updateAccumulativeScore() {
    //updates the attribute accumulative score after every read/write operation that can modify scores
    accumulative_reputation_score = 0;
    float scoreTotal = 0;
    if (reputation_scores.size() == 0)
    {
        accumulative_reputation_score = 100; //default value set to 100 if user has no tweets
    } else {
        for (auto itr = reputation_scores.begin(); itr != reputation_scores.end(); itr++)  //iterator to iterate through tweet scores map
        {
            scoreTotal += itr -> first.second; //adds every score of each tweet to total 
        }
        accumulative_reputation_score = scoreTotal;
    }
    
}

void User::updateAverageRepScore() {
    //updates the attribute average score after every read/write operation that can modify scores
    average_reputation_score = 0;
    float scoreTotal = 0;
    if (reputation_scores.size() == 0)
    {
        average_reputation_score = 100; //default value set to 100 if user has no tweets
    } else {
        for (auto itr = reputation_scores.begin(); itr != reputation_scores.end(); itr++) //iterator to iterate through tweet scores map
        {
            scoreTotal += itr -> first.second; //adds every score of each tweet to total 
        }
        average_reputation_score = scoreTotal / reputation_scores.size(); //average is total divided by how many tweets
        round(average_reputation_score); //average rounded to the nearest whole number
    }
}

void User::reportBlockedContent(string tweet) {
    //deducts scores based on blocked content in a tweet
    int score;
    bool moderated;
    for (auto itr = reputation_scores.begin(); itr != reputation_scores.end(); itr++) //iterator to iterate through users tweet scores
    {
        if (itr -> first.first == tweet && itr -> first.second > 0) //if tweet value from map is equal to reported tweet
        {
            moderated = itr -> second;
            score = itr -> first.second;
            score -= 1; //score deducted by 1
            reputation_scores.insert({{tweet, score}, true}); //insert new unique item as replacement
            reputation_scores.erase(itr); //erases old item with old score
            break;
        }
        
    }
}

void User::reportTweetScore(string tweet) {
    //deducts scores based on user reporting tweet
    int score;
    bool moderated;
    for (auto itr = reputation_scores.begin(); itr != reputation_scores.end(); itr++)  //iterator to iterate through users tweet scores
    {
        if (itr -> first.first == tweet && itr -> first.second > 0) //if tweet value from map is equal to reported tweet
        {
            moderated = itr -> second;
            score = itr -> first.second;
            score -= 1;  //score deducted by 1
            reputation_scores.insert({{tweet, score}, moderated}); //insert new unique item as replacement
            reputation_scores.erase(itr); //erases old item with old score
            break;
        }
        
    }
}

bool User::operator <( const User &rhs ) const
{
    return ( C_ID < rhs.C_ID ); 
}

ostream& operator<<(ostream &str,const User &s) { //friend overloaded << operator function
//outputs score details of user
    str << endl << "Score Per Tweet - " << s.reputation_scores.size();
    for (auto itr = s.reputation_scores.begin(); itr != s.reputation_scores.end(); itr++) {
        str << endl << "Score:" <<itr->first.second << '\t' << "Tweet:" << itr->first.first;
    }
    if (s.reputation_scores.size() == 0)
    {
        str << endl << "You have no tweets";
    }
    
    str << endl << "Average Score: " << s.average_reputation_score << endl << "Accumulative Score: " << s.accumulative_reputation_score << endl;
    return str;
}