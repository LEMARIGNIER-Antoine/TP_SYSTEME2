au départ, j'appelais gettftp.c depuis un main mais en fait, c'est un fichier à part et donc, il doit être changé directement dans le cmakeKist

Il faut bien regarder comment la requête RRq est construite et après, on additionne, les différents élements de la requête
![Capture d’écran du 2024-12-11 14-58-33](https://github.com/user-attachments/assets/85c80d41-e630-4330-87c1-f2eae7341d6a)

Sur wireshark, en vérifiant au port 1069, on voit bien que que la reception d'1 block fonctionne. J'avais pris ones1024.

![Capture d’écran du 2024-12-18 14-35-55](https://github.com/user-attachments/assets/c1b12105-35e5-4ef1-b598-6ac4100d6459)



