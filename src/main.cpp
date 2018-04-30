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
#include "../include/menu.h"
#include "../include/ui.h"

/***************************** Namespaces Used *****************************/
namespace pt = boost::property_tree;
namespace ba = boost::algorithm;
using namespace bc;
using namespace wallet;
using namespace chain;
using namespace machine;

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
    // std::cout << "\nDo you want to automatically load this wallet next time? (y|n): ";
    // char option;
    // std::cin >> option;

    int option = displayAutoImp((termHeight - 3) / 2 + 5);

    pt::ptree config;
    if (option == 0)
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
    // std::cout << "\nA new wallet has been generated. Please note down the mnemonic and do not disclose it anywhere. This can be used to recover your wallet in case you lose your private key." << std::endl;
    std::string mnemonic = myWallet->displayMnemonic();

    displayResult2("A new wallet has been generated. Please note down the mnemonic and do not disclose it anywhere. This can be used to recover your wallet in case you lose your private key.", mnemonic);
    
    createConfigFile(mnemonic);
}

void importWallet(bool autoImport)
{
    string_list mnemonic;
    std::string mnemonicString;
    if (autoImport)
    {
        // std::cout << "Loading your wallet..." << std::endl;
        pt::ptree config;
        pt::read_json("config.json", config);
        mnemonicString = config.get<std::string>("mnemonic");
        ba::split(mnemonic, mnemonicString, ba::is_any_of(" "));
    }
    else
    {
        // std::cout << "Please enter the mnemomic: " << std::endl;
        // for(int i = 0; i < 12; i++)
        //     std::cin >> mnemonic[i];
        mnemonic = enterMnemonic();
        if(firstTime)
            createConfigFile(join(mnemonic));
    }
    
    myWallet = new HD_Wallet(mnemonic);
}

void create_transaction()
{
    myWallet->setBalance(getBalance(myWallet));
    std::cout << "Enter amount in satoshis to send: ";
    std::string satoshi;
    std::cin >> satoshi;
    while (stoi(satoshi) > myWallet->getBalance())
    {
        std::cout << "You do not have sufficient balance. Please enter a lesser amount (in sathoshis): ";
        std::cin >> satoshi;
    }
    std::cout<<"Enter the destination address: ";
    std::string destination;
    cin >> destination;

    getUTXO(myWallet, stoi(satoshi));

    bc::chain::transaction tx = bc::chain::transaction();
    uint32_t version = 1u;
    tx.set_version(version);
    
    // Inputs
    hash_digest utxoHash;
    data_chunk pubkey;
    bc::chain::script lockingScript;
    bc::chain::input input1[UTXOobj.size()];
    int index;
    int OPindex;
    for(int i = 0; i < UTXOobj.size(); i++)
    {
        std::string hashString = UTXOobj[i].UTXOhash;
        OPindex = UTXOobj[i].OPindex;
        index = UTXOobj[i].index;

        decode_hash(utxoHash, hashString);
        bc::chain::output_point utxo(utxoHash, OPindex);
        // pubkey = to_chunk(myWallet->childPublicKey(index).point());
        // lockingScript = bc::chain::script().to_pay_key_hash_pattern(bitcoin_short_hash(pubkey));
        input1[i] = bc::chain::input();
        input1[i].set_previous_output(utxo);
        input1[i].set_sequence(0xffffffff);
        tx.inputs().push_back(input1[i]);
    }
    // std::vector<std::string> usedAddrs = myWallet->getUsedAddresses();
    // int usedAddrsCount = myWallet->getUsedAddressesCount();
    // std::string addrList = usedAddrs[0];
    // for(int i = 1; i < usedAddrsCount; i++)
    // {
    //     addrList += "|" + usedAddrs[i];
    // }


    // Outputs
    bc::wallet::payment_address destinationAddr(destination);
    bc::chain::script outputScript = bc::chain::script().to_pay_key_hash_pattern(destinationAddr.hash());
    uint64_t satoshi_amount;
    decode_base10(satoshi_amount, satoshi, 8); // btc_decimal_places = 8
    bc::chain::output output1(satoshi_amount, outputScript);
    tx.outputs().push_back(output1);


	//make Sig Script
	bc::machine::operation::list sigScript;
    sigScript.push_back(operation(opcode::dup));
    sigScript.push_back(operation(opcode::hash160));
    operation op_pubkey = operation(to_chunk(destinationAddr.hash()));
    sigScript.push_back(op_pubkey);
    sigScript.push_back(operation(opcode::equalverify));
    sigScript.push_back(operation(opcode::checksig));

    char array[200];
    strcpy(array, myWallet->childPrivateKey(UTXOobj[0].index).encoded().c_str());
    auto my_secret = base16_literal("3eec08386d08321cd7143859e9bf4d6f65a71d24f37536d76b4224fdea48009f");
    ec_private my_private0(my_secret, ec_private::testnet, true);
    ec_compressed pubkey0= my_private0.to_public().point();
    payment_address my_address0 = my_private0.to_payment_address();

    endorsement sig;
    script prev_script = script::to_pay_key_hash_pattern(my_address0.hash());
    uint8_t input0_index(0u);
	script::create_endorsement(sig, my_secret, prev_script, tx,input0_index, 0x01);

    operation::list sig_script_0;
    sig_script_0.push_back(operation(sig));
    sig_script_0.push_back(operation(to_chunk(pubkey0)));
    script my_input_script_0(sig_script_0);

    tx.inputs()[0].set_script(my_input_script_0);

	// sigScript.push_back(bc::machine::operation(sig));

	// sigScript.push_back(bc::machine::operation(pubkey));
	// bc::chain::script unlockingScript(sigScript);
	// std::cout << unlockingScript.to_string(0xffffffff) << "\n" << std:: endl;

	//Make Signed TX
	// tx.inputs()[0].set_script(unlockingScript);
	std::cout << "Raw Transaction: " << std::endl;
    std::cout << encode_base16(tx.to_data()) << std::endl;

    // logic

    UTXOobj.clear();
}

int main()
{
    initUI();
    displayTitle();

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

        makeCreateImportMenu();
        int option = displayMenu(options);
        switch(option)
        {
            case 1:
                createWallet();
                break;
            case 2:
                importWallet(0);
                break;
        }
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


    displayInterface();
    updateBal(getBalance(myWallet));
    while (1)
    {
        makeMainMenu();
        int option = displayMenu(options);
        switch(option)
		{
            case 1:
                // std::cout << "Your balance is: " << getBalance(myWallet) << " satoshis." << std::endl;
                updateBal(getBalance(myWallet));
                break;
            case 2:
                // std::cout << "\nNew receiving address: " << myWallet->generateNewAddress() << std::endl;
                displayResult("New receiving address: " + myWallet->generateNewAddress());
                updateAddrs(myWallet->getUsedAddresses());
                pt::read_json("config.json", config);
                config.put("usedAddressesCount", myWallet->getUsedAddressesCount());
                pt::write_json("config.json", config);
                break;
            case 3:
                displayResult("Feature not completed yet");
                // create_transaction();
                break;
            case 4:
                displayResult(myWallet->displayMnemonic());
                break;
            case 5:
                displayResult(myWallet->displayMasterPrivateKey());
                break;
            case 6:
                endUI();
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