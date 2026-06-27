#!/bin/bash

# Valores padrão caso o usuário não passe nada
METODO=${1:-1}
QTD=${2:-1000}
SIT=${3:-3}

# Cria a pasta de logs se não existir
mkdir -p logs

LOGFILE="logs/metodo${METODO}_qtd${QTD}_sit${SIT}.log"

echo "====================================="
echo "Iniciando Teste -> Metodo: $METODO | Qtd: $QTD | Sit: $SIT"
echo "====================================="

echo "Executando ordenacao"
./ordena $METODO $QTD $SIT | tee $LOGFILE

echo "Ordenacao realizada"
echo "Verificando arquivo final"
./build/verifica $SIT $QTD | tee -a $LOGFILE
echo "Verificacao finalizada"
echo "Log salvo em: $LOGFILE"
echo "====================================="