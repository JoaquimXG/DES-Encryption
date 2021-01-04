SHELL := bash
.SHELLFLAGS := -eu -o pipefail -c  
.DELETE_ON_ERROR:

# Header directory
IDIR =include

# Compiler and flags
CC=g++
CFLAGS=-I$(IDIR)

# Object output directory
ODIR=src/obj

# Converting header file names to include IDIR
_DEPS = permutations.h functions.h EncryptionParameters.h Options.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

# Converting object file names to include ODIR
# encryptor and decryptor must be compiled separately
# as both contain 'main' functions
_OBJ = des.o functions.o permutations.o EncryptionParameters.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_ENCRYPT_OBJ = encryptor.o
ENCRYPT_OBJ = $(patsubst %,$(ODIR)/%,$(_ENCRYPT_OBJ))

_DECRYPT_OBJ = decryptor.o
DECRYPT_OBJ = $(patsubst %,$(ODIR)/%,$(_DECRYPT_OBJ))

# Rule for compiling any object file from .cpp file
$(ODIR)/%.o: src/%.cpp $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

# Rule for compiling both binaries, encryptor and decryptor
# Sentinel file is used to ensure all results are up to date
$(ODIR)/.sentinel: $(OBJ) $(ENCRYPT_OBJ) $(DECRYPT_OBJ)
	$(CC) -g -o encryptor $(OBJ) $(ENCRYPT_OBJ) $(CFLAGS)
	$(CC) -g -o decryptor $(OBJ) $(DECRYPT_OBJ) $(CFLAGS)
	touch $(ODIR)/.sentinel

# Rule fo encryptor on its own
encryptor: $(OBJ) $(ENCRYPT_OBJ)
	$(CC) -o encryptor $(OBJ) $(ENCRYPT_OBJ) $(CFLAGS)

# Rule for decryptor on its own
decryptor: $(OBJ) $(DECRYPT_OBJ)
	$(CC) -o encryptor $(OBJ) $(ENCRYPT_OBJ) $(CFLAGS)

# Rule to tidy any output
.PHONY: clean
clean:
	rm -f $(ODIR)/*.o *~ core $(IDIR)/*~ encryptor decryptor 
	rm -rf test

# Rule to perform the most basic tests
.PHONY: test
test:
	$(MAKE) $(ODIR)/.sentinel
	rm -rf test/
	mkdir -p test 
	ls -l >> test/input.txt
	./encryptor test/input.txt -o test/encrypted.hex
	./decryptor test/encrypted.hex -o test/decrypted.txt
	./encryptor test/input.txt -o test/encryptedcbc.hex -m cbc
	./decryptor test/encryptedcbc.hex -o test/decryptedcbc.txt -m cbc
	./encryptor test/input.txt -o test/encryptedpcbc.hex -m pcbc
	./decryptor test/encryptedpcbc.hex -o test/decryptedpcbc.txt -m pcbc
	./encryptor test/input.txt -o test/encryptedcfb.hex -m cfb
	./decryptor test/encryptedcfb.hex -o test/decryptedcfb.txt -m cfb
	./encryptor test/input.txt -o test/encryptedofb.hex -m ofb
	./decryptor test/encryptedofb.hex -o test/decryptedofb.txt -m ofb
	@echo 
	@echo 
	@echo "Running diff on input and decrypted files -------------------------"
	@echo 
	cat test/decrypted.txt	
	cat test/decryptedcbc.txt	
	cat test/decryptedpcbc.txt	
	cat test/decryptedcfb.txt	
	cat test/decryptedofb.txt	


playground: $(ODIR)/playground.o
	$(CC) -g -o play $(ODIR)/playground.o $(OBJ) $(CFLAGS)
