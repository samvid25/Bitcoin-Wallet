#ifndef __UI_HELPER__
#define __UI_HELPER__

#include <ncurses.h>
#include <vector>
#include <string>

int xMin, yMin, xMax, yMax;
int termWidth, termHeight;

WINDOW *headerWin, *sideWin, *menuWin, *balanceWin, *resultWin, *resultWin2, *autoImpWin;

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

	width = 120; //0.78 * termWidth;
	height = 9;
	start_x = (termWidth - 120) / 2; //0.12 * termWidth;
	start_y = (termHeight - 9) / 2;
	
	WINDOW *titleWin = newwin(height, width, start_y, start_x);
	refresh();
	box(titleWin, 0, 0);
	wbkgd(titleWin, COLOR_PAIR(1));
	wrefresh(titleWin);


	mvprintw(start_y + 1, start_x + 1, " __          __  _                            _          __  __          _ _ _        __          __   _ _      _   _ ");
	mvprintw(start_y + 2, start_x + 1, " \\ \\        / / | |                          | |        |  \\/  |        (_) | |       \\ \\        / /  | | |    | | | |");
	mvprintw(start_y + 3, start_x + 1, "  \\ \\  /\\  / /__| | ___ ___  _ __ ___   ___  | |_ ___   | \\  / | ___ _____| | | __ _   \\ \\  /\\  / /_ _| | | ___| |_| |");
	mvprintw(start_y + 4, start_x + 1, "   \\ \\/  \\/ / _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\ | __/ _ \\  | |\\/| |/ _ \\_  / | | |/ _` |   \\ \\/  \\/ / _` | | |/ _ \\ __| |");
	mvprintw(start_y + 5, start_x + 1, "    \\  /\\  /  __/ | (_| (_) | | | | | |  __/ | || (_) | | |  | | (_) / /| | | | (_| |    \\  /\\  / (_| | | |  __/ |_|_|");
	mvprintw(start_y + 6, start_x + 1, "     \\/  \\/ \\___|_|\\___\\___/|_| |_| |_|\\___|  \\__\\___/  |_|  |_|\\___/___|_|_|_|\\__,_|     \\/  \\/ \\__,_|_|_|\\___|\\__(_)");

	attroff(COLOR_PAIR(1));

	getch();

	werase(titleWin);
	wrefresh(titleWin);
}

void displayInterface()
{
	int startPos = (termWidth - 67) / 2;
	headerWin = newwin(0.2 * termHeight, termWidth, 0, 0);
	box(headerWin, 0, 0);
	wbkgd(headerWin, COLOR_PAIR(1));
	mvwprintw(headerWin, 1, startPos, "    __  ___            _ ____         _       __      ____     __ ");
	mvwprintw(headerWin, 2, startPos, "   /  |/  /___  ____  (_) / /___ _   | |     / /___ _/ / /__  / /_");
	mvwprintw(headerWin, 3, startPos, "  / /|_/ / __ \\/_  / / / / / __ `/   | | /| / / __ `/ / / _ \\/ __/");
	mvwprintw(headerWin, 4, startPos, " / /  / / /_/ / / /_/ / / / /_/ /    | |/ |/ / /_/ / / /  __/ /_  ");
	mvwprintw(headerWin, 5, startPos, "/_/  /_/\\____/ /___/_/_/_/\\__,_/     |__/|__/\\__,_/_/_/\\___/\\__/  ");
	wrefresh(headerWin);

	sideWin = newwin(0.8 * termHeight + 1, 38, 0.2 * termHeight, termWidth - 38);
	box(sideWin, 0, 0);
	wbkgd(sideWin, COLOR_PAIR(1));
	wattron(sideWin, A_UNDERLINE);
	mvwprintw(sideWin, 2, (termWidth - 38) / 2, "Used receiving addresses");
	wattroff(sideWin, A_UNDERLINE);
	wrefresh(sideWin);

	balanceWin = newwin(5, 0.25 * termWidth, termHeight - 5, 0.75*termWidth - 36);
	box(balanceWin, 0, 0);
	wbkgd(balanceWin, COLOR_PAIR(1));
	wrefresh(balanceWin);

	wrefresh(stdscr);
}

void updateBal(int bal)
{
	std::string s = "Balance: " + std::to_string(bal) + " satoshis";
	wattron(balanceWin, A_BOLD);
	mvwprintw(balanceWin, 2, (0.25 * termWidth - s.length()) / 2, s.c_str());
	wattroff(balanceWin, A_BOLD);
	wrefresh(balanceWin);
}

void updateAddrs(std::vector<std::string> addrs)
{
	int s = addrs.size();
	for (int i = 0; i < s; i++)
	{
		mvwprintw(sideWin, 4+i, 2, addrs[i].c_str());
		wrefresh(sideWin);
	}
}

int displayMenu(std::vector<std::string> opts)
{
	int height = opts.size()+2;
	int width = termWidth/3; // div by 2
	int start_x = 1; //	0.33 * termWidth;
	int start_y = (termHeight - height) >> 1; // div by 2
	menuWin = newwin(height, width, start_y, start_x);
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

void displayResult(std::string res)
{
	werase(menuWin);
	wrefresh(menuWin);
	resultWin = newwin(3, res.length() + 4, (termHeight - 3) / 2, 1);
	box(resultWin, 0, 0);
	wbkgd(resultWin, COLOR_PAIR(1));
	mvwprintw(resultWin, 1, 2, res.c_str());
	wrefresh(resultWin);
	getch();
	werase(resultWin);
	wrefresh(resultWin);
}

int displayAutoImp(int start_y)
{
	autoImpWin = newwin(5, 71, start_y, (termWidth - 71) / 2);
	box(autoImpWin, 0, 0);
	wbkgd(autoImpWin, COLOR_PAIR(1));
	keypad(autoImpWin, true);

	mvwprintw(autoImpWin, 1, 2, "Would you like the wallet to be automatically loaded the next time?");
	std::vector<std::string> opts = {"Yes", "No"};
	int choice;
	int highlight = 0;
	int menuSize = opts.size();

	while(1)
	{
		for (int i = 0; i < menuSize; i++)
		{
			if(i == highlight)
				wattron(autoImpWin, A_REVERSE);
			int len = opts[i].length();
			mvwprintw(autoImpWin, i + 2, (71 - len) / 2, "%s", opts[i].c_str());
			wattroff(autoImpWin, A_REVERSE);
		}
		choice = wgetch(autoImpWin);
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
			break;
		}
	}

	werase(resultWin);
	werase(resultWin2);
	werase(autoImpWin);
	wrefresh(resultWin);
	wrefresh(resultWin2);
	wrefresh(autoImpWin);
	return highlight;
}

void displayResult2(std::string s1, std::string s2)
{
	werase(menuWin);
	wrefresh(menuWin);
	resultWin = newwin(4, 95 + 4, (termHeight - 3) / 2 - 2, (termWidth - (95 + 4)) / 2);
	box(resultWin, 0, 0);
	wbkgd(resultWin, COLOR_PAIR(1));
	mvwprintw(resultWin, 1, 2,s1.substr(0,95).c_str());
	mvwprintw(resultWin, 2, (99 - (s1.length() - 95)) / 2,s1.substr(95,s1.length()-1).c_str());
	
	wrefresh(resultWin);

	resultWin2 = newwin(3, s2.length() + 4, (termHeight - 3) / 2 + 2, (termWidth - (s2.length() + 4)) / 2);
	box(resultWin2, 0, 0);
	wbkgd(resultWin2, COLOR_PAIR(1));
	wattron(resultWin2, A_BOLD);
	mvwprintw(resultWin2, 1, 2, s2.c_str());
	wattroff(resultWin2, A_BOLD);
	wrefresh(resultWin2);

	getch();

	// displayAutoImp((termHeight - 3) / 2 + 5);

}

string_list enterMnemonic()
{
	werase(menuWin);
	wrefresh(menuWin);

	char mnemonic[100];
	string_list mnemonicList;
	resultWin = newwin(3, 30, (termHeight - 3) / 2 - 2, (termWidth - 28) / 2);
	box(resultWin, 0, 0);
	wbkgd(resultWin, COLOR_PAIR(1));
	mvwprintw(resultWin, 1, 2, "Enter your wallet mnemonic");
	wrefresh(resultWin);

	resultWin2 = newwin(3, 90, (termHeight - 3) / 2 + 2, (termWidth - 90) / 2);	
	keypad(resultWin2, true);
	box(resultWin2, 0, 0);
	wbkgd(resultWin2, COLOR_PAIR(1));
	wrefresh(resultWin2);	
	mvwgetstr(resultWin2, 1, 1, mnemonic);
	char *word = strtok(mnemonic, " ");
	// int i = 0;
	while(word)
	{
		mnemonicList.push_back(std::string(word));
		// mnemonicList[i++] = std::string(word);
		word = strtok(NULL, " ");
	}

	return mnemonicList;
	// displayAutoImp((termHeight - 3) / 2 + 5);

}

#endif