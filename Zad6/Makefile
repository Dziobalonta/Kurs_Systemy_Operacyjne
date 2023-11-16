.SUFFIXES: .c .cpp .o .x .h
DIR = $(notdir $(CURDIR))
SYSNAME = $(shell uname -s)
NAME1 = powielacz
NAME2 = krytyczna
NAME3 = semafor
NAME4 = powielacz_2
NAME5 = krytyczna_2
PROCESY = 3
KRYTYCZNE = 1
EXEC1  = $(NAME1).x  
EXEC2  = $(NAME2).x
EXEC4  = $(NAME4).x
EXEC5  = $(NAME5).x
OBJS1  = $(NAME1).o
OBJS2  = $(NAME2).o
OBJS3  = $(NAME3).o
OBJS4  = $(NAME4).o
OBJS5  = $(NAME5).o
LIB_NAME = SemaforFunkcje
LIB_DIR = ./lib
LIB_STAT = lib$(LIB_NAME).a
LIB_OBJS = $(OBJS3)
LIBS  = 
HEADS1 = 
HEADS2 = 
HEADS3 = $(NAME3).h 
CFLAGS = -Wall -std=c++11 -pedantic -O -fPIC
LFLAGS = -Wall -std=c++11 -pedantic -O
LDLIBS = -lm
CO = g++
LD = $(CO)
AR = ar
ARFLAGS = rsv

%.o: %.c %.h
	$(CO) $(CFLAGS) -c $<

%.o: %.c
	$(CO) $(CFLAGS) -c $<

$(NAME3).o: $(NAME3).cpp
	$(CO) $(CFLAGS) -c $< -I include

$(NAME4).o: $(NAME4).cpp
	$(CO) $(CFLAGS) -c $< -I include

$(NAME5).o: $(NAME5).cpp
	$(CO) $(CFLAGS) -c $< -I include

#all
.PHONY: all
all: $(EXEC1) $(EXEC2) $(EXEC4) $(EXEC5) $(LIB_STAT)


#Komplilacja
$(EXEC1): $(OBJS1) $(LIBS) 
	$(LD) -o $@ $(LFLAGS) $^

$(EXEC2): $(OBJS2) $(LIBS) 
	$(LD) -o $@ $(LFLAGS) $^

$(LIB_STAT): $(LIB_OBJS)
	$(AR) $(ARFLAGS) $@ $?
	mkdir -p $(LIB_DIR)
	mv $(LIB_STAT) $(LIB_DIR)

$(EXEC4): $(OBJS4) $(LIB_STAT)
	$(LD) $(OBJS4) -o $@ $(LFLAGS) -L$(LIB_DIR) -l$(LIB_NAME) $(LDLIBS) -I include

$(EXEC5): $(OBJS5) $(LIB_STAT)
	$(LD) $(OBJS5) -o $@ $(LFLAGS) -L$(LIB_DIR) -l$(LIB_NAME) $(LDLIBS) -I include


#Uruchamianie
.PHONY: run1 run2
run1: $(EXEC1) $(EXEC2)
	./$(EXEC1) ./$(EXEC2) $(PROCESY) $(KRYTYCZNE)

run2: $(EXEC4) $(EXEC5)
	./$(EXEC4) ./$(EXEC5) $(PROCESY) $(KRYTYCZNE)


###############################################################################
# Sprzataczka (tez "phony target")
###############################################################################
.PHONY: clean tar
clean:                                                     
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR}
# Archiwizacja i kompresja
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )