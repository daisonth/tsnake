#include <ncurses.h>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <deque>
#include <random>
#include <algorithm>
#include "tsnake.hpp"

#define height 25
#define width (height * 2)

int main(int argc, char *argv[])
{
	std::random_device rd;
	std::uniform_int_distribution<int> randX(1,width-3);
	std::uniform_int_distribution<int> randY(1,height-3);

	initscr();
	curs_set(0); //hide terminal cursor
	cbreak(); // read individual keY hits rather than complete lines of text
	noecho(); // stop echoing keYboard inputs
	refresh(); // refresh stdscr

	WINDOW *board = createWin();

	int input;
	int highscore = 0;

	do
	{
		int X = width/2, Y = height/2;
		int tailY = Y, tailX = X;
		int appleX = randX(rd), appleY = randY(rd);
		int score = 0;
		int nextdireciton = -1, direction = KEY_UP;
		char body = 'o', apple = 'A';

		std::vector<char> snake(10,'X');
		std::deque<int> moves;
		std::vector<std::pair<int,int>> positons;

		printHomeScreen(board, highscore);

		wgetch(board);

		redraw(board);

		printScore(board, score);

		// print snake
		for(char c: snake)
		{
			mvwaddch(board, tailY++, tailX, body);
			positons.push_back(std::make_pair(tailX,tailY));
		}
		wrefresh(board);

		// print the first apple
		mvwaddch(board,appleY,appleX,apple);

		for(int i=0; i<snake.size(); i++) moves.push_back(KEY_UP); //record initial moves for tail end

		auto pos = std::find(positons.begin(), positons.end(), std::make_pair(tailX,tailY));
		auto it = std::find(positons.begin(), positons.end(), std::make_pair(appleX,appleY));
		mvwaddch(board,tailY,tailX,' ');

		while(Y < height-1 && Y > 0 && X < width-1 && X > 0 )  // main game loop
		{
			// print new head positon
			mvwaddch(board,Y,X,body);
			positons.push_back(std::make_pair(X,Y));
			wrefresh(board);

			// wait and take input
			do {
				wtimeout(board, 200);
				nextdireciton = wgetch(board);
			} while(nextdireciton != KEY_UP && nextdireciton != KEY_DOWN && nextdireciton != KEY_LEFT && nextdireciton != KEY_RIGHT && nextdireciton != -1);

			// if there is change in direction
			if(nextdireciton  != -1)
			{
				if(
				    (direction == KEY_RIGHT && nextdireciton != KEY_LEFT) || \
				    (direction == KEY_LEFT && nextdireciton != KEY_RIGHT) || \
				    (direction == KEY_DOWN && nextdireciton != KEY_UP) || \
				    (direction == KEY_UP && nextdireciton != KEY_DOWN)
				)
					direction = nextdireciton;
			}

			// updtae next head direction from input
			if(direction == KEY_UP) --Y;
			else if(direction == KEY_DOWN) ++Y;
			else if(direction == KEY_LEFT) --X;
			else if(direction == KEY_RIGHT) ++X;

			// if the snake eat the apple
			if(Y == appleY && X == appleX)
			{
				// increase score bY 5 and print it
				score+=5;
				printScore(board, score);

				// print new apple in another random location
				do {
					appleY = randY(rd);
					appleX = randX(rd);
					mvwaddch(board,appleY,appleX,apple);
					auto it = std::find(positons.begin(), positons.end(), std::make_pair(appleX,appleY));
				} while(it == positons.end()); // new apple location must note be inside the snake body

				moves.push_back(direction); //record the next move
			}
			else
			{
				moves.push_back(direction); //record the next move

				// update tail end
				if(moves.front() == KEY_UP) --tailY;
				else if(moves.front() == KEY_DOWN) ++tailY;
				else if(moves.front() == KEY_LEFT) --tailX;
				else if(moves.front() == KEY_RIGHT) ++tailX;

				// remove the current tail end piece
				moves.pop_front();
				positons.erase(positons.begin());
				mvwaddch(board,tailY,tailX,' ');
			}

			// check if the snake bite itself
			auto pos = std::find(positons.begin(), positons.end(), std::make_pair(X,Y));
			if(pos != positons.end()) break;
		}

		if(highscore < score) highscore = score;
		printEndScreen(board, highscore, score);

		// take input
		do {
			wtimeout(board,-1);
			input = wgetch(board);
		} while(input != 10 && input != 113); // input must be either 'ENTER' or 'Q';

	} while(input == 10);

	destroy_win(board);

	endwin();
	return 0;
}
