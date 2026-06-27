#!/bin/bash
METODO=${1:-1}

NUMMEDIA=5
TAMANHOS=(100 1000 10000 100000 471705)
SITUACOES=(1 2 3) # 1-Ascendente, 2-Descendente, 3-Aleatório

mkdir -p logs
LOGFILE="logs/benchmark_metodo${METODO}.log"

echo "==========================================================" | tee $LOGFILE
echo "BENCHMARK - MÉTODO $METODO ($NUMMEDIA execucoes)" | tee -a $LOGFILE
echo "==========================================================" | tee -a $LOGFILE

for sit in "${SITUACOES[@]}"; do
    if [ "$sit" == "1" ]; then sit_name="Crescente"; fi
    if [ "$sit" == "2" ]; then sit_name="Decrescente"; fi
    if [ "$sit" == "3" ]; then sit_name="Aleatório"; fi

    echo "" | tee -a $LOGFILE
    echo ">>> SITUAÇÃO: $sit_name <<<" | tee -a $LOGFILE

    # Laço dos Tamanhos
    for tam in "${TAMANHOS[@]}"; do
        echo "  -> Testando tamanho: $tam registros" | tee -a $LOGFILE

        soma_leit=0
        soma_esc=0
        soma_comp=0
        soma_tempo=0

        # Laço das repetições para a média
        for (( i=1; i<=NUMMEDIA; i++ )); do
            # Roda o programa e guarda a saída em uma variável
            saida=$(./ordena $METODO $tam $sit)

            # Usa grep e awk para extrair os números exatos dos seus prints
            leit=$(echo "$saida" | grep "transferencias Leitura:" | awk '{print $5}')
            esc=$(echo "$saida" | grep "transferencias Escrita:" | awk '{print $5}')
            comp=$(echo "$saida" | grep "comparacoes:" | awk '{print $4}')
            tempo=$(echo "$saida" | grep "Tempo de execucao:" | awk '{print $4}')

            leit=${leit:-0}
            esc=${esc:-0}
            comp=${comp:-0}
            tempo=${tempo:-0}
            soma_leit=$(awk "BEGIN {print $soma_leit + $leit}")
            soma_esc=$(awk "BEGIN {print $soma_esc + $esc}")
            soma_comp=$(awk "BEGIN {print $soma_comp + $comp}")
            soma_tempo=$(awk "BEGIN {print $soma_tempo + $tempo}")
        done

        med_leit=$(awk "BEGIN {printf \"%.0f\", $soma_leit / $NUMMEDIA}")
        med_esc=$(awk "BEGIN {printf \"%.0f\", $soma_esc / $NUMMEDIA}")
        med_comp=$(awk "BEGIN {printf \"%.0f\", $soma_comp / $NUMMEDIA}")
        med_tempo=$(awk "BEGIN {printf \"%.4f\", $soma_tempo / $NUMMEDIA}")

        # Imprime o resultado sumarizado e salva no log
        echo "Execucao concluida: Tempo: ${med_tempo}s | Leituras: $med_leit | Escritas: $med_esc | Comparacoes: $med_comp" | tee -a $LOGFILE
    done
    echo "----------------------------------------------------------" | tee -a $LOGFILE
done

echo "OK"
echo "Relatório salvo em: $LOGFILE"