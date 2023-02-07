
all: app gcov

#build app with coverage flag, then generate *.gcno file
app: 
	gcc --coverage ./tmp.c ./lib.c -o tmp

gcov: app
	#run app, create the counter file *.gcda
	./tmp

	#covert the counter to a html report file format by lcov & genhtml
	lcov -c -o main.info -d .
	genhtml -o html main.info

clean:
	rm *.o  tmp html -rf 
	rm *.gcda *.gcno main.info *.html -f
