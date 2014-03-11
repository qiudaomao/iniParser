target=test.out
$(target): ConfigINI.cpp ConfigINI.h test.cpp
	g++ ConfigINI.cpp test.cpp -o $(target)
run:$(target)
	./test.out
clean:
	rm -rf *.out *.o
