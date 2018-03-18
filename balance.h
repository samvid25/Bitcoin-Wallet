#ifndef __BALANCE_HELPER__
#define __BALANCE_HELPER__

#define ADDRESS_GAP 20

using namespace bc;

extern HD_Wallet *myWallet;

uint64_t balancer(const auto &rows)
{
		uint64_t unspent_balance = 0;

		for(const auto row: rows)
		{
		    // spend unconfirmed (or no spend attempted)
		    if (row.output.hash() != null_hash)
		        unspent_balance += row.value;
		}
		return unspent_balance;
}

float getBalance(wallet::payment_address address)
{
	client::connection_type connection = {};
	connection.retries = 3;
	connection.timeout_seconds = 8;
	connection.server = config::endpoint("tcp://testnet1.libbitcoin.net:19091");

	client::obelisk_client client(connection);

	static const auto on_done = [](const auto& rows)
	{
		uint64_t balance = balancer(rows);
		return atof(encode_base10(balance, 8).c_str());
		//std::cout << encode_base10(balance, 8) << std::endl;

	};
	static const auto on_error2 = [](const code ec) {

		std::cout << "Error Code: " << ec.message() << std::endl;

	};

	if(!client.connect(connection))
	{
		std::cout << "Fail" << std::endl;
	} else {
		std::cout << "Connection Succeeded" << std::endl;
	}

	client.blockchain_fetch_history3(on_error2, on_done, address);
	client.wait();


}

float wallet_balance()
{
	float balance = 0;
	for (int i = 0; i < ADDRESS_GAP; i++)
    {
        balance += getBalance(myWallet->childAddress(i));
	}
	return balance;
}

#endif