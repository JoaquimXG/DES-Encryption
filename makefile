#TODO Separate old dependancies from new and remove old rules
SHELL := bash
.SHELLFLAGS := -eu -o pipefail -c  
.DELETE_ON_ERROR:

# Header directory
IDIR = include

# Compiler and flags
CC=g++
CFLAGS=-I$(IDIR)

# Object output directory
ODIR=src/obj

# Converting header file names to include IDIR
_DEPS = permutations.h CryptParameters.h CryptOption.h CryptMode.h CryptAlgorithm.h DesAlgorithm.h Debugger.h cryptUtils.h constants.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

# Converting object file names to include ODIR
# encryptor and decryptor must be compiled separately
# as both contain 'main' functions
_OBJ = permutations.o CryptParameters.o CryptOption.o CryptMode.o EcbMode.o CbcMode.o PcbcMode.o CfbMode.o OfbMode.o DesAlgorithm.o cryptUtils.o constants.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_CRYPT_OBJ = crypt.o
CRYPT_OBJ = $(patsubst %,$(ODIR)/%,$(_CRYPT_OBJ))

# Rule for compiling any object file from .cpp file
$(ODIR)/%.o: src/%.cpp $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

# Rule for compiling both binaries, encryptor and decryptor
# Sentinel file is used to ensure all results are up to date
$(ODIR)/.sentinel: $(OBJ) $(CRYPT_OBJ)
	$(CC) -g -o crypt $(OBJ) $(CRYPT_OBJ) $(CFLAGS)
	touch $(ODIR)/.sentinel

.PHONY: test
test: 
	$(MAKE) $(ODIR)/.sentinel
	rm -rf test/
	mkdir -p test 
	ls -l >> test/input.txt
	#Testing ECB crypt
	./crypt encrypt test/input.txt -o test/encrypted.hex
	./crypt decrypt test/encrypted.hex -o test/decrypted.txt
	#Testing CBC crypt
	./crypt encrypt test/input.txt -o test/encryptedCbc.hex -m cbc
	./crypt decrypt test/encryptedCbc.hex -o test/decryptedCbc.txt -m cbc
	#Testing PCBC crypt
	./crypt encrypt test/input.txt -o test/encryptedPcbc.hex -m pcbc
	./crypt decrypt test/encryptedPcbc.hex -o test/decryptedPcbc.txt -m pcbc
	#Testing CFB Crypt
	./crypt encrypt test/input.txt -o test/encryptedCfb.hex -m cfb -s 8
	./crypt decrypt test/encryptedCfb.hex -o test/decryptedCfb.txt -m cfb -s 8
	#Testing CFB Crypt
	./crypt encrypt test/input.txt -o test/encryptedOfb.hex -m ofb -s 4
	./crypt decrypt test/encryptedOfb.hex -o test/decryptedOfb.txt -m ofb -s 4
	@echo 
	@echo "Running diff on input and decrypted files -------------------------"
	@echo 
	cat test/decrypted.txt
	cat test/decryptedCbc.txt
	cat test/decryptedPcbc.txt
	cat test/decryptedCfb.txt
	cat test/decryptedOfb.txt

.PHONY: all
all: 
	$(MAKE)

# Rule to tidy any output
.PHONY: clean
clean:
	rm -f $(ODIR)/*.o *~ core $(IDIR)/*~ crypt play encrypted.txt decrypted.txt
	rm -rf test

playground: $(ODIR)/playground.o
	$(CC) -g -o play $(ODIR)/playground.o $(OBJ) $(CFLAGS)
