CC = gcc
OBJS = election.o area.o tribe.o assist.o map.o node.o electionTestsExample.o
EXEC = election
DEBUG_FLAGS = -g
COMP_FLAGS = -std=c99 -Wall -Werror

$(EXEC) : $(OBJS)
	$(CC) $(DEBUG_FLAGS) $(OBJS) -o $@
area.o: area.c mtm_map/map.h area.h election.h assist.h tribe.h
	$(CC) -c  $(DEBUG_FLAGS) $(COMP_FLAGS) $*.c 
assist.o: assist.c assist.h
	$(CC) -c  $(DEBUG_FLAGS) $(COMP_FLAGS) $*.c 
election.o: election.c mtm_map/map.h election.h area.h assist.h tribe.h
	$(CC) -c  $(DEBUG_FLAGS) $(COMP_FLAGS) $*.c 
electionTestsExample.o: tests/electionTestsExample.c election.h mtm_map/map.h test_utilities.h
	$(CC) -c  $(DEBUG_FLAGS) $(COMP_FLAGS) tests/$*.c 
tribe.o: tribe.c mtm_map/map.h assist.h tribe.h
	$(CC) -c  $(DEBUG_FLAGS) $(COMP_FLAGS) $*.c 
map.o: mtm_map/map.c mtm_map/map.h mtm_map/node.h
	$(CC) -c  $(DEBUG_FLAGS) $(COMP_FLAGS) mtm_map/$*.c 
node.o: mtm_map/node.c mtm_map/node.h
	$(CC) -c  $(DEBUG_FLAGS) $(COMP_FLAGS) mtm_map/$*.c 
clean:
	rm -f $(OBJS) $(EXEC)