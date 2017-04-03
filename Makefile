

testslip : slip.o testslip.o
	gcc -o testslip slip.o testslip.o

testslip.o : test/testslip.c
	gcc -c test/testslip.c -Iinc

slip.o : src/slip.c
	gcc -c src/slip.c -Iinc

.PHONY : clean
clean:
	rm testslip *.o