#ifndef __BALANCE_HELPER__
#define __BALANCE_HELPER__

#include <iostream>
#include <vector>
#include <restclient-cpp/restclient.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#define ADDRESS_GAP 20

namespace bpt = boost::property_tree;

int getBalance(HD_Wallet *myWallet)
{
	std::string addr;
	RestClient::Response response;
	std::string jsonResponse;
	long int totalBalance = 0;

	std::string queryUrl = "https://testnet.blockchain.info/balance?active=";
	std::vector<std::string> addressesUsed = myWallet->getUsedAddresses();
	int count = myWallet->getUsedAddressesCount();

	for (int i = 0; i < count; i++)
	{
		addr = addressesUsed[i];
		response = RestClient::get(queryUrl + addr);
		jsonResponse = response.body;
		std::stringstream stream(jsonResponse);
		boost::property_tree::ptree root;
		boost::property_tree::read_json(stream, root);
		totalBalance += root.get<int>(addr+".final_balance");
	}
	return totalBalance;
}

int getAddrBalance(std::string addr)
{
	RestClient::Response response;
	std::string jsonResponse;
	std::string queryUrl = "https://testnet.blockchain.info/balance?active=";
	response = RestClient::get(queryUrl + addr);
	jsonResponse = response.body;
	std::stringstream stream(jsonResponse);
	boost::property_tree::ptree root;
	boost::property_tree::read_json(stream, root);
	return root.get<int>(addr + ".final_balance");
}

#endif