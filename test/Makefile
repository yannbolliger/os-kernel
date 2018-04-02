


all: min_unit

min_unit: min_unit.o ../kernel/sched.o sched_tests.o



# little macro to delete all object files
.PHONY: r remake clean
clean:
	rm *.o ../kernel/sched.o min_unit

remake: clean all
r: remake
