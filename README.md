# Bitcoin-Wallet
A bitcoin wallet built using the libbitcoin framework. (still in progress)
This project is aligned to mozilla campus club projects.
Team:
Samvid - NITK 
Assigned Mentor for project: Manjesh P Shetty [Regional coordinator - mozilla]  
## Requirements

To build the wallet, you will need the following libraries:
* libbitcoin
* libbitcoin-client
* libbitcoin-protocol
* boost v1.60.0
* secp256k1
* zmq

## Requirements installation

```bash
$ git clone git@github.com:libbitcoin/libbitcoin-client.git
$ cd libbitcoin-client
```

Then open the install.sh file in your favourite code editor, and change the following line 
`BOOST_URL="http://downloads.sourceforge.net/project/boost/boost/1.57.0/boost_1_57_0.tar.bz2"
BOOST_ARCHIVE="boost_1_57_0.tar.bz2"`   
to   
`BOOST_URL="http://downloads.sourceforge.net/project/boost/boost/1.60.0/boost_1_60_0.tar.bz2"
BOOST_ARCHIVE="boost_1_60_0.tar.bz2"`

Then run the installation script:
```bash
$ sudo chmod +x install.sh
$ ./install.sh --build-boost
```

The above instructions will build the necessary libraries. Now run `make` to compile the wallet.
Then
```bash
./Mozilla_Wallet
```
to run the wallet.
