Proiectul implementeaza un sistem simplu de Remote Procedure Call (RPC) folosind socket-uri TCP in C pe Linux.
Scopul este ca un program client sa poata apela functii care sunt de fapt executate pe un proces server, prin retea.
Momentan, serverul expune cateva functii de exemplu (add, sub), iar clientul trimite numele functiei si argumentele, primind inapoi rezultatul calculat pe server.

Componentele proiectului:
1) Fisier comun (rpc_common.h)
Contine definitiile folosite atat de client, cat si de server:
-structuri pentru cerere si raspuns
-portul de comunicare
-includerile standard pentru socket-uri
2) Serverul RPC (rpc_server.c)
-creeaza un socket TCP si il asociaza unui port (bind)
-asculta conexiuni cu listen()
-accepta clienti cu accept()
-primeste structura rpc_request de la client
-verifica numele functiei 
-executa functia locala corespunzatoare
-trimite inapoi rezultatul prin write()
3) Clientul RPC (rpc_client.c)
creeaza socketul si se conecteaza la server
-cere utilizatorului functia si argumentele
-trimite cererea catre server
-asteapta raspunsul
-afiseaza rezultatul pe ecran
-inchide conexiunea

Fiecare cerere RPC se face pe o conexiune separata.

Fluxul cererii
CLIENT                              SERVER
---------------------------------------------------
socket() → connect()  ------------> accept()
write(req)  ----------------------> read(req)
                                   executa functia
read(res)  <---------------------- write(res)
close()     ----------------------> close()

Proiectul include si un Makefile.

