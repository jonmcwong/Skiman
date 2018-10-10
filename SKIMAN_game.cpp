#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <ostream> 
#include <vector>
#include <cstdlib>
#include <cmath>
#include <termios.h>
#include <unistd.h>
#include <limits>
#include <sstream>

using namespace std;
using namespace std::chrono; //nanoseconds
using namespace std::this_thread; //sleep_for()

//Contents:
//Section 1) Declaring Global variables
//Section 2) Declaringmiscellaneous (and independant) functions
//Section 3) Declaring (dependant) variables
//Section 4) int main()
//Section 5) Defining  declared variables

//Section 1) Declaring Global variables.........................................................................................................
//struct resolution
//struct object
//int actualwidth
//double score
//char choice
//vector <object> lastscreen
//int HIGHSCORE 
//creating Global variables

//define and object type
struct resolution
{
	int width;
	int height;
};

struct object
{
	string out;
	double x;
		
};

int actualwidth{90};

double score{0};

char choice = 'Y';

vector<object> lastscreen; //saves the latest screen
int HIGHSCORE{-1000};

//Instantiating Global variables

	object nothing = {" ", 1}; //object called nothing
	object flag = {"0                  0",}; //object called flag
	object arrows = {"^^^^^^^^^^^^^^^^^^" ,}; //object arrows
	object SKIMAN = {"|O|",}; //object called SKIMAN
	object TRAIL = {"| |",}; //for the trail behind the SKIMAN
	object TRACK = {" ", 1};

	
termios T, Default; //Termios Structure, struct termios is defined in termios' header file

resolution terminal{81,54};// resolution of terminal

	
//Section 2) Declaring and defining miscellaneous (and independant) functions (definitions at the bottom)......................................................................................
//void wait()
//void print_line()
//void buffer_on()
//void buffer_off()
//void clear_stdin()
//char print_linereturn()
//void make_introduction()

void wait(int ns); //creates of delay of ms nanoseconds

void print_line(string line); //prints line character by character

void buffer_off(); // disables buffered input

void buffer_on(); // enables raw mode

void clear_stdin(); // clears the input buffer

char print_linereturn(string line); // prints line character by character returns the answer in the form char

//Section 3) Declaring and defining (dependant) functions...............................................................................................................................
//int get_resolution()
//vector<object> refresh_screen()
//void find_newpos()
//bool print_screen()

resolution get_resolution() //returns the height of the screen NEED to return width 
{
	 
	//variable called choice
	cout << string(300, '\n') << endl;
	print_line("Before you start playing, SKIMAN needs to know the height of your screen.");
			
		
	while (choice == 'Y')
	{
		choice = print_linereturn("\nDo you know the resolution of your terminal? (Y or N)");
		while (choice != 'Y' && choice != 'N')
		{
			clear_stdin();
			choice = print_linereturn("\nSorry I didnt understand your reply, do you know the resolution of your terminal? (Y or N)");
		}
		char end = 0;
		int i{0};
		switch (choice)
		{
			case 'N':
			{
				print_line("\nOk, Let's find the resolution out then!");
				print_line("\nWhen told to GO please press ENTER...");
				print_line("\nuntil THIS line appears at the top of your screen.");
				print_line("\nThen type '1' on the last line to indicate the bottom of the screen.");
			
				cout << " GO!" << endl;
				while(end != '1')
				{	
					cin.clear();
					fflush(stdin);
					end = getchar();
					if(end == '\n')
					{
						cout << end;
						i++;
					}
				}
				print_line("\nYour screen will now be " + to_string(i + 2) + " lines high.");
				cout << string(300, '\n') << endl;
				terminal.height = i + 2;
				//now for the width
				//DO STUFF
				//DO STUFF
				print_line("Now to find out the height.");
				print_line("\nWhen told to GO, please press SPACE until you have typed a SPACE on the next line.");
				print_line("\nThen type '1' after this SPACE on this new line to indicate the bottom of the screen.");
				end = ' ';
				i = 0;
				cout << " GO!" << endl;
				while(end != '1')
				{
					cin.clear();
					fflush(stdin);
					end = getchar();
					if(end == ' ')
					{
						cout << end;
						i++;
					}
				}
				print_line("\nYour screen will now be " + to_string(i) + " columns wide.");
				terminal.width = i;
				break;
			}
			case 'Y':
			{
				buffer_on();
				string line = "\nPlease input the number of columns in your terminal, then the number number of rows:";
				for(auto i: line)
				{
					cout << i << flush;
					wait(5000000);
				}	
				cout << '\n';
				cin >> terminal.width >> terminal.height;
				buffer_off();
				break;
			}
		}
		//Ask whether user wants to repeat
		clear_stdin();
		choice = print_linereturn("\nYour resolution is now " + to_string(terminal.width) + " by " + to_string(terminal.height) + ". Would you like to set it again? (Y or N)");
		//validate answer, if it is wrong then ask for another input
		while (choice != 'Y' && choice != 'N')
		{
			clear_stdin();
			choice = print_linereturn("\nSorry I didnt understand your reply, would you like to set your resolution again? (Y or N)");
		}
		//Depending on response, program will be run again
	}
	actualwidth = terminal.width;
	if(round(terminal.height * 1.5) <= terminal.width)
	{
		terminal.width = round(terminal.height * 1.5);
	}
	return terminal;
}

vector<object> refresh_screen()//returns the new updated screen vector
{
	static int framecounter{1}, counter{2};
	static vector<object> screen(terminal.height - 1, nothing);//This vector represents the screen, it is height long and each item is nothing
	if(framecounter == 1)
	{
		screen[screen.size() - 10] = TRACK;
		if(counter != 5)//if the counter is a number other than 2...
		{
			if(counter == 2 || counter == 3 || counter == 4)
			{
				screen.insert(screen.begin(), flag);
				screen[0].x = screen[1].x;
			}
			else
			{
				if(counter == 1)//if it is 1 then replace arrows with flag
				{
					screen[0].x -= 1;
					screen[0].out = flag.out;
					
					 //TASK, do the other if statments then create print function
				}
				screen.insert(screen.begin() + 1, nothing); //shift everything down
			}		
		}
		else //if counter is 2 then insert arrows
		{
			arrows.x = rand() % (terminal.width - flag.out.length() + 1) + 2;// range from 1 to width minus 1
			screen.insert(screen.begin(), arrows);
		}
		screen.pop_back();	//after each cycle, delete the last line
		lastscreen = screen;
		if(counter <= round(terminal.height * 5 / 8))//distance between flags
		{
			counter++;
		}
		else
		{
				counter = 1;
		}
	
	}	
	if(framecounter <= 1)//vector refreshes every 5 frames, refresh rate
	{
		framecounter++;
	}
	else
	{
		framecounter = 1;
	}
	return screen;
}

void find_newpos(double &xpos, double interval) //detects what keys are being pressed and then changes the vvelocity accordingly
{
	static double velocity{0};
	const double acceleration{3000000000/interval}; //Rate of chamge of acceleration
	clear_stdin;
	switch (getchar())
	{
		case 'a':
			clear_stdin;
			velocity -= acceleration / 50;
			SKIMAN.out = "O||";
			TRAIL.out = " ||";
			break;
		case 'l':	
			clear_stdin;
			velocity += acceleration / 50;
			SKIMAN.out = "||O";
			TRAIL.out = "|| ";
			break;
		default:
			velocity *= 0.9;
			SKIMAN.out = "|O|";
			TRAIL.out = "| |";
	}
	clear_stdin;
	xpos += velocity / 50;
	if(xpos < 1)
	{
		xpos = 1;
		velocity = 0;
		SKIMAN.out = "|O|";
		TRAIL.out = "| |";
	}
	else if(xpos > terminal.width)
	{
		xpos = terminal.width;
		velocity = 0;
		SKIMAN.out = "|O|";
		TRAIL.out = "| |";
	}
}
	
bool print_screen(vector<object> screen, int xpos) //returns 1 when alive, returns 0 when dead
{
	stringstream picture, trail;
	picture << '\n';
	for(int i = 0; i < screen.size(); i++)
	{
		
		picture << string(round((actualwidth - terminal.width) / 2 ), ' ');
		if(i != screen.size() - 10)// for any line besides the SKIMANs line, just print it.
		{
				
			picture << string(screen[i].x - 1, ' ') << screen[i].out << '\n';
			//cout << to_string(screen[i].x - 1);
		}
		else if(screen[i].out == flag.out)//if on SKIMANs line, and there is a flag on that line
		{
			if(xpos <= screen[i].x - SKIMAN.out.length())// if the SKIMAN is to the left
			{
				picture << string(xpos - 1, ' ') << SKIMAN.out << string(screen[i].x - xpos - SKIMAN.out.length(), ' ') << screen[i].out << '\n';
				TRACK = {TRAIL.out + string(screen[i].x - xpos - TRAIL.out.length(), ' ') + screen[i].out, xpos};
				score -= 1;
			}
			else if(xpos >= screen[i].x + screen[i].out.length())//if the SKIMAN is after the flag
			{
				picture << string(screen[i].x - 1, ' ') << screen[i].out << string(xpos - screen[i].x - screen[i].out.length(), ' ') << SKIMAN.out << '\n';
				TRACK = {screen[i].out + string(xpos - screen[i].x - screen[i].out.length(), ' ') + TRAIL.out, screen[i].x};
				score -= 1;
			}
			else if(xpos >= screen[i].x + 1 && xpos <= screen[i].x + screen[i].out.length() - SKIMAN.out.length() - 1)//if SKIMAN is in the middle of the flag
			{
				picture << string(screen[i].x - 1, ' ') << "0" << string(xpos - screen[i].x - 1, ' ') << SKIMAN.out << string(screen[i].x + screen[i].out.length() - xpos - SKIMAN.out.length() - 1, ' ') << "0\n";  
				TRACK = {"0" + string(xpos - screen[i].x - 1, ' ') + TRAIL.out + string(screen[i].x + screen[i].out.length() - xpos - TRAIL.out.length() - 1, ' ') + "0", screen[i].x};
				score += 1;
			}
			else//if there is a collison
			{
				cout << picture.str() << "SCORE: " << score << flush;
				return 0;
			}
		}	
		else// for printing the SKIMANs line when there is nothing but SKIMAN on i
		{
			picture << string(xpos - 1, ' ') << SKIMAN.out << '\n';
			TRACK  = {TRAIL.out, xpos}	;
		}
	}
	cout << picture.str() << "SCORE: " << score << flush;
	
	return 1;
}		
	
void make_introduction()
{
	choice = print_linereturn("\nIf you would like to skip the introduction press Q, if you would like to go throught them press any other key");
	if(choice != 'Q')
	{
		print_line("\nOK, here's your beloved intro.");
		print_line("\nYou are a SKIMAN...   ");
		print_line("\nYou must go through as many flags as possible.");
		print_line("\nYou look like this:\n|O|        ");
		print_line("\nYeah I know, I had a crappy budget....      ");
		print_line("\nThe flags look like this:\n" + flag.out + "             ");
		print_line("\nYou will gain or lose a points depending on whether you go through or miss a flag.");
		print_line("\nBut you will gain even more points for going through a flag...   ");								
		print_line("\nThe aim of the game is to get as many points as possible...     ");
		print_line("\nWATCH OUT!! If you hit the pole of a flag you will lose.");
		print_line("\nUse A to move left and L to move right.");
		print_line("\nGood luck, and MAY THE ODDS BE EVER IN YOUR FAVOUR!!! Jonathan out.");
		print_line("\nI am Jonathan by the way, the maker of this game :D Nice to meet you.");
	}
	else
	{
		print_line("\nFine, it's not my fault if you dont know how to play!");
		print_line("\nI am Jonathan by the way, the maker of this game :D Nice to meet you.");
	}
		print_line("\nFor the best experience. Please minimise your computer's key repeat delay when held down");
		print_line("\nYou can do this by opening the keyboard settings menu. This will make the game easier to play");
	print_line("\nOK! Now that you you have done that, we shall start!");
}
//Section 4) int main()...............................................................................................................................

int main()
{
	tcgetattr(STDIN_FILENO, &Default);///
	T = Default;
	buffer_off();
	get_resolution();
	make_introduction();
	choice = 'Y';
	while(choice == 'Y')
	{	
		cout << "\n3 " << flush;
		wait(1000000000);
		cout << "2 " << flush;
		wait(1000000000);
		cout << "1 " << flush;
		wait(1000000000);
		cout << "START!!! " << flush;
		wait(1000000000);
		SKIMAN.x = terminal.width / 2;
		score = 0;
	
		int delay{10000000};//to be changed; DO NOT GO ABOVE 30
	
		//NEED LOOP (is still alive?)
		T.c_cc[VMIN] = 0;
		tcsetattr(STDIN_FILENO, TCSANOW , &T); //Apply the new settings
		auto t = system_clock::now();
		while(print_screen(refresh_screen(), round(SKIMAN.x)))
		{
			cin.clear();
				fflush(stdin);
			find_newpos(SKIMAN.x, delay);
			sleep_until(t + nanoseconds(delay));
			t = system_clock::now();
			delay -= 500;
			
		}
		object deadline = {string(lastscreen[lastscreen.size() - 10].x - 1, ' ') + lastscreen[lastscreen.size() - 10].out + "    ", 1};
		for(int i = 0; i <= 2; i++)
		{
			lastscreen[lastscreen.size() - 10] = deadline;
			stringstream picture;
			for(int i = 0 ; i < lastscreen.size(); i++)
			{
				picture << string(round((actualwidth - terminal.width) / 2 ), ' ');
				picture << string(lastscreen[i].x - 1, ' ') << lastscreen[i].out << '\n';			
			}
			cout << picture.str() << "SCORE: " << score << endl;
			wait(500000000);
			lastscreen[lastscreen.size() - 10].out.at(round(SKIMAN.x)) = SKIMAN.out.at(0);
			lastscreen[lastscreen.size() - 10].out.at(round(SKIMAN.x + 1)) = SKIMAN.out.at(1);
			lastscreen[lastscreen.size() - 10].out.at(round(SKIMAN.x + 2)) = SKIMAN.out.at(2);
			for(int i = 0; i < lastscreen.size(); i++)
			{
				picture << string(round((actualwidth - terminal.width) / 2 ), ' ');
				picture << string(lastscreen[i].x - 1, ' ') << lastscreen[i].out << '\n';
			}
			cout << picture.str() << "SCORE: " << score << endl;	
			wait(500000000);
		}
		if(score > HIGHSCORE) 
		{
			HIGHSCORE = score;
			print_line("\nCongratulations! Your new high score is: " + to_string(HIGHSCORE));
		}
			T.c_cc[VMIN] = 1;
		tcsetattr(STDIN_FILENO, TCSANOW , &T); //Apply the new settings
		clear_stdin();
		choice = print_linereturn("\nWould you like to play again? (Y or N)");
		while (choice != 'Y' && choice != 'N')
		{
			clear_stdin();
			choice = print_linereturn("\nSorry I didnt understand your reply, would you like to play again? (Y or N)");
		}
	}
		cout << "Bye!!" << endl;
		wait(1000000000);
		//recieve input,  change motion variables, change position
		//refresh_screen(screen);
		//print screen
		//time delay
	//Die animation. (in form of loop)
		//print screen with variations of the skiman there and not there
		//time delay
	
	//Now LOOP ENTIRE THING FOR REPLAY
	tcsetattr(STDIN_FILENO, TCSANOW, &Default);
	return 0;
}


//Section 5) Defining functions.........................................................................................................................

void wait(int ns) //creates of delay of ms nanoseconds
{
	sleep_for(nanoseconds(ns));
}

void print_line(string line) //prints line character by character
{
	line += " Press ANY KEY to continue.";
	for(int i = 0; i <= line.size() - 1; i++)
	{
		cout << line.at(i) << flush;
		wait(5000000);
	}
	char pause;
	clear_stdin();
	buffer_off();
	pause = getchar();//pause
	
}

void buffer_off() // disables buffered input
{
	T.c_lflag &= ~ICANON; //Manipulate the flag bits to do what I want it to do
    T.c_lflag &= ~ECHO;
    T.c_cc[VMIN] = 1;
	tcsetattr(STDIN_FILENO, TCSANOW , &T); //Apply the new settings
}
        
void buffer_on() // enables raw mode
{
	T.c_lflag |= ICANON; //Manipulate the flag bits to do what you want it to do
	T.c_lflag |= ECHO;
	T.c_cc[VMIN] = 10;
	tcsetattr(STDIN_FILENO, TCSANOW, &T);

}
	

void clear_stdin() // clears the input buffer
{
	T.c_cc[VMIN] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW , &T); //Apply the new settings
	for(int i = 0; i < 100; i++)
		{
			getchar();
		}
	T.c_cc[VMIN] = 10;
	tcsetattr(STDIN_FILENO, TCSANOW , &T); //Apply the new settings
	cin.clear();
	fflush(stdin);
	
}

char print_linereturn(string line) // prints line character by character returns the answer in the form char
{
	for(int i = 0; i <= line.size() - 1; i++)
	{
		cout << line.at(i) << flush;
		wait(5000000);
	}
	cout << endl;
	clear_stdin();
	char answer;
	buffer_off();
	answer = getchar();
	return answer; //TRY return getchar()
}
	
	
	//TASK, put ignore lliiness betwween  all   the Print_liineess. use limits.
	
	
	
	
	
	
	
	

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
