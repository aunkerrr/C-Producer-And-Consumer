Domande di progettazione

Prima di scrivere il codice, rispondere per iscritto alle seguenti domande. Le risposte guidano le scelte implementative.

    D1 — Quante sentinelle deve inserire ogni produttore quando raggiunge EOF? Perché proprio quel numero?
    D2 — È possibile che due produttori leggano EOF “contemporaneamente” e inseriscano entrambi le sentinelle? Come si evita che le sentinelle vengano inserite più volte del dovuto?
    D3 — Perché il mutex sul buffer (mutex_buf) non è sufficiente da solo e servono anche i semafori sem_vuoti e sem_pieni?
    D4 — L’ordine dei caratteri in output.txt è garantito essere identico a quello di input.txt? Motivare la risposta considerando che ci sono più consumatori che scrivono sullo stesso file.
    D5 — Cosa succede se BUFFER_SIZE è inferiore al numero di produttori NP? Il programma può andare in deadlock?

    D1 — Quante sentinelle deve inserire ogni produttore quando raggiunge EOF? Perché proprio quel numero?
    Il produttore (nello specifico, il primo che incontra l'EOF) deve inserire esattamente un numero di sentinelle pari a NC (il numero totale di thread consumatori).
    Motivazione: Ogni consumatore estrarrà esattamente una sentinella (ad esempio il carattere \0) dal buffer per capire che non ci sono più dati da elaborare e terminare il proprio ciclo. Se venissero inserite meno sentinelle, alcuni consumatori rimarrebbero bloccati per sempre in attesa su sem_wait(&sem_pieni), causando un deadlock.

    D2 — È possibile che due produttori leggano EOF “contemporaneamente” e inseriscano entrambi le sentinelle? Come si evita che le sentinelle vengano inserite più volte del dovuto?
    Sì, sarebbe possibile se la lettura e la gestione dell'EOF non fossero adeguatamente sincronizzate.
    Come evitarlo: Si utilizza una variabile globale condivisa come flag (ad esempio int file_finished = 0;). Il primo produttore che legge EOF, trovandosi all'interno della sezione critica protetta dal mutex_file_in, controlla il flag, lo imposta a 1 e inserisce le sentinelle. Gli altri produttori che leggeranno EOF successivamente troveranno il flag già a 1 ed usciranno semplicemente dal ciclo, evitando di duplicare le sentinelle nel buffer.

    D3 — Perché il mutex sul buffer (mutex_buf) non è sufficiente da solo e servono anche i semafori sem_vuoti e sem_pieni?
    Il mutex garantisce esclusivamente la mutua esclusione: impedisce che due thread accedano o modifichino l'array del buffer nello stesso esatto istante, evitando la corruzione dei dati.
    Tuttavia, il mutex non gestisce i vincoli di stato. I semafori servono per la sincronizzazione condizionale: sem_vuoti blocca i produttori quando il buffer è pieno, e sem_pieni blocca i consumatori quando il buffer è vuoto. Senza i semafori, i thread dovrebbero controllare continuamente lo stato del buffer con un ciclo di attesa attiva (busy-waiting), sprecando inutilmente cicli di CPU.

    D4 — L’ordine dei caratteri in output.txt è garantito essere identico a quello di input.txt? Motivare la risposta considerando che ci sono più consumatori che scrivono sullo stesso file.
    L'ordine è garantito solo se le operazioni di lettura-inserimento e estrazione-scrittura sono rese completamente atomiche.
    Se un produttore legge un carattere, rilascia il mutex_file_in e solo dopo cerca di accedere al buffer, potrebbe essere superato da un altro produttore (race condition), disordinando i caratteri.
    Per garantire un ordine identico, il produttore deve mantenere il mutex_file_in bloccato finché non ha depositato il carattere nel buffer. Similmente, il consumatore deve bloccare il mutex_file_out prima di estrarre il carattere dal buffer e rilasciarlo solo dopo averlo scritto nel file di output. In questo modo, l'ordine sequenziale (FIFO) viene rispettato rigorosamente.

    D5 — Cosa succede se BUFFER_SIZE è inferiore al numero di produttori NP? Il programma può andare in deadlock?
    No, il programma non va in deadlock.
    Se la dimensione del buffer (es. 2) è inferiore al numero di produttori (es. 3), il terzo produttore si bloccherà semplicemente sulla chiamata sem_wait(&sem_vuoti), addormentandosi in attesa che si liberi un posto. Il sistema continuerà a funzionare correttamente; l'unica differenza è che un buffer molto piccolo crea un "collo di bottiglia" (bottleneck) che rallenta l'esecuzione, ma non compromette la correttezza logica. Finché c'è almeno un consumatore attivo che svuota il buffer, il sistema farà progressi.