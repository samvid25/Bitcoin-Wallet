#ifndef __UTXO_HELPER__
#define __UTXO_HELPER__

#include <iostream>
#include <vector>
#include <restclient-cpp/restclient.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace bpt = boost::property_tree;

class UTXOvalues
{
	public:
	std::string UTXOhash;
	int unspentAmount;
	int index;
	int balance;
	UTXOvalues()
	{

	}
	UTXOvalues(std::string hash, int unsp, int i, int bal)
	{
		UTXOhash = hash;
		unspentAmount = unsp;
		index = i;
		balance = bal;
	}
};

std::vector<UTXOvalues> UTXOobj;

void getUTXO(HD_Wallet* myWallet, int amt)
{
	std::string queryURL = "https://testnet.blockchain.info/unspent?active=";	
	int UTXOamount;
	int usedAddressesCount = myWallet->getUsedAddressesCount();
	std::vector<std::string> usedAddresses = myWallet->getUsedAddresses();
	std::string addr;
	int bal = 0;
	RestClient::Response response;
	std::string jsonResponse;
	std::vector< std::pair<std::string, int> > UTXOs;
	for (int i = 0; i < usedAddressesCount; i++)
	{
		bal = getAddrBalance(usedAddresses[i]);
		response = RestClient::get(queryURL + usedAddresses[i]);
		jsonResponse = response.body;
		std::stringstream stream(jsonResponse);
		bpt::ptree root;
		bpt::read_json(stream, root);
		// root = root.get_child("unspent_outputs");
		for(auto &utxo : root.get_child("unspent_outputs"))
		{
			auto &rhs = utxo.second;
			int utxoamt = rhs.get<int>("value");
			if (utxoamt < amt)
			{
				UTXOobj.push_back(UTXOvalues(rhs.get<std::string>("tx_hash"), utxoamt, i, 0));
				amt -= utxoamt;
			}
			else if(amt > 0 && utxoamt > amt)
			{
				UTXOobj.push_back(UTXOvalues(rhs.get<std::string>("tx_hash"), utxoamt, i, utxoamt-amt));
				return;
			}
		}
	}
}

#endif