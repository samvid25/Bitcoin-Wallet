/**************************** Libbitcoin Libraries ***************************/
#include <bitcoin/bitcoin.hpp>
#include <bitcoin/protocol.hpp>
#include <bitcoin/client.hpp>

/****************************** Boost Libraries *****************************/
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

/***************************** General Libraries ***************************/
#include <string.h>
#include <iostream>
#include <fstream>

/********************************* Helpers *********************************/
#include "wallet.h"
#include "balance.h"

/***************************** Namespaces Used *****************************/
namespace pt = boost::property_tree;
namespace ba = boost::algorithm;

/****************************** Global Objects *****************************/
bool firstTime = true;
HD_Wallet *myWallet;

/***************************************************************************/

int menu()
{
	std::cout << std::endl;
    std::cout << "1. Display keys" << std::endl;
    std::cout << "2. Display Wallet Balance" << std::endl;
    std::cout << "3. Send bitcoin" << std::endl;
    std::cout << "4. Exit" << std::endl;
    std::cout << "Enter your option: ";
    int option;
    cin >> option;
    return option;
}

void createConfigFile(std::string mnemonic)
{
    // Set 'autoLoad' to automatically load the wallet the next time the application is loaded
    std::cout << "\nDo you want to automatically load this wallet next time? (y|n): ";
    char option;
    std::cin >> option;

    pt::ptree config;
    if (option == 'y' | option == 'Y')
        config.put("autoImport", true);
    else
        config.put("autoImport", false);        

    config.put("mnemonic", mnemonic);
    //encrypt mnemonic file
    pt::write_json("config.json", config);
}

void createWallet()
{
    data_chunk entropy = data_chunk(16);
    pseudo_random_fill(entropy);
    myWallet = new HD_Wallet(entropy);
    std::cout << "\nA new wallet has been generated. Please note down the mnemonic and do not disclose it anywhere. This can be used to recover your wallet in case you lose your private key." << std::endl;
    std::string mnemonic = myWallet->displayMnemonic();

    createConfigFile(mnemonic);
}

void importWallet(bool autoImport)
{
    string_list mnemonic;
    std::string mnemonicString;
    if (autoImport)
    {
        std::cout << "Loading your wallet..." << std::endl;
        pt::ptree config;
        pt::read_json("config.json", config);
        mnemonicString = config.get<std::string>("mnemonic");
        ba::split(mnemonic, mnemonicString, ba::is_any_of(" "));
    }
    else
    {
        std::cout << "Please enter the mnemomic: " << std::endl;
        for(int i = 0; i < 12; i++)
            std::cin >> mnemonic[i];
        if(firstTime)
            createConfigFile(join(mnemonic));
    }
    
    myWallet = new HD_Wallet(mnemonic);
}

void displayKeys()
{
    int option;
    int index;
	while(option != 5)
	{
		std::cout << std::endl;
		std::cout << "1. Receiving addresses" << std::endl;
		std::cout << "2. Master private key" << std::endl;
		std::cout << "3. Mnemonic seed" << std::endl;
		std::cout << "4. Child private keys" << std::endl;
		std::cout << "5. Back" << std::endl;
		std::cout << "Enter your option: ";
		cin >> option;		
		switch(option)
		{
			case 1: std::cout<<"\nEnter the number of addresses you wish to be displayed: ";
					std::cin>>index;
					myWallet->displayAddresses(0,index);
					break;
			case 2: myWallet->displayPrivateKey();
					break;
			case 3: myWallet->displayMnemonic();
					break;
			case 4: std::cout<<"\nEnter the index of the private key you wish to be displayed: ";
					std::cin>>index;
					myWallet->displayChildPrivateKey(index);
					break;
			case 5: return;
		}
	}
}

void create_transaction()
{
    std::cout << "Enter the destination address: ";
    std::string dest;
    getline(cin, dest);
    std::cout << "Enter amount in BTC to send: ";
    std::string BTC;
    uint64_t Satoshis;
    decode_base10(Satoshis, BTC, 8);
}

int main()
{
    std::cout << "Welcome to Mozilla Wallet!" << std::endl;

    if (!std::ifstream("config.json"))  // If the configuration file does not exist
        firstTime = true;
    else                                // Configuration file found
        firstTime = false;

    if(firstTime)
    {
        // Create a new wallet or import an existing wallet
        std::ofstream config;
        config.open("config.json");
        config.close();
        int option;
        std::cout << "\nWould you like to create a new wallet or import an existing one?";
        std::cout << "\n1. Create";
        std::cout << "\n2. Import";
        std::cout << "\nEnter your option: ";
        std::cin >> option;
        do
        {
            switch(option)
            {
                case 1:
                    createWallet();
                    break;
                case 2:
                    importWallet(0);
                    break;
                default:
                    std::cout << "Please enter a valid option." << std::endl;
            }
        } while (option != 1 && option != 2);
    }
    else
    {
        // Import existing wallet
        bool autoImport;
        pt::ptree config;
        pt::read_json("config.json", config);
        autoImport = config.get<bool>("autoImport");
        importWallet(autoImport);
        
    }

	int option;
	while((option = menu()) != 4)
	{
		switch(option)
		{
			case 1:
				displayKeys();
				break;
            case 2:
                // std::cout<<getBalance(myWallet->childAddress(0));
                // std::cout << getBalance(wallet::payment_address("2NFUMXRyjRQajiMPR1kxHrPB2TabEeRT5LZ"));
                std::cout << wallet_balance();
                break;
            case 3:
                create_transaction();
                break;
            case 4:
                exit(0);
			default:
				std::cout << "Please enter a valid option." << std::endl;
		}
	}
}

//TO DO:
// split into multiple files
// Add following to menu:
// option to create new wallet
// option to delete current wallet and import another one
// list of previous transactions