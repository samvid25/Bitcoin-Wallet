#ifndef __MENU_HELPER__
#define __MENU_HELPER__

#include<string>
#include<vector>

std::vector<std::string> options;

void makeMainMenu()
{
	options.clear();
	options.push_back("Refresh wallet balance");
	options.push_back("Generate new receiving address");
	options.push_back("Send bitcoin");
	options.push_back("Wallet recovery mnemonic");
	options.push_back("Master private key");
	options.push_back("Exit");
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