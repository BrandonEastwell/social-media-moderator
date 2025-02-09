CE221-A2-SOCIALMOD: main.o Manager.o Person.o ReadTweets.o User.o
	g++ main.o Manager.o Person.o ReadTweets.o User.o -o CE221-A2-SOCIALMOD

main.o: main.cpp
	g++ -c main.cpp

Manager.o: Manager.cpp
	g++ -c Manager.cpp

Person.o: Person.cpp
	g++ -c Person.cpp

ReadTweets.o: ReadTweets.cpp
	g++ -c ReadTweets.cpp

User.o: User.cpp
	g++ -c User.cpp

clean:
	rm *.o math