


all: min_unit

min_unit: min_unit.o ../kernel/scheduler.o scheduler_tests.c



# little macro to delete all object files
.PHONY: r remake clean
clean:
	rm *.o ../kernel/scheduler.o min_unit

remake: clean all
r: remake
