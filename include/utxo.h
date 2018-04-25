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
	int OPindex;
	UTXOvalues()
	{

	}
	UTXOvalues(std::string hash, int unsp, int i, int bal, int op)
	{
		UTXOhash = hash;
		unspentAmount = unsp;
		index = i;
		balance = bal;
		OPindex = op;
	}
};

std::vector<UTXOvalues> UTXOobj;

void getUTXO(HD_Wallet* myWallet, int amt)
{	
	int UTXOamount;
	int usedAddressesCount = myWallet->getUsedAddressesCount();
	std::vector<std::string> usedAddresses = myWallet->getUsedAddresses();
	std::string addr;
	int bal = 0;

	std::string queryURL = "https://testnet.blockchain.info/unspent?active=";
	RestClient::Response response;
	std::string jsonResponse;
	bpt::ptree root;

	for (int i = 0; i < usedAddressesCount; i++)
	{
		bal = getAddrBalance(usedAddresses[i]);
		response = RestClient::get(queryURL + usedAddresses[i]);
		jsonResponse = response.body;
		std::stringstream stream(jsonResponse);
		bpt::read_json(stream, root);
		
		for(auto &utxo : root.get_child("unspent_outputs"))
		{
			auto &rhs = utxo.second;
			int utxoamt = rhs.get<int>("value");
			if (utxoamt < amt)
			{
				UTXOobj.push_back(UTXOvalues(rhs.get<std::string>("tx_hash"), utxoamt, i, 0, rhs.get<int>("tx_output_n")));
				amt -= utxoamt;
			}
			else if(amt > 0 && utxoamt > amt)
			{
				UTXOobj.push_back(UTXOvalues(rhs.get<std::string>("tx_hash"), utxoamt, i, utxoamt-amt, rhs.get<int>("tx_output_n")));
				return;
			}
		}
	}
}

#endif