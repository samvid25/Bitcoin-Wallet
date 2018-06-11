#ifndef __MENU_HELPER__
#define __MENU_HELPER__

#include<string>
#include<vector>

std::vector<std::string> options;

void makeMainMenu()
{
	options.clear();
	options.push_back("1.Refresh wallet balan");
	options.push_back("2.Generate new receiving address");
	options.push_back("3.Send bitcoin");
	options.push_back("4.Wallet recovery mnemonic");
	options.push_back("5.Master private key");
	options.push_back("6.Exit");
	options.push_back("Select your option.");
}

void makeCreateImportMenu()
{
	options.clear();
	options.push_back("Create a new wallet");
	options.push_back("Import an existing wallet");
	options.push_back("Select your option.");
}

#endif
