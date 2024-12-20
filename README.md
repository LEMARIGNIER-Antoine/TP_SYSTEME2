Remarques globales : au départ, j'appelais gettftp.c depuis un main mais en fait, c'est un fichier à part et donc, il doit être changé directement dans le cmakeKist

Clairement, j'aurais du découper en plusieurs fonctions ce que je faisais mais surement pas manque de vision du tp, je  n'ai que 2 fichiers vraiment différents, get et put donc à refaire, je définirais mieux mes actions.

Questions 1 à 3 :
Le début du tp est très ressemblant au td donc je m'en suis très fortement inspiré.

Question 4 :
Il faut bien regarder comment la requête RRQ est construite et après, on additionne, les différents élements de la requête
![Capture d’écran du 2024-12-11 14-58-33](https://github.com/user-attachments/assets/85c80d41-e630-4330-87c1-f2eae7341d6a)

Sur wireshark, en vérifiant au port 1069, on voit bien que que la reception d'1 block fonctionne. J'avais pris ones1024. Recevoir les paquets était plus complexe à mon goût que d'écrire la requête.

![Capture d’écran du 2024-12-18 14-35-55](https://github.com/user-attachments/assets/c1b12105-35e5-4ef1-b598-6ac4100d6459)


Maintenant, pour plusieurs paquets, on rajoute un do while, on voit bien les différents block ![Capture d’écran du 2024-12-18 15-02-40](https://github.com/user-attachments/assets/102dbe88-832b-42ff-ad3b-7970add035f8)

En ayant ceci, nosu avons fini la 4.C

Maintenant, pour la requête en écriture (WRQ),
j'ai essayé de créer un fichier vide et d'écrire dedans mais je n'ai pas réussi à aboutir avec la requête demandé.


