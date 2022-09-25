#pragma once

#define height 25
#define width (height * 2)

void printScore(WINDOW *win,int score)
{
	mvwprintw(win,0,width/2 - 6, " SCORE : %d ",score);
}

void redraw(WINDOW *win)
{
	wclear(win);
	box(win, 0,0);
	wrefresh(win);
}

void printHomeScreen(WINDOW *win, int highscore)
{
	redraw(win);
	mvwprintw(win,height/2 - 7,width/2 - 24,R"(        _____    ____  _  __ ___   __   ___     )");
	mvwprintw(win,height/2 - 6,width/2 - 24,R"(       /_  _/   /  _/ / |/ // # \ / //7/ _/     )");
	mvwprintw(win,height/2 - 5,width/2 - 24,R"(        / /___ _\ \  /  | // _  //  ,'/ _/      )");
	mvwprintw(win,height/2 - 4,width/2 - 24,R"(       /_//__//___/ /_/|_//_//_//_/\\/___/      )");
	mvwprintw(win,height/2 - 3,width/2 - 24,R"(            __    __    __    __                )");
	mvwprintw(win,height/2 - 2,width/2 - 24,R"(           /  \  /  \  /  \  /  \               )");
	mvwprintw(win,height/2 - 1,width/2 - 24,R"(__________/  __\/  __\/  __\/  __\______________)");
	mvwprintw(win,height/2 - 0,width/2 - 24,R"(_________/  /__/  /__/  /__/  /_________________)");
	mvwprintw(win,height/2 + 1,width/2 - 24,R"(         | / \   / \   / \   / \  \____         )");
	mvwprintw(win,height/2 + 2,width/2 - 24,R"(         |/   \_/   \_/   \_/   \    o \        )");
	mvwprintw(win,height/2 + 3,width/2 - 24,R"(                                 \_____/--<     )");
	mvwprintw(win,height/2 + 5,width/2 - 7,"HIGHSCORE : %d",highscore);
	mvwprintw(win,height - 4,width/2 - 11,R"(Press any key to start)");
	wrefresh(win);
}

void printEndScreen(WINDOW *win, int highscore, int score)
{
	redraw(win);
	printScore(win,score);
	mvwprintw(win,height/2 - 5,width/2 - 10,R"(             ____  )");
	mvwprintw(win,height/2 - 4,width/2 - 10,R"(            / . .\ )");
	mvwprintw(win,height/2 - 3,width/2 - 10,R"(      GAME  \  ---<)");
	mvwprintw(win,height/2 - 2,width/2 - 10,R"(        OVER \  /  )");
	mvwprintw(win,height/2 - 1,width/2 - 10,R"(   __________/ /   )");
	mvwprintw(win,height/2 - 0,width/2 - 10,R"(-=:___________/    )");
	if(highscore < score)
	{
		highscore = score;
		mvwprintw(win,height/2 + 3,width/2 - 9,"NEW HIGHSCORE : %d",highscore);
	}
	mvwprintw(win,height - 6,width/2 - 13,R"(press 'ENTER' to play again)");
	mvwprintw(win,height - 4,width/2 - 8,R"(press 'Q' to quit)");
	wrefresh(win);
}


int getInput(WINDOW *platform)
{
	wtimeout(platform, 100);
	int direction = wgetch(platform);
	return direction;
}

WINDOW *createWin()
{
	WINDOW *win = newwin(height, width, (LINES/2 - (height/2)), (COLS/2 - height));
	box(win, 0, 0);
	keypad(win, TRUE);
	wrefresh(win);
	return win;
}

void destroy_win(WINDOW *win)
{
	wborder(win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wrefresh(win);
	delwin(win);
}
