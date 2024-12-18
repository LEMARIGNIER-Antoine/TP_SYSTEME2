#include <stdio.h>
#include "gettftp.h"
#include "puttftp.h"  // Ajouter l'en-tête pour la fonction puttftp

int main(int argc, char **argv) {
   gettftp(argc, argv);
   puttftp(argc, argv);
   return 0;
}
