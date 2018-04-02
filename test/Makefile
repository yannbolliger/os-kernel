
CFLAGS+= -g

all: min_unit

min_unit: min_unit.o ../kernel/sched.o ../kernel/sched_rq.o sched_tests.o sched_rq_tests.o



# little macro to delete all object files
.PHONY: r remake clean
clean:
	rm *.o ../kernel/sched.o min_unit

remake: clean all
r: remake
