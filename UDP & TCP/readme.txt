Nume: Andronic Dragos Sebastian
Grupa: 324CDa

Fisiere header
data.h: 
    Declar doua structuri de date TCP_Command (ce transmit de la subscriber la server)
    si UDP_Respose (ce transmit de la server la subscriber).

list.h:
    Declar doua structuri de date Client_Info (informatii legate de subscriber: socket, id,
    status (daca e conectat sau nu), coada cu mesajele salvate prin SF, lista de topicuri
    la care e abonat) si Topic (informatii legate topic-uri: nume si parametrul de SF), dar si
    lista de clienti (Client_List) si lista de topci-uri (Topic_List)

    Functii specifice listei de clienti: add_client, findCLient, freeClientList
    Functii specifice liste de topicuri: add_topic, remove_topic, get_topic, contains, freeTopicList

queue.h, queue_tcp_command.h, queue_udp_response.h :
    Declar cozi pentru structurile: UDP_Message, TCP_Command, UDP_Response
    Contin functii de creare, front (intoarce primul element din coada), enqueue, dequeue si
    dealocare memorie.

Protocol: transmit mesaje de aceeasi dimensiune si fac incadrarea lor in functie de dimensiunea trimisa
apoi adaug datele intr-o coada.

tcp_protocol_request.h:
    Contine functia protocol_tcp ce face incadrarea mesajelor primite de la subscriber (marimea TCP_Command) 
    intr-un buffer si le adauga intr-o coada.

tcp_protocol_response.h:
    Contine functia protocol_udp ce face incadrarea mesajelor primite de la subscriber (marimea UDP_Response) 
    intr-un buffer si le adauga intr-o coada.

Fisiere sursa:

server.cpp: asigura comunicarea intre subscriber si topicurile la care clientii s-au abonat. Daca primeste date
    pe socket-ul de UDP parcurg lista de clienti, daca clientul este abonat la topicul de la care am primit mesaj
    va verifica daca acesta e conectat si va transmite datele, altfel va verifica daca optiunea de Store and Forward
    este activa si va pune mesajele intr-o coada. 
        Daca se primesc date de pe socket-ul TCP verific daca este un client conectat cu acel nume, daca se conecteaza
    sau reconecteaza un client. Se ocupa si de actiunile de subscribe, unsubscribe sau deconectarea unui client.

subscriber.cpp: transmite o cerere de conectare catre server si actiunile de subscribe/unsubscribe si se ocupa de
    interpretarea mesajelor primite de la topicurile la care s-a abonat