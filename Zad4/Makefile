.SUFFIXES: .c .cpp .o .x .h
DIR = $(notdir $(CURDIR))
SYSNAME = $(shell uname -s)
NAME = pipe
EXEC  = $(NAME).x  
OBJS  = $(NAME).o
NAMESOURCE = source.txt
NAMEDESTINATION = destination.txt
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
all: $(EXEC) 


#Komplilacja
$(EXEC): $(OBJS) $(LIBS) 
	$(LD) -o $@ $(LFLAGS) $^

#Uruchamianie
.PHONY: run
run: $(EXEC)
	./$(EXEC) $(NAMESOURCE) $(NAMEDESTINATION)
	sleep 1
	diff $(NAMESOURCE) $(NAMEDESTINATION) || true

###############################################################################
# Sprzataczka (tez "phony target")
###############################################################################
.PHONY: clean tar
clean:                                                     
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR}
# Archiwizacja i kompresja
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
