# Bitcoin-Wallet
A bitcoin wallet built using the libbitcoin framework. (still in progress)
This project is aligned to mozilla campus club projects.

Team:

Samvid - NITK

Assigned Mentor for project: Manjesh P Shetty [Regional coordinator - mozilla campus clubs karnataka]

## Requirements

To build the wallet, you will need the following libraries:
* boost v1.60.0
* libbitcoin
* libbitcoin-client
* libbitcoin-explorer
* libbitcoin-protocol
* libbitcoin-network
* restclient-cpp (uses libcurl, which is installed by default on *nix systems)
* secp256k1
* zmq

## Requirements installation

### libbitcoin, boost, secp256k1 and zmq

```bash
$ git clone git@github.com:libbitcoin/libbitcoin-network.git
$ cd libbitcoin-network
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

### restclient-cpp

```bash
$ git clone git@github.com:mrtazz/restclient-cpp.git
$ cd restclient-cpp
$ ./autogen.sh
$ ./configure
$ make install
```

The above instructions will build the necessary libraries. Now run `make` to compile the wallet.
Then
```bash
$ cd bin
$ ./Mozilla_Wallet
```
to run the wallet.
