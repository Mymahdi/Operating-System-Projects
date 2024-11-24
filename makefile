all: main warehouse product

main: main.cpp utils.cpp
	g++ -o main main.cpp utils.cpp -std=c++17

warehouse: warehouse.cpp utils.cpp
	g++ -o warehouse warehouse.cpp utils.cpp -std=c++17

product: product.cpp utils.cpp
	g++ -o product product.cpp utils.cpp -std=c++17

clean:
	rm -f main warehouse product
