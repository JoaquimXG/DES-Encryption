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
_DEPS = OLDpermutations.h OLDfunctions.h OLDOptions.h EncryptionParameters.h 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

# Converting object file names to include ODIR
# encryptor and decryptor must be compiled separately
# as both contain 'main' functions
_OBJ = OLDdes.o OLDfunctions.o OLDpermutations.o EncryptionParameters.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_ENCRYPT_OBJ = OLDencryptor.o
ENCRYPT_OBJ = $(patsubst %,$(ODIR)/%,$(_ENCRYPT_OBJ))

_DECRYPT_OBJ = OLDdecryptor.o
DECRYPT_OBJ = $(patsubst %,$(ODIR)/%,$(_DECRYPT_OBJ))

# Rule for compiling any object file from .cpp file
$(ODIR)/%.o: src/%.cpp $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

# Rule to tidy any output
.PHONY: clean
clean:
	rm -f $(ODIR)/*.o *~ core $(IDIR)/*~ OLDencryptor OLDdecryptor 
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
	$(CC) -o OLDencryptor $(OBJ) $(ENCRYPT_OBJ) $(CFLAGS)

# Rule to perform the most basic tests
.PHONY: OLDtest
OLDtest:
	$(MAKE) $(ODIR)/.sentinel
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
