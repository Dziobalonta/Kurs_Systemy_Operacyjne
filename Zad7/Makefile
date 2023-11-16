.SUFFIXES: .c .cpp .o .x .h
DIR = $(notdir $(CURDIR))
SYSNAME = $(shell uname -s)

#Nazwy
MAIN_NAME = main
PROD_NAME = producent
KONS_NAME = konsument
PAM_NAME = pamiec_dzielona
SEM_NAME = semafor
IN = wejscie.txt
OUT = wyjscie.txt

#Pliki obiektowe
MAIN_OBJ  = $(MAIN_NAME).o
PROD_OBJ  = $(PROD_NAME).o
KONS_OBJ  = $(KONS_NAME).o
PAM_OBJ  = $(PAM_NAME).o
SEM_OBJ  = $(SEM_NAME).o

#Pliki wykonywalne
MAIN_EXEC_S = $(MAIN_NAME)_S.x
PROD_EXEC_S = $(PROD_NAME)_S.x
KONS_EXEC_S = $(KONS_NAME)_S.x

MAIN_EXEC_D1 = $(MAIN_NAME)_D1.x
PROD_EXEC_D1 = $(PROD_NAME)_D1.x
KONS_EXEC_D1 = $(KONS_NAME)_D1.x

MAIN_EXEC_D2 = $(MAIN_NAME)_D2.x
PROD_EXEC_D2 = $(PROD_NAME)_D2.x
KONS_EXEC_D2 = $(KONS_NAME)_D2.x

#naglowkowe
SEM_HEAD = $(SEM_NAME).h
PAM_HEADS = $(PAM_NAME).h

LIB_NAME = Zad7Funkcje
LIB_DIR = ./lib
#Biblioteka statyczna
LIB_STAT = lib$(LIB_NAME).a
#Biblioteka dynamiczna
LIB_SHAR = lib$(LIB_NAME).so

#FLAGI
FLAGS = -Wall -std=c++11 -pedantic
CFLAGS = -Wall -std=c++11 -pedantic -O -fPIC
LFLAGS = -Wall -std=c++11 -pedantic -O
LDLIBS = -lm
AR = ar
ARFLAGS = rsv

#KOMPILATOR / LINKER 
CO = g++
LD = $(CO)

#tworzenie plikow obiektowych

%.o: %.cpp
	$(CO) $^ $(FLAGS) -c

$(LIB_STAT): $(SEM_OBJ) $(PAM_OBJ)
	$(AR) $(ARFLAGS) $@ $?
	mkdir -p $(LIB_DIR)
	mv $(LIB_STAT) $(LIB_DIR)

$(LIB_SHAR): $(SEM_OBJ) $(PAM_OBJ)
	$(LD) -shared -o $@ $(FLAGS) $(SEM_OBJ) $(PAM_OBJ)
	mkdir -p $(LIB_DIR)
	mv $(LIB_SHAR) $(LIB_DIR)

#tworzenie plikow z biblioteka statyczna
%_S.x: %.o $(LIB_STAT)
	$(LD) -o $@ $(FLAGS) $< -L$(LIB_DIR) -l$(LIB_NAME) $(CFLAGS)

#tworzenie plikow z bliblioteka dzielona -1 sposob
%_D1.x: %.o $(LIB_SHAR)
	$(LD) -o $@ $(FLAGS) $< -L$(LIB_DIR) -l$(LIB_NAME) -Wl,-R $(LIB_DIR) $(CFLAGS)

#tworzenie plikow z bliblioteka dzielona -2 sposob
%_D2.x: %.o $(LIB_SHAR)
	$(LD) -o $@ $(FLAGS) $< -L$(LIB_DIR) -l$(LIB_NAME) $(CFLAGS)

#Uruchamianie
.PHONY: run_statyczna run_dzielona1 run_dzielona2

#biblioteka statyczna
run_statyczna: $(MAIN_EXEC_S) $(PROD_EXEC_S) $(KONS_EXEC_S)
	./$(MAIN_EXEC_S) ./$(PROD_EXEC_S) ./$(KONS_EXEC_S) $(IN) $(OUT)
	diff $(IN) $(OUT) || true

#biblioteka dzielona -1 sposob
run_dzielona1: $(MAIN_EXEC_D1) $(PROD_EXEC_D1) $(KONS_EXEC_D1)
	./$(MAIN_EXEC_D1) ./$(PROD_EXEC_D1) ./$(KONS_EXEC_D1) $(IN) $(OUT)
	diff $(IN) $(OUT) || true

#biblioteki dzielona -2 sposob
run_dzielona2: $(MAIN_EXEC_D2) $(PROD_EXEC_D2) $(KONS_EXEC_D2)
	(export LD_LIBRARY_PATH=${LIB_DIR}:${LD_LIBRARY_PATH}; \
	./$(MAIN_EXEC_D2) ./$(PROD_EXEC_D2) ./$(KONS_EXEC_D2) $(IN) $(OUT))
	diff $(IN) $(OUT) || true



###############################################################################
# Sprzataczka (tez "phony target")
###############################################################################
.PHONY: clean tar
clean:                                                     
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR}
# Archiwizacja i kompresja
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )