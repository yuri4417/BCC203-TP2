#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "struct.h"
#include "arquivos.h"
#include "Executavel.h"
int main(int argc, char *argv[]) {
    struct stat st = {0};
    if (stat("./data", &st) == -1)
        mkdir("./data", 0777);

    Config cfg;
    Bench bench;
    if(!valida(argc,argv,&cfg))
        return -1;
    executar(&cfg,&bench);
    return 0;
}