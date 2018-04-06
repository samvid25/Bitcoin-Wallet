PKGCONFIG= pkg-config
PACKAGES= libbitcoin libbitcoin-client
LIBES:= $(shell $(PKGCONFIG) --cflags $(PACKAGES) --libs $(PACKAGES))

Mozilla_Wallet: src/main.cpp include/wallet.h include/balance.h
	g++ -std=c++14 -W -w -o bin/Mozilla_Wallet src/main.cpp $(LIBES)