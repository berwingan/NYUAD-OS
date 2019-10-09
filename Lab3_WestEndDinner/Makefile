all: 	restaurant  \
	client \
	cashier \
	server \
	cook

cook: cook.c
	gcc cook.c -o cook -lpthread 

restaurant: restaurant.c
	gcc restaurant.c -o restaurant -lpthread

client: client.c
	gcc  client.c -o client -lpthread

cashier: cashier.c
	gcc  cashier.c -o cashier -lpthread

server: server.c
	gcc -g server.c -o  server -lpthread

clean:
	rm -f restaurant cook client  \
		server cashier 
