# Sistema de Monitoramento Industrial de Sensores

Projeto desenvolvido para a disciplina **Estrutura de Dados 2026**.

O sistema simula um ambiente de monitoramento industrial em que sensores produzem leituras contínuas. Cada registro representa uma leitura de sensor com informações de identificação, tempo e variáveis físicas monitoradas.

Campos de cada leitura:

```cpp
int idSensor;
long long timestamp;
float temperatura;
float pressao;
float umidade;
float vibracao;
```

O objetivo do projeto é armazenar, consultar, remover, tratar, analisar e comparar essas leituras usando diferentes estruturas de dados implementadas manualmente em C++17.

---

## 1. Objetivo do projeto

O projeto tem como objetivo demonstrar o uso prático de estruturas de dados em um sistema simulado de monitoramento industrial.

O sistema permite:

- gerar um dataset sintético de leituras de sensores;
- tratar e validar os dados gerados;
- armazenar os registros em diferentes estruturas de dados;
- realizar inserção, busca e remoção;
- executar operações adicionais de análise;
- comparar desempenho entre estruturas;
- simular restrições de memória, processamento, latência, dados e algoritmo;
- exportar resultados para arquivos CSV.

---

## 2. Estruturas de dados implementadas

O projeto utiliza estruturas clássicas e estruturas adicionais.

### 2.1 Estruturas clássicas

- **LinkedList** — lista encadeada simples.
- **AVL** — árvore binária de busca balanceada.
- **HashTable** — tabela hash com tratamento de colisões.

### 2.2 Estruturas fora da ementa

- **SkipList** — estrutura probabilística com múltiplos níveis para busca eficiente.
- **BloomFilter** — filtro probabilístico de pertinência.

### 2.3 Estrutura otimizada

- **OptimizedHashTable** — versão otimizada da tabela hash original, usada para comparação direta com a `HashTable`.

---

## 3. Organização do projeto

Estrutura principal do repositório:

```text
.
├── main.cpp
├── Makefile
├── README.md
├── IA_USAGE.md
├── include/
│   ├── AVL.h
│   ├── AVLNode.h
│   ├── BloomFilter.h
│   ├── DataTreatment.h
│   ├── Generator.h
│   ├── HashNode.h
│   ├── HashTable.h
│   ├── LinkedList.h
│   ├── ListNode.h
│   ├── OptimizedHashTable.h
│   ├── SensorAnalytics.h
│   ├── SensorData.h
│   ├── SkipList.h
│   ├── SkipNode.h
│   └── Visualization.h
├── src/
│   ├── AVL.cpp
│   ├── BloomFilter.cpp
│   ├── DataTreatment.cpp
│   ├── Generator.cpp
│   ├── HashTable.cpp
│   ├── LinkedList.cpp
│   ├── OptimizedHashTable.cpp
│   ├── SensorAnalytics.cpp
│   ├── SkipList.cpp
│   ├── SkipNode.cpp
│   └── Visualization.cpp
└── output/
    ├── benchmark.csv
    ├── escalabilidade.csv
    └── restricoes.csv
```

---

## 4. Requisitos para execução

Para compilar e executar o projeto, é necessário ter:

- compilador C++ com suporte a C++17;
- `make`, preferencialmente;
- terminal compatível com comandos básicos.

Ambientes recomendados:

- Linux;
- GitHub Codespaces;
- MSYS2 UCRT64 no Windows;
- VS Code com compilador C++ configurado.

---

## 5. Compilação

### 5.1 Compilação com Makefile

Na pasta raiz do projeto, execute:

```bash
make clean
make
```

Se a compilação for concluída corretamente, será gerado o executável:

```text
sistema
```

No Windows, dependendo do ambiente, o executável pode aparecer como:

```text
sistema.exe
```

### 5.2 Compilação manual

Caso o `make` não esteja disponível, compile manualmente com:

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude main.cpp src/Generator.cpp src/DataTreatment.cpp src/AVL.cpp src/HashTable.cpp src/LinkedList.cpp src/SkipNode.cpp src/SkipList.cpp src/BloomFilter.cpp src/OptimizedHashTable.cpp src/SensorAnalytics.cpp src/Visualization.cpp -o sistema
```

---

## 6. Execução

### 6.1 Modo automático

Para executar todos os testes automaticamente:

```bash
./sistema
```

No Windows:

```bash
./sistema.exe
```

O modo automático executa:

1. geração do dataset;
2. tratamento dos dados;
3. inserção nas estruturas;
4. validação de busca e remoção;
5. estatísticas completas;
6. histograma de temperatura;
7. detecção de anomalias;
8. filtro por intervalo;
9. ordenação por campo;
10. benchmark de estruturas de busca exata;
11. benchmark do Bloom Filter;
12. comparação entre HashTable e OptimizedHashTable;
13. testes restritivos;
14. benchmark de escalabilidade;
15. comparação entre crescimento real e teórico;
16. exportação de CSVs.

Ao final da execução, a saída esperada é:

```text
============================================================
SISTEMA EXECUTADO COM SUCESSO
VALIDACOES PRINCIPAIS CONCLUIDAS
============================================================
```

### 6.2 Modo interativo

Para abrir o menu interativo:

```bash
./sistema --interativo
```

No Windows:

```bash
./sistema.exe --interativo
```

O modo interativo permite escolher operações manualmente pelo terminal.

Menu disponível:

```text
[1]  Inserir novo registro manualmente
[2]  Buscar por timestamp
[3]  Remover por timestamp
[4]  Exibir estatisticas completas
[5]  Relatorio de anomalias
[6]  Filtrar por intervalo de timestamp
[7]  Ordenar dataset por campo
[8]  Executar benchmark completo
[9]  Executar testes restritivos
[10] Visualizar estrutura
[11] Comparar Hash original vs Hash otimizada
[12] Benchmark de escalabilidade
[0]  Sair
```

---

## 7. Dataset

O dataset é sintético e representa leituras de sensores industriais.

A geração usa seed fixa para garantir reprodutibilidade:

```cpp
Generator gen(42);
```

O sistema gera pelo menos:

```text
10000 registros
```

Cada registro possui:

- identificador do sensor;
- timestamp único;
- temperatura;
- pressão;
- umidade;
- vibração.

Os dados passam por tratamento antes de serem inseridos nas estruturas.

---

## 8. Tratamento dos dados

O tratamento dos dados é realizado pelo módulo `DataTreatment`.

Ele é responsável por:

- normalizar registros;
- validar leituras;
- impedir que dados inválidos sejam inseridos nas estruturas principais.

Durante a execução, o sistema informa:

- quantidade de registros gerados;
- quantidade de registros válidos;
- quantidade de registros normalizados;
- quantidade de registros descartados.

---

## 9. Operações adicionais implementadas

Além de inserir, buscar e remover dados, o sistema executa operações adicionais de análise.

### 9.1 Estatísticas

O sistema calcula, para temperatura, pressão, umidade e vibração:

- média;
- desvio padrão;
- valor mínimo;
- valor máximo.

### 9.2 Histograma

O sistema gera um histograma ASCII da temperatura, dividido em faixas de 2 °C.

### 9.3 Detecção de anomalias

O sistema identifica leituras anômalas com base nos valores dos sensores.

### 9.4 Filtragem

É possível filtrar registros por intervalo de timestamp.

### 9.5 Ordenação

O sistema permite ordenar o dataset por:

- timestamp;
- temperatura;
- pressão;
- umidade;
- vibração;
- idSensor.

---

## 10. Benchmarks

O projeto executa benchmarks comparativos com diferentes métricas.

### 10.1 Benchmark A — estruturas de busca exata

Este benchmark compara:

- AVL;
- HashTable;
- LinkedList;
- SkipList;
- OptimizedHashTable.

Métricas medidas:

- tempo de inserção;
- tempo total de busca;
- tempo médio de consulta;
- tempo de remoção;
- latência média combinada;
- uso estimado de memória;
- número de colisões, quando aplicável;
- taxa de colisões, quando aplicável.

### 10.2 Benchmark B — Bloom Filter

O Bloom Filter é analisado separadamente porque é uma estrutura probabilística.

Ele pode indicar falso positivo, mas não deve indicar falso negativo quando usado corretamente.

Por isso, seus resultados não são comparados diretamente com as estruturas de busca exata.

---

## 11. Estrutura otimizada

O projeto compara a tabela hash original com uma versão otimizada:

```text
HashTable vs OptimizedHashTable
```

A comparação inclui:

- inserção;
- busca;
- remoção;
- colisões;
- taxa de colisões;
- uso de memória;
- latência média.

Essa comparação atende à exigência de otimizar ao menos uma estrutura de dados e compará-la com sua versão original.

---

## 12. Testes restritivos

O sistema implementa cinco testes restritivos, cobrindo diferentes categorias.

### R2 — Restrição de memória

A AVL é limitada a 500 elementos.

Objetivo: simular uma estrutura com limite máximo de armazenamento.

### R7 — Restrição de processamento

Cada busca possui um orçamento máximo de tempo.

Objetivo: verificar quais estruturas violam mais vezes o limite definido.

### R12 — Restrição de latência

Parte das consultas recebe atraso artificial.

Objetivo: simular acesso externo lento, como rede ou banco de dados.

### R16 — Restrição de dados

20% dos registros são corrompidos artificialmente.

Objetivo: verificar o impacto de leituras inválidas e a importância do tratamento de dados.

### R21 — Restrição algorítmica

A busca em AVL é comparada com a busca em LinkedList.

Objetivo: demonstrar a diferença entre busca aproximadamente logarítmica e busca linear.

---

## 13. Escalabilidade

O benchmark de escalabilidade executa testes com diferentes tamanhos de entrada:

```text
1000
5000
10000
50000
```

O objetivo é observar como o tempo de execução cresce conforme o volume de dados aumenta.

As estruturas analisadas são:

- AVL;
- HashTable;
- LinkedList;
- SkipList;
- OptimizedHashTable.

---

## 14. Comparação real vs teórica

O sistema compara o crescimento observado nos testes de escalabilidade com o crescimento esperado pela teoria.

Essa comparação não é uma prova formal de complexidade, mas ajuda a verificar se o comportamento prático está coerente com a análise teórica das estruturas.

---

## 15. Visualização no terminal

O sistema apresenta visualizações diretamente no terminal, incluindo:

- tabelas de benchmark;
- ranking de busca mais rápida;
- ranking de menor uso de memória;
- histograma de temperatura;
- resumo das estruturas;
- comparação entre hash original e hash otimizada.

---

## 16. Arquivos de saída

Durante a execução, o programa cria ou atualiza a pasta:

```text
output/
```

Arquivos gerados:

```text
output/benchmark.csv
output/escalabilidade.csv
output/restricoes.csv
```

Esses arquivos podem ser usados para montar tabelas e gráficos no relatório técnico.

---

## 17. Uso de IA generativa

O uso de IA generativa no desenvolvimento do código está documentado no arquivo:

```text
IA_USAGE.md
```

Esse arquivo descreve quais partes do código foram desenvolvidas, corrigidas ou organizadas com assistência de IA.

---

## 18. Como testar rapidamente

### 18.1 Teste automático

```bash
make clean
make
./sistema
```

Resultado esperado:

```text
SISTEMA EXECUTADO COM SUCESSO
VALIDACOES PRINCIPAIS CONCLUIDAS
```

### 18.2 Teste interativo

```bash
./sistema --interativo
```

Digite:

```text
4
```

para visualizar estatísticas.

Digite:

```text
0
```

para sair.

---

## 19. Observações finais

- O código foi escrito em C++17.
- As estruturas principais foram implementadas manualmente.
- O Bloom Filter é tratado separadamente por ser probabilístico.
- O dataset sintético permite execução reprodutível.
- Os arquivos CSV gerados auxiliam na construção de tabelas e gráficos para análise posterior.
