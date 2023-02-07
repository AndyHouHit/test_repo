
all: app gcov

app: 
	gcc --coverage ./tmp.c -o tmp

gcov: app
	./tmp
	lcov -c -o main.info -d .
	genhtml -o html main.info

clean:
	rm *.o  tmp html -rf 
	rm *.gcda *.gcno main.info *.html
