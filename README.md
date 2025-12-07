# Proiect RPC – Remote Procedure Call in C

Proiectul implementează un sistem simplu de **Remote Procedure Call (RPC)** folosind **socket-uri TCP** în C pe Linux.  

Scopul este ca un program client să poată apela funcții care sunt de fapt executate pe un proces server, prin rețea.

Serverul expune funcții simple (ex: `add`, `sub`), iar clientul trimite numele funcției și argumentele, primind înapoi rezultatul calculat pe server.


## Componentele proiectului

# Fisier comun (`rpc_common.h`)
Conține definițiile folosite atât de client, cât și de server:
- structurile pentru cerere (`rpc_request`) și răspuns (`rpc_response`);
- portul de comunicare (`PORT 5000`);
- includerile standard pentru socket-uri.

# Biblioteca RPC (`rpc_lib.c/.h`)
Oferă funcții ajutătoare pentru:
- trimiterea și primirea datelor (`rpc_send`, `rpc_recv`);
- conectarea la server cu timeout (`rpc_connect`);
- retry automat în caz de eșec (`rpc_retry_connect`).

Aceasta abstractizează comunicarea prin socketuri și permite gestionarea ușoară a erorilor și timeout-urilor.

# Serverul RPC (`rpc_server.c`)
- creează un socket TCP și îl asociază unui port (bind);
- ascultă conexiuni cu `listen()`;
- acceptă clienți cu `accept()` în buclă infinită;
- pentru fiecare client, creează un **thread separat** (`pthread_create`);
- primește structura `rpc_request`, execută funcția corespunzătoare (`add`, `sub`);
- trimite rezultatul prin `write()` și închide conexiunea.

Serverul este **multi-threaded**, putând deservi mai mulți clienți simultan.


# Clientul RPC (`rpc_client.c` + `main_client.c`)
Clientul se conectează la server și permite două moduri de apel:

- **Mod sincron:** trimite cererea și așteaptă răspunsul;
- **Mod asincron:** lansează un thread care trimite cererea și primește răspunsul în fundal, fără a bloca execuția.

Fluxul:
1. utilizatorul introduce funcția (`add`/`sub`) și argumentele;
2. clientul trimite cererea către server;
3. serverul procesează și trimite rezultatul;
4. clientul afișează rezultatul.


## Fluxul cererii RPC

CLIENT                             SERVER
socket() → connect() ------------> accept()
write(req) ----------------------> read(req)
execută funcția
read(res) <---------------------- write(res)
close() ----------------------> close()


Fiecare cerere RPC se face pe o conexiune separată.

## Alte detalii
- Suport pentru **apeluri sincron/asincron** și **multi-threading**.
- Gestionarea erorilor, timeout-urilor și retry-urilor în biblioteca RPC.
- Cod modular și ușor extensibil (se pot adăuga funcții noi pe server, ex: `mul`, `div`, `pow`).

## Compilare și rulare

Proiectul include un **Makefile**:
```bash
make        # compilează server și client
make server # compilează doar serverul
make client # compilează doar clientul
make clean  # șterge fișierele intermediare
```

Rulare:
Pornește serverul:

```bash
./server
```

Pornește clientul:
```bash
./client
```


Proiectul demonstrează principiile unui sistem RPC de bază:
comunicare client–server prin socketuri TCP, apeluri la distanță, moduri sincron/asincron și server multi-threaded.