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
#include <vector>

/********************************* Helpers *********************************/
#include "../include/wallet.h"
#include "../include/balance.h"
#include "../include/utxo.h"

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
    std::cout << "1. Wallet balance" << std::endl;
    std::cout << "2. Generate new receiving address" << std::endl;
    std::cout << "3. Send bitcoin" << std::endl;
    std::cout << "4. List all used receiving addresses" << std::endl;
    std::cout << "5. Wallet recovery mnemonic" << std::endl;
    std::cout << "6. Master private key" << std::endl;
    std::cout << "7. Exit" << std::endl;
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

void create_transaction()
{
    std::cout << "Enter amount in satoshis to send: ";
    int satoshi;
    std::cin >> satoshi;
    do
    {
        std::cout << "You do not have sufficient balance. Please enter a lesser amount (in sathoshis): ";
        std::cin >> satoshi;
    } while (satoshi > getBalance(myWallet));
    std::cout<<"Enter the destination address: ";
    std::string destination;
    getline(cin, destination);

    getUTXO(myWallet, satoshi);    
    

    bc::wallet::payment_address destinationAddr(destination);
    bc::chain::script outputScript = bc::chain::script().to_pay_key_hash_pattern(destinationAddr.hash());
    bc::chain::output output1(satoshi, outputScript);
    
    // Inputs
    hash_digest utxoHash;
    bc::chain::transaction tx = bc::chain::transaction();
    data_chunk pubkey;
    bc::chain::script lockingScript;
    bc::chain::input input1;
    for(int i = 0; i < UTXOobj.size(); i++)
    {
        std::string hashString = UTXOobj[i].UTXOhash;
        int index = UTXOobj[i].index;

        decode_hash(utxoHash, hashString);
        bc::chain::output_point utxo(utxoHash, index);
        pubkey = to_chunk(myWallet->childPublicKey(index).point());
        lockingScript = bc::chain::script().to_pay_key_hash_pattern(bitcoin_short_hash(pubkey));
        input1 = bc::chain::input();
        input1.set_previous_output(utxo);
        input1.set_sequence(0xffffffff);
        tx.inputs().push_back(input1);
    }
    // std::vector<string> usedAddrs = myWallet->getUsedAddresses();
    // int usedAddrsCount = myWallet->getUsedAddressesCount();
    // std::string addrList = usedAddrs[0];
    // for(int i = 1; i < usedAddrsCount; i++)
    // {
    //     addrList += "|" + usedAddrs[i];
    // }



    // logic

    UTXOobj.clear();
}

int main()
{
    std::cout << "Welcome to Mozilla Wallet!" << std::endl;

    if (!std::ifstream("config.json"))  // If the configuration file does not exist
        firstTime = true;
    else                                // Configuration file found
        firstTime = false;

    pt::ptree config;

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
        pt::read_json("config.json", config);
        autoImport = config.get<bool>("autoImport");
        importWallet(autoImport);
        // myWallet->setUsedAddressesCount(config.get<int>("usedAddressesCount"));
    }

	int option;
	while((option = menu()) != 7)
	{
		switch(option)
		{
            case 1:
                std::cout << "Your balance is: " << getBalance(myWallet) << " satoshis." ;
                break;
            case 2:
                std::cout << "\nNew receiving address: " << myWallet->generateNewAddress() << std::endl;
                pt::read_json("config.json", config);
                config.put("usedAddressesCount", myWallet->getUsedAddressesCount());
                pt::write_json("config.json", config);
                break;
            case 3:
                create_transaction();
                break;
            case 4:
                std::cout << "\nThe following receiving addresses have been used so far:\n";
                myWallet->dispayUsedAddresses();
                break;
            case 5:
                myWallet->displayMnemonic();
                break;
            case 6:
                myWallet->displayMasterPrivateKey();
                break;
            case 7:
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
// during import, populate usedAddresses list by checking address gap + balance