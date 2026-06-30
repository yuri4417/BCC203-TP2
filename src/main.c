#include <sys/stat.h>
#include "struct.h"
#include "Executavel.h"


int main(int argc, char *argv[]) {
    struct stat st = {0};
    if (stat("./data", &st) == -1)
       MKDIR("./data");

    Config cfg = {0};
    Bench bench = {0};

    //Valida os argumentos de entrada
    if(!valida(argc,argv,&cfg))
        return -1;

    //Executa o programa de ordenacao
    executar(&cfg,&bench);
    return 0;
}