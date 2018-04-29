#ifndef __UI_HELPER__
#define __UI_HELPER__

#include <ncurses.h>
#include <vector>
#include <string>

int xMin, yMin, xMax, yMax;
int termWidth, termHeight;

void initUI()
{
	// ncurses functions to create and display UI
	initscr();
	cbreak();
	start_color();
	init_pair(1, COLOR_WHITE, 236);

	getbegyx(stdscr, yMin, xMin);
	getmaxyx(stdscr, yMax, xMax);
	termWidth = xMax - xMin;
	termHeight = yMax - yMin;

	box(stdscr, 0, 0);
	wbkgd(stdscr, COLOR_PAIR(1));
	wrefresh(stdscr);
}

void endUI()
{
	getch();
	endwin();
}

void displayTitle()
{


	int height, width, start_y, start_x;

	width = 0.78 * termWidth;
	height = 9;
	start_x = 0.12 * termWidth;
	start_y = (termHeight - 9) / 2;
	
	WINDOW *titleWin = newwin(height, width, start_y, start_x);
	refresh();
	box(titleWin, 0, 0);
	wbkgd(titleWin, COLOR_PAIR(1));
	wrefresh(titleWin);


	mvprintw(start_y + 1, start_x + 2, " __          __  _                            _          __  __          _ _ _        __          __   _ _      _   _ ");
	mvprintw(start_y + 2, start_x + 2, " \\ \\        / / | |                          | |        |  \\/  |        (_) | |       \\ \\        / /  | | |    | | | |");
	mvprintw(start_y + 3, start_x + 2, "  \\ \\  /\\  / /__| | ___ ___  _ __ ___   ___  | |_ ___   | \\  / | ___ _____| | | __ _   \\ \\  /\\  / /_ _| | | ___| |_| |");
	mvprintw(start_y + 4, start_x + 2, "   \\ \\/  \\/ / _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\ | __/ _ \\  | |\\/| |/ _ \\_  / | | |/ _` |   \\ \\/  \\/ / _` | | |/ _ \\ __| |");
	mvprintw(start_y + 5, start_x + 2, "    \\  /\\  /  __/ | (_| (_) | | | | | |  __/ | || (_) | | |  | | (_) / /| | | | (_| |    \\  /\\  / (_| | | |  __/ |_|_|");
	mvprintw(start_y + 6, start_x + 2, "     \\/  \\/ \\___|_|\\___\\___/|_| |_| |_|\\___|  \\__\\___/  |_|  |_|\\___/___|_|_|_|\\__,_|     \\/  \\/ \\__,_|_|_|\\___|\\__(_)");

	attroff(COLOR_PAIR(1));

	getch();

	werase(titleWin);
	wrefresh(titleWin);
	// werase();
	// wrefresh(stdscr);
}

int displayMenu(std::vector<std::string> opts)
{
	int height = opts.size()+2;
	int width = termWidth >> 1; // div by 2
	int start_x = 0.25 * termWidth;
	int start_y = (termHeight - height) >> 1; // div by 2
	WINDOW *menuWin = newwin(height, width, start_y, start_x);
	box(menuWin, 0, 0);
	wbkgd(menuWin, COLOR_PAIR(1));
	wrefresh(menuWin);
	keypad(menuWin, true);

	int choice;
	int highlight = 0;
	int menuSize = opts.size() - 1;

	while(1)
	{
		for (int i = 0; i < menuSize+1; i++)
		{
			if(i == highlight)
				wattron(menuWin, A_REVERSE);
			int len = opts[i].length();
			mvwprintw(menuWin, i + 1, (width - len) / 2, "%s", opts[i].c_str());
			wattroff(menuWin, A_REVERSE);
		}
		choice = wgetch(menuWin);
		switch(choice)
		{
			case KEY_UP:
				highlight--;
				if(highlight == -1)
					highlight = 0;
				break;
			case KEY_DOWN:
				highlight++;
				if (highlight == menuSize)
					highlight = menuSize - 1;
				break;
			default:
				break;
		}
		if(choice == 10)
		{
				return highlight+1;
		}
	}
}

	// int displayMenu(vector<string> options)
	// {
	// 	int height = 6;
	// 	int width = 0.34 * termWidth;
	// 	int start_x = 0.33 * termWidth;
	// 	int start_y = 0.5 * termHeight;
	// 	WINDOW *menuWin = newwin(height, width, start_y, start_x);
	// 	box(menuWin, 0, 0);
	// 	wbkgd(menuWin, COLOR_PAIR(1));
	// 	wrefresh(menuWin);
	// }

#endif