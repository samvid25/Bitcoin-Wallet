PKGCONFIG= pkg-config
PACKAGES= libbitcoin libbitcoin-client
LIBES:= $(shell $(PKGCONFIG) --cflags $(PACKAGES) --libs $(PACKAGES))

Mozilla_Wallet: main.cpp wallet.h balance.h
	g++ -std=c++14 -w -o Mozilla_Wallet main.cpp $(LIBES)