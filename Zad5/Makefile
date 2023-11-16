.SUFFIXES: .c .cpp .o .x .h
DIR = $(notdir $(CURDIR))
SYSNAME = $(shell uname -s)
NAME1 = FifoPipe
NAME2 = FifoProd
NAME3 = FifoKons
NAMEFIFO = PotokNazwany
NAMESOURCE = zrodlo.txt
NAMEDESTINATION = destynacja.txt
EXEC1  = $(NAME1).x  
EXEC2  = $(NAME2).x
EXEC3  = $(NAME3).x
OBJS1  = $(NAME1).o
OBJS2  = $(NAME2).o 
OBJS3  = $(NAME3).o
LIBS  = 
HEADS = 
CFLAGS = -Wall -std=c99 -pedantic -O -fPIC
LFLAGS = -Wall -std=c99 -pedantic -O
LDLIBS = -lm
CO = g++
LD = $(CO)
AR = ar
ARFLAGS = rsv

%.o: %.c %.h
	$(CO) $(CFLAGS) -c $<

%.o: %.c
	$(CO) $(CFLAGS) -c $<

#all
.PHONY: all
all: $(EXEC1) $(EXEC2) $(EXEC3)


#Komplilacja
$(EXEC1): $(OBJS1) $(LIBS) 
	$(LD) -o $@ $(LFLAGS) $^

$(EXEC2): $(OBJS2) $(LIBS) 
	$(LD) -o $@ $(LFLAGS) $^

$(EXEC3): $(OBJS3) $(LIBS) 
	$(LD) -o $@ $(LFLAGS) $^

#Uruchamianie
.PHONY: run1
run1: $(EXEC1) $(EXEC2) $(EXEC3)
	./$(EXEC1) ./$(EXEC2) ./$(EXEC3)

.PHONY: run2
run2: $(EXEC2) $(EXEC3)
	mkfifo -m 0777 $(NAMEFIFO)
	xterm -hold -title Producent -e ./$(EXEC2) $(NAMEFIFO) $(NAMESOURCE) &
	xterm -hold -title Konsument -e ./$(EXEC3) $(NAMEFIFO) $(NAMEDESTINATION) &
	sleep 2
	diff $(NAMESOURCE) $(NAMEDESTINATION) || true
	rm PotokNazwany  


###############################################################################
# Sprzataczka (tez "phony target")
###############################################################################
.PHONY: clean tar
clean:                                                     
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR}
# Archiwizacja i kompresja
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
