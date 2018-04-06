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

  public:
    HD_Wallet()
    {
    }

    HD_Wallet(const HD_Wallet *wallet)
    {
        entropy = wallet->entropy;
        seed = wallet->seed;
        mnemonic = wallet->mnemonic;
        privateKey = wallet->privateKey;
        publicKey = wallet->publicKey;
    }

    HD_Wallet(const data_chunk Userentropy)
    {
        entropy = Userentropy;
        mnemonic = wallet::create_mnemonic(entropy);
        seed = to_chunk(wallet::decode_mnemonic(mnemonic));
        privateKey = wallet::hd_private(seed, wallet::hd_private::testnet);
        publicKey = privateKey.to_public();
    }

    HD_Wallet(const wallet::word_list mnemonicSeed)
    {
        seed = to_chunk(wallet::decode_mnemonic(mnemonicSeed));
        mnemonic = mnemonicSeed;
        privateKey = wallet::hd_private(seed, wallet::hd_private::testnet);
        publicKey = privateKey.to_public();
    }

    wallet::hd_private childPrivateKey(int index)
    {
        return privateKey.derive_private(index);
    }

    wallet::hd_public childPublicKey(int index)
    {
        return publicKey.derive_public(index);
    }

    wallet::payment_address childAddress(int index)
    {
        return wallet::payment_address(wallet::ec_public(childPublicKey(index).point()).to_payment_address(), 0x6f);
    }

    void displayPrivateKey()
    {
        std::cout << "\nPrivate Key:\n" << privateKey.encoded() << std::endl;
    }

    void displayChildPrivateKey(int index)
    {
        std::cout << "\nChild Key " << index+1 << ":\n" << childPrivateKey(index).encoded() << std::endl;
    }

    void displayAddress(int index)
    {
        std::cout << "\nAddress "<< index+1 << ":\n" << childAddress(index).encoded() << std::endl;
    }

    void displayAddresses(int start, int end)
    {
        while (start != end)
        {
            displayAddress(start);
            start++;
        }
    }

    std::string displayMnemonic()
    {
        if (wallet::validate_mnemonic(mnemonic))
        {
            std::string mnemonicString = join(mnemonic);
            std::cout << "\nYour wallet's mnemonic is: \n" << mnemonicString << std::endl;
            return mnemonicString;
        }
        else
        {
            std::cout << "Mnemonic invalid!" << std::endl;
            return std::string();
        }
    }

};

#endif