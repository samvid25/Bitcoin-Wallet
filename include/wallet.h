#ifndef __WALLET_HELPER__
#define __WALLET_HELPER__

using namespace bc;

class HD_Wallet
{
  private:
    data_chunk entropy;
    data_chunk seed;
    wallet::word_list mnemonic;
    wallet::hd_private privateKey;
    wallet::hd_public publicKey;
    std::vector<std::string> usedAddresses;
    int usedAddressesCount;
    long int balance;

  public:
    HD_Wallet()
    {
        usedAddressesCount = 0;
        balance = 0;
    }

    HD_Wallet(const HD_Wallet *wallet)
    {
        entropy = wallet->entropy;
        seed = wallet->seed;
        mnemonic = wallet->mnemonic;
        privateKey = wallet->privateKey;
        publicKey = wallet->publicKey;
        usedAddressesCount = wallet->usedAddressesCount;
        balance = wallet->balance;
    }

    HD_Wallet(const data_chunk Userentropy)
    {
        entropy = Userentropy;
        mnemonic = wallet::create_mnemonic(entropy);
        seed = to_chunk(wallet::decode_mnemonic(mnemonic));
        privateKey = wallet::hd_private(seed, wallet::hd_private);
        publicKey = privateKey.to_public();
        usedAddressesCount = 0;
        balance = 0;
    }

    HD_Wallet(const wallet::word_list mnemonicSeed)
    {
        seed = to_chunk(wallet::decode_mnemonic(mnemonicSeed));
        mnemonic = mnemonicSeed;
        privateKey = wallet::hd_private(seed, wallet::hd_private);
        publicKey = privateKey.to_public();
        usedAddressesCount = 0;
        balance = 0;
    }

    // Returns a private key derived from the master private key
    wallet::hd_private childPrivateKey(int index)
    {
        return privateKey.derive_private(index);
    }

    // Returns a public key derived from the master public key
    wallet::hd_public childPublicKey(int index)
    {
        return publicKey.derive_public(index);
    }

    wallet::payment_address childAddress(int index)
    {
        return wallet::payment_address(wallet::ec_public(childPublicKey(index).point()).to_payment_address(), 0x6f);
    }

    // Display the master private key as a string
    std::string displayMasterPrivateKey()
    {
        // std::cout << "\nPrivate Key:\n" << privateKey.encoded() << std::endl;
        return privateKey.encoded();
    }

    // Display the master public key as a string
    std::string displayMasterPublicKey()
    {
        // std::cout << "\nPrivate Key:\n" << publicKey.encoded() << std::endl;
        return publicKey.encoded();
    }

    // Display the child private key as a string
    void displayChildPrivateKey(int index)
    {
        std::cout << "\nChild Key " << index+1 << ":\n" << childPrivateKey(index).encoded() << std::endl;
    }

        // Display the child public key as a string
    void displayChildPublicKey(int index)
    {
        std::cout << "\nChild Key " << index+1 << ":\n" << childPublicKey(index).encoded() << std::endl;
    }

    void dispayUsedAddresses()
    {
        for (int i = 0; i < usedAddressesCount; i++)
            std::cout << usedAddresses[i] << std::endl;
    }

    std::string generateNewAddress()
    {
        usedAddresses.push_back(childAddress(usedAddressesCount).encoded());
        usedAddressesCount++;
        return usedAddresses[usedAddressesCount - 1];
    }

    int getUsedAddressesCount()
    {
        return usedAddressesCount;
    }

    long int getBalance()
    {
        return balance;
    }

    void setBalance(long int bal)
    {
        balance = bal;
    }

    std::string displayMnemonic()
    {
        if (wallet::validate_mnemonic(mnemonic))
        {
            std::string mnemonicString = join(mnemonic);
            // std::cout << "\nYour wallet's mnemonic is: \n" << mnemonicString << std::endl;
            return mnemonicString;
        }
        else
        {
            std::cout << "Mnemonic invalid!" << std::endl;
            return std::string();
        }
    }

    std::vector<std::string> getUsedAddresses()
    {
        return usedAddresses;
    }

    void addUsedAddress(std::string addr)
    {
        usedAddresses.push_back(addr);
    }

    void setUsedAddressesCount(int count)
    {
        usedAddressesCount = count;
    }
};

#endif