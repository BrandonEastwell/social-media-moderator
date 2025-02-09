// include your own comments

#ifndef _USER_H_
#define _USER_H_

#include <string>
#include <map>
#include <stdexcept>
#include "Person.h"

using namespace std;

class NoScoreException: public exception
{
};

class User: public Person
{   public:
        // constructor should initialise name and registration number using arguments
        // and initialise score map to be empty
        User(const string &name, int regNo);

		User(const string &name, int regNo, int accumulative_reputation_score, int average_reputation_score);


	   // method to return registration number
	   int getRegNo() const;

		// method to return name attribute
	   string getName() const;

		// method to return average score attribute
	   float getAverageScore();

		// method to return accumulative score attribute
	   float getAccumulativeScore();

		// method to return count of moderated tweets
	   int getModeratedTweets();

	   // method to add the score to the map
	   void addScore(float score,string tweet, bool moderated);

	   // method to update the accumulative score
	   void updateAccumulativeScore();

		//method to update average score attribute
	   void updateAverageRepScore();

		//method to deduct score from tweet based on report
	   void reportTweetScore(string tweet);

		//method to deduct score from tweet based on blocked content
	   void reportBlockedContent(string tweet);

	    bool operator <( const User &rhs ) const;

    	// friend function to output details of user to stream
    	// output to be defined and formated by you
    	// if the user has no scores "has no scores" should be output instead of the scores
		friend ostream& operator<<(ostream &str, const User &s);

    private:
	   int regNo;
	   string name;
	   int accumulative_reputation_score; 
	   multimap<pair<string,int>,bool> reputation_scores; //
	   int average_reputation_score; 
	   int C_ID;
};

#endif
