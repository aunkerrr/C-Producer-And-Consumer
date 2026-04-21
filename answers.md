Domande di progettazione

Prima di scrivere il codice, rispondere per iscritto alle seguenti domande. Le risposte guidano le scelte implementative.

    D1 — Quante sentinelle deve inserire ogni produttore quando raggiunge EOF? Perché proprio quel numero?
    D2 — È possibile che due produttori leggano EOF “contemporaneamente” e inseriscano entrambi le sentinelle? Come si evita che le sentinelle vengano inserite più volte del dovuto?
    D3 — Perché il mutex sul buffer (mutex_buf) non è sufficiente da solo e servono anche i semafori sem_vuoti e sem_pieni?
    D4 — L’ordine dei caratteri in output.txt è garantito essere identico a quello di input.txt? Motivare la risposta considerando che ci sono più consumatori che scrivono sullo stesso file.
    D5 — Cosa succede se BUFFER_SIZE è inferiore al numero di produttori NP? Il programma può andare in deadlock?
