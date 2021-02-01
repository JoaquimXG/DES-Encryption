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
_DEPS = OLDpermutations.h OLDfunctions.h OLDOptions.h CryptParameters.h CryptOption.h CryptMode.h CryptAlgorithm.h DesAlgorithm.h Debugger.h cryptUtils.h 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

# Converting object file names to include ODIR
# encryptor and decryptor must be compiled separately
# as both contain 'main' functions
_OBJ = OLDdes.o OLDfunctions.o OLDpermutations.o CryptParameters.o CryptOption.o CryptMode.o EcbMode.o CbcMode.o PcbcMode.o CfbMode.o DesAlgorithm.o cryptUtils.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_ENCRYPT_OBJ = OLDencryptor.o
ENCRYPT_OBJ = $(patsubst %,$(ODIR)/%,$(_ENCRYPT_OBJ))

_DECRYPT_OBJ = OLDdecryptor.o
DECRYPT_OBJ = $(patsubst %,$(ODIR)/%,$(_DECRYPT_OBJ))

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
	$(MAKE) OLDencryptor
	$(MAKE) OLDdecryptor
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
	./crypt encrypt test/input.txt -o test/encryptedCfb.hex -m cfb
	./crypt decrypt test/encryptedCfb.hex -o test/decryptedCfb.txt -m cfb
	#Testing OFB Crypt using old decryptor
	./crypt encrypt test/input.txt -o test/encryptedOfb.hex -m ofb
	#--------------------
	./crypt decrypt test/encryptedOfb.hex -o test/decryptedOfb.txt -m ofb
	./OLDdecryptor test/encryptedOfb.hex -o test/OLDdecryptedOfb.txt -m ofb
	@echo 
	@echo "Running diff on input and decrypted files -------------------------"
	@echo 
	cat test/decrypted.txt
	cat test/decryptedCbc.txt
	cat test/decryptedPcbc.txt
	cat test/decryptedCfb.txt
	cat test/OLDdecryptedOfb.txt
	cat test/decryptedOfb.txt

.PHONY: all
all: 
	$(MAKE)
	$(MAKE) OLD

# Rule to tidy any output
.PHONY: clean
clean:
	rm -f $(ODIR)/*.o *~ core $(IDIR)/*~ OLDencryptor OLDdecryptor crypt play encrypted.txt decrypted.txt
	rm -rf test

# Rule for compiling both OLD binaries, encryptor and decryptor
.PHONY: OLD
OLD: $(OBJ) $(ENCRYPT_OBJ) $(DECRYPT_OBJ)
	$(CC) -g -o OLDencryptor $(OBJ) $(ENCRYPT_OBJ) $(CFLAGS)
	$(CC) -g -o OLDdecryptor $(OBJ) $(DECRYPT_OBJ) $(CFLAGS)

# Rule fo encryptor on its own
OLDencryptor: $(OBJ) $(ENCRYPT_OBJ)
	$(CC) -o OLDencryptor $(OBJ) $(ENCRYPT_OBJ) $(CFLAGS)

# Rule for decryptor on its own
OLDdecryptor: $(OBJ) $(DECRYPT_OBJ)
	$(CC) -o OLDdecryptor $(OBJ) $(DECRYPT_OBJ) $(CFLAGS)

# Rule to perform the most basic tests
.PHONY: OLDtest
OLDtest:
	$(MAKE) OLD
	rm -rf test/
	mkdir -p test 
	ls -l >> test/input.txt
	./OLDencryptor test/input.txt -o test/OLDencrypted.hex
	./OLDdecryptor test/OLDencrypted.hex -o test/OLDdecrypted.txt
	./OLDencryptor test/input.txt -o test/OLDencryptedcbc.hex -m cbc
	./OLDdecryptor test/OLDencryptedcbc.hex -o test/OLDdecryptedcbc.txt -m cbc
	./OLDencryptor test/input.txt -o test/OLDencryptedpcbc.hex -m pcbc
	./OLDdecryptor test/OLDencryptedpcbc.hex -o test/OLDdecryptedpcbc.txt -m pcbc
	./OLDencryptor test/input.txt -o test/OLDencryptedcfb.hex -m cfb
	./OLDdecryptor test/OLDencryptedcfb.hex -o test/OLDdecryptedcfb.txt -m cfb
	./OLDencryptor test/input.txt -o test/OLDencryptedofb.hex -m ofb
	./OLDdecryptor test/OLDencryptedofb.hex -o test/OLDdecryptedofb.txt -m ofb
	@echo 
	@echo 
	@echo "Running diff on input and decrypted files -------------------------"
	@echo 
	cat test/OLDdecrypted.txt	
	cat test/OLDdecryptedcbc.txt	
	cat test/OLDdecryptedpcbc.txt	
	cat test/OLDdecryptedcfb.txt	
	cat test/OLDdecryptedofb.txt	


playground: $(ODIR)/playground.o
	$(CC) -g -o play $(ODIR)/playground.o $(OBJ) $(CFLAGS)
