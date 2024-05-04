# Development Process
### EN

## Requirements analysis
Simulate a login page with communication between client and server by writing to a file


## Design
- `chat.txt` file for communicating.
- `accounts.txt` file for saving recordings.
- `id and password` for the client


- `startServer` the "server" writes to the file that is listening;
- `startClient`
- reads from the file if the server is listening and if so continues,
- **writes `id, password` and `action`** to the file making your request for access or registration;
- `Client` writes and reads responses to and from the **server** with the `chat.txt` file;
- `Server`
- writes and reads responses to and from the **client** with the `chat.txt` file,
- checks if the client's request is legitimate, i.e. *if it is not registered* or *if the id is already registered*;
Error status code
- `401` Unauthorized when the client asks to log in but is not registered
- `403` Forbidden when registration is not possible with that username
- `400` Bad Request The request could not be fulfilled due to syntax errors.
- `404` Not Found The requested resource was not found but may be available in the future.
Success status code
- `200` OK successful requests.

- `201` Created The request was satisfied, returning access
- `202` Accepted The processing request has been accepted but has not yet finished.
- ~~`203` Non-Authoritative Information. The server is a _transforming proxy_ (e.g. a Web Accelerator) that has received a 200 OK from its origin, but is returning a modified version of the origin's response.~~
- `204` No Content The server successfully processed the request and will not return any content.
- `205` Reset Content The server successfully processed the request and will not return any content. Unlike response 204, this requires the requester to reset the _document view_.

---

# Processo di Sviluppo
### IT 

## Analisi dei Requisiti
Simulare una pagina di login con comunicazione tra client e server tramite scrittura su file


## Progettazione
- File `chat.txt` per comunicare.
- File `accounts.txt` per salvare le registrazioni.
- `id e password` per il client


- `startServer` il "server" scrive sul file che è in ascolto;
- `startClient` 
	- legge dal file, se il server è in ascolto e in caso prosegue,
	- **scrive sul file `id, passsword` e `action`** facendo la sua richiesta, di accesso  o di registrazione;
- `Client` scrive e legge le risposte da e verso il **server** con il file `chat.txt`;
- `Server`
	-  scrive e legge le risposte da e verso il **client** con il file `chat.txt`, 
	- controlla se la richiesta del client  è lecita, cioè *se non è registrato* o *se l'id è già registrato*;
	Status code di errore
	- `401` Unauthorized quando il client chiede di accedere ma non è registrato
	- `403` Forbidden quando la registrazione non è possibile con quel nome utente
	- `400` Bad Request La richiesta non può essere soddisfatta a causa di errori di sintassi.
	- `404` Not Found La risorsa richiesta non è stata trovata ma in futuro potrebbe essere disponibile.
	Status code di successo
	- `200` OK richieste andate a buon fine.
	
	- `201` Created La richiesta è stata soddisfatta, restituendo l'accesso
	- `202` Accepted La richiesta di elaborazione è stata accettata ma non è ancora terminata.
	- ~~`203` Non-Authoritative Information.	Il server è un _transforming proxy_ (ad es. un Web Accelerator) che ha ricevuto un 200 OK dalla sua origine, ma sta restituendo una versione modificata della risposta dell'origine.~~
	- `204` No Content Il server ha processato con successo la richiesta e non restituirà nessun contenuto.
	- `205` Reset Content Il server ha processato con successo la richiesta e non restituirà nessun contenuto. Al contrario della risposta 204, questa richiede che il richiedente resetti il _document view_.
