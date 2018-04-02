
CFLAGS+= -g

all: min_unit

TESTED_OBJECTS = ../kernel/sched.o ../kernel/sched_rq.o
TEST_FILES = $(addsuffix _tests.o, $(basename $(notdir $(TESTED_OBJECTS))))

min_unit: min_unit.o $(TESTED_OBJECTS) $(TEST_FILES)



# little macro to delete all object files
.PHONY: r remake clean
clean:
	rm *.o $(TESTED_OBJECTS) min_unit

remake: clean all
r: remake
