# Uso de IA Generativa no Código

Este arquivo documenta o uso de IA generativa no desenvolvimento e na correção do código do projeto **Estrutura de Dados 2026**.

A IA foi utilizada como apoio para correção, organização, integração e documentação técnica do código-fonte. O uso ficou restrito ao suporte na codificação, depuração, estruturação dos benchmarks, criação de comentários técnicos e organização dos arquivos auxiliares.

---

## Objetivo do uso de IA

A IA foi usada para auxiliar nas seguintes tarefas:

- Análise do enunciado do projeto.
- Comparação entre os requisitos do PDF e o código existente.
- Correção e expansão do `main.cpp`.
- Integração do `Generator` e do `DataTreatment`.
- Implementação de estatísticas completas.
- Implementação de histograma.
- Implementação de ordenação por campo.
- Implementação de benchmarks.
- Implementação de testes restritivos.
- Separação metodológica entre busca exata e Bloom Filter probabilístico.
- Criação de comentários técnicos no código.
- Organização do `README.md`.
- Geração de arquivos CSV de saída.
- Verificação de compilação e execução.

---

## Arquivos modificados com assistência de IA

### `main.cpp`

Principais alterações:

- Reorganização do fluxo principal.
- Implementação do modo automático.
- Implementação do modo interativo.
- Uso real de `Generator`.
- Uso real de `DataTreatment`.
- Inserção dos registros nas estruturas.
- Execução de estatísticas, filtros, ordenação e anomalias.
- Execução dos benchmarks.
- Execução dos testes restritivos.
- Exportação de resultados em CSV.

---

### `include/AVL.h` e `src/AVL.cpp`

Principais alterações:

- Inclusão de método para estimativa de uso de memória.
- Inclusão de limite máximo opcional de elementos para o teste restritivo R2.
- Inclusão de comentários técnicos explicando papel, complexidade e justificativa da AVL.

---

### `include/HashTable.h` e `src/HashTable.cpp`

Principais alterações:

- Inclusão de método para estimativa de uso de memória.
- Organização de métricas de colisão.
- Inclusão de taxa de colisões.
- Inclusão de comentários técnicos explicando a estrutura, suas vantagens e suas limitações.

---

### `include/LinkedList.h` e `src/LinkedList.cpp`

Principais alterações:

- Inclusão de método para estimativa de uso de memória.
- Inclusão de comentários técnicos explicando o uso da lista encadeada como estrutura clássica e como comparação algorítmica.

---

### `include/SkipList.h` e `src/SkipList.cpp`

Principais alterações:

- Inclusão de método para estimativa de uso de memória.
- Inclusão de comentários técnicos explicando os níveis probabilísticos e o papel da Skip List no projeto.

---

### `include/BloomFilter.h` e `src/BloomFilter.cpp`

Principais alterações:

- Inclusão de justificativa sobre Counting Bloom Filter.
- Explicação de por que o Bloom Filter clássico não permite remoção segura.
- Separação conceitual entre Bloom Filter probabilístico e estruturas de busca exata.
- Inclusão de método para estimativa de uso de memória.

---

### `include/OptimizedHashTable.h` e `src/OptimizedHashTable.cpp`

Principais alterações:

- Inclusão de método para estimativa de uso de memória.
- Organização de métricas de colisão.
- Inclusão de comentários técnicos sobre a comparação com a HashTable original.

---

### `include/SensorAnalytics.h` e `src/SensorAnalytics.cpp`

Principais alterações:

- Implementação de estatísticas completas.
- Cálculo de média, mínimo, máximo e desvio padrão.
- Implementação de histograma ASCII de temperatura.
- Implementação de ordenação por campo.
- Impressão dos primeiros e últimos registros ordenados.

---

### `include/Visualization.h` e `src/Visualization.cpp`

Principais alterações:

- Organização das visualizações no terminal.
- Impressão de tabelas de benchmark.
- Comparação real vs teórica.
- Visualização dos principais resultados.

---

### `README.md`

Principais alterações:

- Organização das instruções de compilação.
- Organização das instruções de execução.
- Explicação do modo automático.
- Explicação do modo interativo.
- Descrição das estruturas e saídas geradas.

---

## Prompts utilizados

### Prompt principal de correção

O prompt principal usado solicitou que a IA corrigisse e expandisse o código do projeto de Estrutura de Dados 2026, trabalhando somente no código-fonte do repositório.

O prompt exigiu:

- Uso de C++17.
- Preservação das estruturas manuais.
- Uso de `Generator` e `DataTreatment`.
- Implementação de estatísticas completas.
- Implementação de ordenação.
- Benchmarks completos.
- Separação entre busca exata e Bloom Filter probabilístico.
- Testes restritivos R2, R7, R12, R16 e R21.
- Menu interativo.
- Exportação de CSVs.
- Comentários técnicos no código.
- Verificação com `make`.
- Execução automática com `./sistema`.
- Execução interativa com `./sistema --interativo`.

---

## Links das conversas

Adicionar aqui os links das conversas usadas, se disponíveis.

Exemplo:

```text
Conversa 1: [colar link aqui]
Conversa 2: [colar link aqui]
Conversa 3: [colar link aqui]
```

Caso não seja possível recuperar os links, registrar:

```text
Os links diretos das conversas não estavam disponíveis no momento da organização final do repositório. Os prompts utilizados foram documentados neste arquivo para fins de transparência.
```

---

## Observação importante

O uso de IA foi aplicado como suporte à codificação e organização técnica do projeto.

A escrita do relatório técnico final deve ser feita separadamente pelo aluno, respeitando as orientações do enunciado da disciplina.
