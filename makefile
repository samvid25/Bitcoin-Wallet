PKGCONFIG= pkg-config
PACKAGES= libbitcoin
LIBES:= $(shell $(PKGCONFIG) --cflags $(PACKAGES) --libs $(PACKAGES))

Mozilla_Wallet: main.cpp wallet.cpp
	g++ -std=c++11 -w -o Mozilla_Wallet wallet.cpp main.cpp $(LIBES)