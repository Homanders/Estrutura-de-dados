#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>

#include "AVL.h"
#include "BloomFilter.h"
#include "DataTreatment.h"
#include "Generator.h"
#include "HashTable.h"
#include "LinkedList.h"
#include "OptimizedHashTable.h"
#include "SensorAnalytics.h"
#include "SkipList.h"
#include "Visualization.h"

using namespace std;
using namespace std::chrono;

struct BenchmarkResult
{
    string nome;
    double insercaoMs = 0.0;
    double buscaMs = 0.0;
    double buscaMediaUs = 0.0;
    double remocaoMs = 0.0;
    double latenciaMediaUs = 0.0;
    double memoriaKb = 0.0;
    long long colisoes = -1;
    double taxaColisao = -1.0;
    int encontrados = 0;
    int removidos = 0;
};

struct EscalaResult
{
    int tamanho = 0;
    double avlMs = 0.0;
    double hashMs = 0.0;
    double listaMs = 0.0;
    double skipMs = 0.0;
    double hashOptMs = 0.0;
};

struct ProjetoContexto
{
    SensorData* dados = nullptr;
    int quantidade = 0;
    int gerados = 0;
    int validos = 0;
    int normalizados = 0;
    int descartados = 0;

    AVL avl;
    HashTable hash;
    LinkedList lista;
    SkipList skip;
    BloomFilter bloom;
    OptimizedHashTable hashOpt;

    ProjetoContexto()
        : hash(20011), bloom(100003, 4), hashOpt(101, 0.75)
    {
    }
};

template <typename Funcao>
static double medirMsLambda(Funcao funcao)
{
    auto inicio = high_resolution_clock::now();
    funcao();
    auto fim = high_resolution_clock::now();
    return duration<double, milli>(fim - inicio).count();
}

static void linha(const string& titulo)
{
    cout << "\n============================================================\n";
    cout << titulo << "\n";
    cout << "============================================================\n";
}

static void garantirPastaOutput()
{
#ifdef _WIN32
    system("if not exist output mkdir output >nul 2>nul");
#else
    system("mkdir -p output");
#endif
}

static void imprimirRegistro(const SensorData& s)
{
    cout << "ID=" << s.idSensor
         << " | TS=" << s.timestamp
         << " | T=" << fixed << setprecision(2) << s.temperatura
         << " | P=" << s.pressao
         << " | U=" << s.umidade
         << " | V=" << s.vibracao << "\n";
}

// [IA-GENERATED] Trecho implementado com assistência de IA. Ver IA_USAGE.md para detalhes.
static SensorData* gerarDataset(int quantidade,
                                int& gerados,
                                int& validos,
                                int& normalizados,
                                int& descartados,
                                bool imprimir)
{
    /*
     * O seed fixo 42 torna os experimentos reprodutiveis.
     * A normalizacao reduz discrepancias de escala entre campos.
     * A validacao impede que registros invalidos contaminem benchmarks e estatisticas.
     */
    Generator gen(42);
    DataTreatment tratamento;

    SensorData* dados = new SensorData[quantidade];

    gerados = quantidade;
    validos = 0;
    normalizados = 0;
    descartados = 0;

    if(imprimir)
    {
        linha("GERACAO DO DATASET");
        cout << "Gerando " << quantidade
             << " registros com distribuicao uniforme (seed: 42)...\n";
    }

    for(int i = 0; i < quantidade; i++)
    {
        SensorData sensor = gen.generateReading(i + 1);
        sensor.timestamp = 1000000000LL + i;

        SensorData normalizado = tratamento.normalize(sensor);
        normalizados++;

        if(tratamento.isValid(normalizado))
        {
            dados[validos] = normalizado;
            validos++;
        }
        else
        {
            descartados++;
        }
    }

    if(imprimir)
    {
        cout << "Registros gerados      : " << gerados << "\n";
        cout << "Registros validos      : " << validos << "\n";
        cout << "Registros normalizados : " << normalizados << "\n";
        cout << "Registros descartados  : " << descartados << "\n";
        cout << "====================================================\n";
    }

    return dados;
}

static void inserirEmTodas(ProjetoContexto& ctx)
{
    for(int i = 0; i < ctx.quantidade; i++)
    {
        ctx.avl.insert(ctx.dados[i]);
        ctx.hash.insert(ctx.dados[i]);
        ctx.lista.pushBack(ctx.dados[i]);
        ctx.skip.insert(ctx.dados[i]);
        ctx.bloom.insert(ctx.dados[i].timestamp);
        ctx.hashOpt.insert(ctx.dados[i]);
    }
}

static long long chaveBusca(const SensorData* dados, int quantidade, int i)
{
    return dados[(i * 7) % quantidade].timestamp;
}

static long long chaveRemocao(const SensorData* dados, int quantidade, int i)
{
    return dados[(i * 11) % quantidade].timestamp;
}

static BenchmarkResult benchmarkAVL(SensorData* dados, int quantidade, int buscas, int remocoes)
{
    BenchmarkResult r;
    r.nome = "AVL";
    AVL avl;

    r.insercaoMs = medirMsLambda([&]()
    {
        for(int i = 0; i < quantidade; i++)
            avl.insert(dados[i]);
    });

    r.buscaMs = medirMsLambda([&]()
    {
        for(int i = 0; i < buscas; i++)
            if(avl.contains(chaveBusca(dados, quantidade, i)))
                r.encontrados++;
    });

    r.remocaoMs = medirMsLambda([&]()
    {
        for(int i = 0; i < remocoes; i++)
        {
            long long chave = chaveRemocao(dados, quantidade, i);
            if(avl.contains(chave))
            {
                avl.remove(chave);
                r.removidos++;
            }
        }
    });

    r.memoriaKb = avl.memoryUsage() / 1024.0;
    r.buscaMediaUs = (r.buscaMs * 1000.0) / buscas;
    r.latenciaMediaUs = ((r.insercaoMs + r.buscaMs + r.remocaoMs) * 1000.0) /
                        (quantidade + buscas + remocoes);
    return r;
}

static BenchmarkResult benchmarkHash(SensorData* dados, int quantidade, int buscas, int remocoes)
{
    BenchmarkResult r;
    r.nome = "HashTable";
    HashTable hash(20011);

    r.insercaoMs = medirMsLambda([&]()
    {
        for(int i = 0; i < quantidade; i++)
            hash.insert(dados[i]);
    });

    r.buscaMs = medirMsLambda([&]()
    {
        for(int i = 0; i < buscas; i++)
            if(hash.contains(chaveBusca(dados, quantidade, i)))
                r.encontrados++;
    });

    r.remocaoMs = medirMsLambda([&]()
    {
        for(int i = 0; i < remocoes; i++)
            if(hash.remove(chaveRemocao(dados, quantidade, i)))
                r.removidos++;
    });

    r.memoriaKb = hash.memoryUsage() / 1024.0;
    r.colisoes = hash.collisions();
    r.taxaColisao = hash.collisionRate();
    r.buscaMediaUs = (r.buscaMs * 1000.0) / buscas;
    r.latenciaMediaUs = ((r.insercaoMs + r.buscaMs + r.remocaoMs) * 1000.0) /
                        (quantidade + buscas + remocoes);
    return r;
}

static BenchmarkResult benchmarkLista(SensorData* dados, int quantidade, int buscas, int remocoes)
{
    BenchmarkResult r;
    r.nome = "LinkedList";
    LinkedList lista;

    r.insercaoMs = medirMsLambda([&]()
    {
        for(int i = 0; i < quantidade; i++)
            lista.pushBack(dados[i]);
    });

    r.buscaMs = medirMsLambda([&]()
    {
        for(int i = 0; i < buscas; i++)
            if(lista.contains(chaveBusca(dados, quantidade, i)))
                r.encontrados++;
    });

    r.remocaoMs = medirMsLambda([&]()
    {
        for(int i = 0; i < remocoes; i++)
            if(lista.remove(chaveRemocao(dados, quantidade, i)))
                r.removidos++;
    });

    r.memoriaKb = lista.memoryUsage() / 1024.0;
    r.buscaMediaUs = (r.buscaMs * 1000.0) / buscas;
    r.latenciaMediaUs = ((r.insercaoMs + r.buscaMs + r.remocaoMs) * 1000.0) /
                        (quantidade + buscas + remocoes);
    return r;
}

static BenchmarkResult benchmarkSkip(SensorData* dados, int quantidade, int buscas, int remocoes)
{
    BenchmarkResult r;
    r.nome = "SkipList";
    SkipList skip;

    r.insercaoMs = medirMsLambda([&]()
    {
        for(int i = 0; i < quantidade; i++)
            skip.insert(dados[i]);
    });

    r.buscaMs = medirMsLambda([&]()
    {
        for(int i = 0; i < buscas; i++)
            if(skip.contains(chaveBusca(dados, quantidade, i)))
                r.encontrados++;
    });

    r.remocaoMs = medirMsLambda([&]()
    {
        for(int i = 0; i < remocoes; i++)
            if(skip.remove(chaveRemocao(dados, quantidade, i)))
                r.removidos++;
    });

    r.memoriaKb = skip.memoryUsage() / 1024.0;
    r.buscaMediaUs = (r.buscaMs * 1000.0) / buscas;
    r.latenciaMediaUs = ((r.insercaoMs + r.buscaMs + r.remocaoMs) * 1000.0) /
                        (quantidade + buscas + remocoes);
    return r;
}

static BenchmarkResult benchmarkHashOpt(SensorData* dados, int quantidade, int buscas, int remocoes)
{
    BenchmarkResult r;
    r.nome = "OptimizedHash";
    OptimizedHashTable hash(101, 0.75);

    r.insercaoMs = medirMsLambda([&]()
    {
        for(int i = 0; i < quantidade; i++)
            hash.insert(dados[i]);
    });

    r.buscaMs = medirMsLambda([&]()
    {
        for(int i = 0; i < buscas; i++)
            if(hash.contains(chaveBusca(dados, quantidade, i)))
                r.encontrados++;
    });

    r.remocaoMs = medirMsLambda([&]()
    {
        for(int i = 0; i < remocoes; i++)
            if(hash.remove(chaveRemocao(dados, quantidade, i)))
                r.removidos++;
    });

    r.memoriaKb = hash.memoryUsage() / 1024.0;
    r.colisoes = hash.collisions();
    r.taxaColisao = hash.collisionRate();
    r.buscaMediaUs = (r.buscaMs * 1000.0) / buscas;
    r.latenciaMediaUs = ((r.insercaoMs + r.buscaMs + r.remocaoMs) * 1000.0) /
                        (quantidade + buscas + remocoes);
    return r;
}

// [IA-GENERATED] Trecho implementado com assistência de IA. Ver IA_USAGE.md para detalhes.
static BenchmarkResult benchmarkBloom(SensorData* dados, int quantidade, int buscas, int remocoes)
{
    BenchmarkResult r;
    r.nome = "BloomFilter";
    BloomFilter bloom(100003, 4);

    r.insercaoMs = medirMsLambda([&]()
    {
        for(int i = 0; i < quantidade; i++)
            bloom.insert(dados[i].timestamp);
    });

    r.buscaMs = medirMsLambda([&]()
    {
        for(int i = 0; i < buscas; i++)
            if(bloom.contains(chaveBusca(dados, quantidade, i)))
                r.encontrados++;
    });

    r.remocaoMs = medirMsLambda([&]()
    {
        for(int i = 0; i < remocoes; i++)
            if(bloom.remove(chaveRemocao(dados, quantidade, i)))
                r.removidos++;
    });

    int falsosPositivos = 0;
    for(int i = 0; i < buscas; i++)
    {
        long ausente = static_cast<long>(2000000000LL + i);
        if(bloom.contains(ausente))
            falsosPositivos++;
    }

    r.memoriaKb = bloom.memoryUsage() / 1024.0;
    r.colisoes = bloom.collisions();
    r.taxaColisao = bloom.collisionRate();
    r.buscaMediaUs = (r.buscaMs * 1000.0) / buscas;
    r.latenciaMediaUs = ((r.insercaoMs + r.buscaMs + r.remocaoMs) * 1000.0) /
                        (quantidade + buscas + remocoes);

    cout << "Falsos positivos observados no Bloom Filter: "
         << falsosPositivos << " de " << buscas << " consultas ausentes.\n";
    cout << "Taxa estimada teorica de falso positivo: "
         << fixed << setprecision(4) << bloom.estimatedFalsePositiveRate() * 100.0 << "%\n";

    return r;
}

// [IA-GENERATED] Trecho implementado com assistência de IA. Ver IA_USAGE.md para detalhes.
static void imprimirBenchmarkExato(const BenchmarkResult resultados[], int n)
{
    /*
     * BENCHMARK PRINCIPAL
     *
     * Este benchmark mede operacoes basicas exigidas pelo projeto:
     * insercao, busca, remocao, tempo medio de consulta, latencia media,
     * uso estimado de memoria e colisoes.
     *
     * A comparacao e separada em dois grupos:
     *
     * 1. Estruturas de busca exata:
     *    AVL, HashTable, LinkedList, SkipList e OptimizedHashTable.
     *
     * 2. Estrutura probabilistica:
     *    BloomFilter.
     *
     * A separacao e necessaria porque Bloom Filter nao responde busca exata;
     * ele responde pertinencia probabilistica, podendo apresentar falsos positivos.
     */
    linha("BENCHMARK A - BUSCA EXATA");
    cout << left << setw(17) << "Estrutura"
         << right << setw(12) << "Ins(ms)"
         << setw(12) << "Busca(ms)"
         << setw(14) << "Media(us)"
         << setw(12) << "Rem(ms)"
         << setw(14) << "Lat(us)"
         << setw(13) << "Mem(KB)"
         << setw(12) << "Colisoes" << "\n";
    cout << "------------------------------------------------------------------------------------------------\n";

    for(int i = 0; i < n; i++)
    {
        cout << left << setw(17) << resultados[i].nome
             << right << setw(12) << fixed << setprecision(3) << resultados[i].insercaoMs
             << setw(12) << resultados[i].buscaMs
             << setw(14) << resultados[i].buscaMediaUs
             << setw(12) << resultados[i].remocaoMs
             << setw(14) << resultados[i].latenciaMediaUs
             << setw(13) << resultados[i].memoriaKb;

        if(resultados[i].colisoes < 0)
            cout << setw(12) << "N/A";
        else
            cout << setw(12) << resultados[i].colisoes;

        cout << "\n";
    }
}

static void imprimirBenchmarkBloom(const BenchmarkResult& r)
{
    linha("BENCHMARK B - BLOOM FILTER PROBABILISTICO");
    cout << "Bloom Filter e probabilistico: pode retornar falso positivo, mas nao falso negativo quando usado corretamente.\n\n";
    cout << left << setw(18) << "Estrutura"
         << right << setw(12) << "Ins(ms)"
         << setw(12) << "Cons(ms)"
         << setw(14) << "Media(us)"
         << setw(12) << "Rem(ms)"
         << setw(14) << "Mem(KB)"
         << setw(12) << "Colisoes" << "\n";
    cout << "--------------------------------------------------------------------------------\n";
    cout << left << setw(18) << r.nome
         << right << setw(12) << fixed << setprecision(3) << r.insercaoMs
         << setw(12) << r.buscaMs
         << setw(14) << r.buscaMediaUs
         << setw(12) << r.remocaoMs
         << setw(14) << r.memoriaKb
         << setw(12) << r.colisoes << "\n";
}

static void exportarBenchmark(const BenchmarkResult resultados[], int n, const BenchmarkResult& bloom)
{
    garantirPastaOutput();
    ofstream out("output/benchmark.csv");
    out << "estrutura,insercao_ms,busca_ms,busca_media_us,remocao_ms,latencia_us,memoria_kb,colisoes,taxa_colisao_percentual\n";

    for(int i = 0; i < n; i++)
    {
        out << resultados[i].nome << ','
            << resultados[i].insercaoMs << ','
            << resultados[i].buscaMs << ','
            << resultados[i].buscaMediaUs << ','
            << resultados[i].remocaoMs << ','
            << resultados[i].latenciaMediaUs << ','
            << resultados[i].memoriaKb << ','
            << resultados[i].colisoes << ','
            << resultados[i].taxaColisao << '\n';
    }

    out << bloom.nome << ','
        << bloom.insercaoMs << ','
        << bloom.buscaMs << ','
        << bloom.buscaMediaUs << ','
        << bloom.remocaoMs << ','
        << bloom.latenciaMediaUs << ','
        << bloom.memoriaKb << ','
        << bloom.colisoes << ','
        << bloom.taxaColisao << '\n';
}

// [IA-GENERATED] Trecho implementado com assistência de IA. Ver IA_USAGE.md para detalhes.
static void executarBenchmarkCompleto(SensorData* dados,
                                      int quantidade,
                                      BenchmarkResult resultados[5],
                                      BenchmarkResult& bloom)
{
    const int qtdBuscas = 1000;
    const int qtdRemocoes = 1000;

    resultados[0] = benchmarkAVL(dados, quantidade, qtdBuscas, qtdRemocoes);
    resultados[1] = benchmarkHash(dados, quantidade, qtdBuscas, qtdRemocoes);
    resultados[2] = benchmarkLista(dados, quantidade, qtdBuscas, qtdRemocoes);
    resultados[3] = benchmarkSkip(dados, quantidade, qtdBuscas, qtdRemocoes);
    resultados[4] = benchmarkHashOpt(dados, quantidade, qtdBuscas, qtdRemocoes);

    imprimirBenchmarkExato(resultados, 5);

    bloom = benchmarkBloom(dados, quantidade, qtdBuscas, qtdRemocoes);
    imprimirBenchmarkBloom(bloom);

    exportarBenchmark(resultados, 5, bloom);
}

static EscalaResult medirEscala(int tamanho)
{
    int gerados = 0, validos = 0, normalizados = 0, descartados = 0;
    SensorData* dados = gerarDataset(tamanho, gerados, validos, normalizados, descartados, false);
    const int buscas = 1000;

    EscalaResult r;
    r.tamanho = tamanho;

    r.avlMs = medirMsLambda([&]()
    {
        AVL avl;
        for(int i = 0; i < validos; i++)
            avl.insert(dados[i]);
        for(int i = 0; i < buscas; i++)
            avl.contains(chaveBusca(dados, validos, i));
    });

    r.hashMs = medirMsLambda([&]()
    {
        HashTable hash(tamanho * 2 + 11);
        for(int i = 0; i < validos; i++)
            hash.insert(dados[i]);
        for(int i = 0; i < buscas; i++)
            hash.contains(chaveBusca(dados, validos, i));
    });

    r.listaMs = medirMsLambda([&]()
    {
        LinkedList lista;
        for(int i = 0; i < validos; i++)
            lista.pushBack(dados[i]);
        for(int i = 0; i < buscas; i++)
            lista.contains(chaveBusca(dados, validos, i));
    });

    r.skipMs = medirMsLambda([&]()
    {
        SkipList skip;
        for(int i = 0; i < validos; i++)
            skip.insert(dados[i]);
        for(int i = 0; i < buscas; i++)
            skip.contains(chaveBusca(dados, validos, i));
    });

    r.hashOptMs = medirMsLambda([&]()
    {
        OptimizedHashTable hash(101, 0.75);
        for(int i = 0; i < validos; i++)
            hash.insert(dados[i]);
        for(int i = 0; i < buscas; i++)
            hash.contains(chaveBusca(dados, validos, i));
    });

    delete[] dados;
    return r;
}

static void exportarEscalabilidade(const EscalaResult resultados[], int n)
{
    garantirPastaOutput();
    ofstream out("output/escalabilidade.csv");
    out << "tamanho,avl_ms,hash_ms,linkedlist_ms,skiplist_ms,optimized_hash_ms\n";
    for(int i = 0; i < n; i++)
    {
        out << resultados[i].tamanho << ','
            << resultados[i].avlMs << ','
            << resultados[i].hashMs << ','
            << resultados[i].listaMs << ','
            << resultados[i].skipMs << ','
            << resultados[i].hashOptMs << '\n';
    }
}

static void executarEscalabilidade(EscalaResult resultados[4], Visualization& vis)
{
    /*
     * O teste de escalabilidade mostra como o desempenho muda quando o volume
     * de dados aumenta. Isso permite comparar o comportamento observado com
     * a complexidade teorica esperada para cada estrutura.
     */
    const int tamanhos[4] = {1000, 5000, 10000, 50000};

    for(int i = 0; i < 4; i++)
        resultados[i] = medirEscala(tamanhos[i]);

    linha("BENCHMARK DE ESCALABILIDADE");
    cout << right << setw(8) << "Tamanho"
         << setw(12) << "AVL(ms)"
         << setw(12) << "Hash(ms)"
         << setw(13) << "Lista(ms)"
         << setw(12) << "Skip(ms)"
         << setw(15) << "HashOpt(ms)" << "\n";
    cout << "----------------------------------------------------------------------------\n";

    for(int i = 0; i < 4; i++)
    {
        cout << right << setw(8) << resultados[i].tamanho
             << setw(12) << fixed << setprecision(3) << resultados[i].avlMs
             << setw(12) << resultados[i].hashMs
             << setw(13) << resultados[i].listaMs
             << setw(12) << resultados[i].skipMs
             << setw(15) << resultados[i].hashOptMs << "\n";
    }

    cout << "\nObservacao: AVL e Skip List tendem a O(n log n) no total. "
         << "Hash tende a O(n) medio. Lista degrada em buscas lineares.\n";

    exportarEscalabilidade(resultados, 4);

    string labels[5] = {"AVL", "HashTable", "LinkedList", "SkipList", "OptimizedHash"};
    string complexidades[5] = {"O(n log n)", "O(n)", "O(n^2) busca", "O(n log n)", "O(n)"};
    double observado[5] = {
        resultados[3].avlMs / resultados[2].avlMs,
        resultados[3].hashMs / resultados[2].hashMs,
        resultados[3].listaMs / resultados[2].listaMs,
        resultados[3].skipMs / resultados[2].skipMs,
        resultados[3].hashOptMs / resultados[2].hashOptMs
    };
    double teorico[5] = {5.60, 5.00, 25.00, 5.60, 5.00};
    vis.printComplexityComparison(labels, complexidades, observado, teorico, 5);
}

static void exportarRestricoes(const string linhas[], int n)
{
    garantirPastaOutput();
    ofstream out("output/restricoes.csv");
    out << "teste,resultado\n";
    for(int i = 0; i < n; i++)
        out << linhas[i] << '\n';
}

/*
 * TESTES EM CONDICOES RESTRITIVAS
 *
 * Estes testes simulam restricoes exigidas pelo projeto:
 *
 * R2  - Memoria: limite maximo de elementos na AVL.
 * R7  - Processamento: orcamento maximo por operacao de busca.
 * R12 - Latencia: atraso artificial em parte das consultas.
 * R16 - Dados: corrupcao de 20% das leituras.
 * R21 - Algoritmica: substituicao de AVL por LinkedList.
 *
 * Cada teste cobre uma categoria diferente: memoria, processamento,
 * latencia, dados e estrutura algoritmica.
 */
// [IA-GENERATED] Trecho implementado com assistência de IA. Ver IA_USAGE.md para detalhes.
static void executarTestesRestritivos(SensorData* dados, int quantidade)
{
    linha("TESTES EM CONDICOES RESTRITIVAS");

    string csv[10];
    int csvCount = 0;

    /* R2 simula memoria limitada rejeitando insercoes apos 500 elementos. */
    AVL avlRestrita;
    avlRestrita.setMaxSize(500);
    for(int i = 0; i < quantidade; i++)
        avlRestrita.insert(dados[i]);

    AVL avlCompleta;
    for(int i = 0; i < quantidade; i++)
        avlCompleta.insert(dados[i]);

    double buscaRestrita = medirMsLambda([&]()
    {
        for(int i = 0; i < 1000; i++)
            avlRestrita.contains(chaveBusca(dados, quantidade, i));
    });

    double buscaCompleta = medirMsLambda([&]()
    {
        for(int i = 0; i < 1000; i++)
            avlCompleta.contains(chaveBusca(dados, quantidade, i));
    });

    cout << "\n[R2 - Restricao de Memoria] AVL limitada a 500 elementos\n";
    cout << "  Insercoes tentadas   : " << quantidade << "\n";
    cout << "  Insercoes aceitas    : " << avlRestrita.size() << "\n";
    cout << "  Insercoes rejeitadas : " << quantidade - avlRestrita.size() << "\n";
    cout << "  Busca AVL restrita   : " << fixed << setprecision(3) << buscaRestrita << " ms\n";
    cout << "  Busca AVL completa   : " << buscaCompleta << " ms\n";
    csv[csvCount++] = "R2," + to_string(quantidade - avlRestrita.size()) + " rejeicoes";

    /* R7 mede violacoes de um orcamento artificial de 0.1 ms por busca. */
    cout << "\n[R7 - Restricao de Processamento] Budget de 0.1 ms por busca\n";
    const double limiteMs = 0.1;
    const int buscas = 1000;

    auto contarViolacoes = [&](const string& nome, auto containsFunc)
    {
        int violacoes = 0;
        for(int i = 0; i < buscas; i++)
        {
            long long chave = chaveBusca(dados, quantidade, i);
            double tempo = medirMsLambda([&]() { containsFunc(chave); });
            if(tempo > limiteMs)
                violacoes++;
        }

        cout << "  " << left << setw(18) << nome << ": "
             << right << setw(4) << violacoes << " violacoes de " << buscas
             << " buscas (" << fixed << setprecision(2)
             << (100.0 * violacoes) / buscas << "%)\n";
    };

    HashTable hash(20011);
    LinkedList lista;
    SkipList skip;
    OptimizedHashTable hashOpt(101, 0.75);
    for(int i = 0; i < quantidade; i++)
    {
        hash.insert(dados[i]);
        lista.pushBack(dados[i]);
        skip.insert(dados[i]);
        hashOpt.insert(dados[i]);
    }

    contarViolacoes("AVL", [&](long long c) { return avlCompleta.contains(c); });
    contarViolacoes("HashTable", [&](long long c) { return hash.contains(c); });
    contarViolacoes("LinkedList", [&](long long c) { return lista.contains(c); });
    contarViolacoes("SkipList", [&](long long c) { return skip.contains(c); });
    contarViolacoes("OptimizedHash", [&](long long c) { return hashOpt.contains(c); });
    csv[csvCount++] = "R7,buscas avaliadas";

    /* R12 simula latencia externa com atraso pequeno para manter o modo automatico rapido. */
    cout << "\n[R12 - Restricao de Latencia] Acesso externo lento simulado\n";
    double semLatencia = medirMsLambda([&]()
    {
        for(int i = 0; i < 100; i++)
            hash.contains(chaveBusca(dados, quantidade, i));
    });

    double comLatencia = medirMsLambda([&]()
    {
        for(int i = 0; i < 100; i++)
        {
            if(i < 20)
                this_thread::sleep_for(milliseconds(5));
            hash.contains(chaveBusca(dados, quantidade, i));
        }
    });

    cout << "  Buscas realizadas              : 100\n";
    cout << "  Buscas com atraso artificial   : 20\n";
    cout << "  Atraso por busca afetada       : 5 ms\n";
    cout << "  Tempo sem latencia artificial  : " << semLatencia << " ms\n";
    cout << "  Tempo com latencia artificial  : " << comLatencia << " ms\n";
    cout << "  Aumento observado              : " << (comLatencia / semLatencia) << "x\n";
    csv[csvCount++] = "R12," + to_string(comLatencia / semLatencia) + "x";

    /* R16 simula sensores defeituosos e compara insercao sem tratamento vs com filtragem. */
    cout << "\n[R16 - Restricao de Dados] 20% de leituras corrompidas\n";
    SensorData* corrompidos = new SensorData[quantidade];
    for(int i = 0; i < quantidade; i++)
        corrompidos[i] = dados[i];

    int totalCorrompidos = quantidade / 5;
    for(int i = 0; i < totalCorrompidos; i++)
    {
        int idx = (i * 37) % quantidade;
        corrompidos[idx].temperatura = -999.0f;
        corrompidos[idx].pressao = 999999.0f;
        corrompidos[idx].umidade = -1.0f;
        corrompidos[idx].vibracao = 9999.0f;
    }

    SensorAnalytics analytics;
    int anomaliasSem = analytics.countAnomalies(corrompidos, quantidade);
    DataTreatment tratamento;
    int inseridosComTratamento = 0;
    int rejeitados = 0;
    SensorData* tratados = new SensorData[quantidade];

    for(int i = 0; i < quantidade; i++)
    {
        if(!tratamento.isValid(corrompidos[i]))
        {
            rejeitados++;
            continue;
        }

        SensorData normalizado = tratamento.normalize(corrompidos[i]);
        if(tratamento.isValid(normalizado))
            tratados[inseridosComTratamento++] = normalizado;
    }

    int anomaliasCom = analytics.countAnomalies(tratados, inseridosComTratamento);
    cout << "  Registros totais              : " << quantidade << "\n";
    cout << "  Registros corrompidos         : " << totalCorrompidos << "\n";
    cout << "  Inseridos sem tratamento      : " << quantidade << "\n";
    cout << "  Anomalias sem tratamento      : " << anomaliasSem << "\n";
    cout << "  Inseridos com tratamento      : " << inseridosComTratamento << "\n";
    cout << "  Rejeitados pelo tratamento    : " << rejeitados << "\n";
    cout << "  Anomalias com tratamento      : " << anomaliasCom << "\n";
    csv[csvCount++] = "R16," + to_string(rejeitados) + " rejeitados";
    delete[] corrompidos;
    delete[] tratados;

    /* R21 evidencia degradacao ao substituir busca logaritmica por busca linear. */
    cout << "\n[R21 - Restricao Algoritmica] AVL substituida por LinkedList\n";
    double tempoAvl = medirMsLambda([&]()
    {
        for(int i = 0; i < 1000; i++)
            avlCompleta.contains(chaveBusca(dados, quantidade, i));
    });
    double tempoLista = medirMsLambda([&]()
    {
        for(int i = 0; i < 1000; i++)
            lista.contains(chaveBusca(dados, quantidade, i));
    });
    cout << "  Tempo de busca AVL        : " << tempoAvl << " ms\n";
    cout << "  Tempo de busca LinkedList : " << tempoLista << " ms\n";
    cout << "  Degradacao observada      : " << (tempoLista / tempoAvl) << "x\n";
    cout << "  Motivo tecnico            : AVL tende a O(log n); LinkedList tende a O(n).\n";
    csv[csvCount++] = "R21," + to_string(tempoLista / tempoAvl) + "x";

    exportarRestricoes(csv, csvCount);
}

static void compararHashes(SensorData* dados, int quantidade)
{
    linha("HASH ORIGINAL VS HASH OTIMIZADA");
    BenchmarkResult h = benchmarkHash(dados, quantidade, 1000, 1000);
    BenchmarkResult o = benchmarkHashOpt(dados, quantidade, 1000, 1000);

    cout << left << setw(26) << "Metrica" << setw(18) << "HashTable"
         << setw(22) << "OptimizedHashTable" << "Melhor\n";
    cout << "----------------------------------------------------------------------------\n";

    auto melhorMenor = [](double a, double b) { return a <= b ? "HashTable" : "Optimized"; };
    auto melhorMaior = [](double a, double b) { return a >= b ? "HashTable" : "Optimized"; };

    cout << left << setw(26) << "Insercao (ms)" << setw(18) << h.insercaoMs << setw(22) << o.insercaoMs << melhorMenor(h.insercaoMs, o.insercaoMs) << "\n";
    cout << setw(26) << "Busca total (ms)" << setw(18) << h.buscaMs << setw(22) << o.buscaMs << melhorMenor(h.buscaMs, o.buscaMs) << "\n";
    cout << setw(26) << "Remocao (ms)" << setw(18) << h.remocaoMs << setw(22) << o.remocaoMs << melhorMenor(h.remocaoMs, o.remocaoMs) << "\n";
    cout << setw(26) << "Memoria (KB)" << setw(18) << h.memoriaKb << setw(22) << o.memoriaKb << melhorMenor(h.memoriaKb, o.memoriaKb) << "\n";
    cout << setw(26) << "Colisoes" << setw(18) << h.colisoes << setw(22) << o.colisoes << melhorMenor(h.colisoes, o.colisoes) << "\n";
    cout << setw(26) << "Taxa colisoes (%)" << setw(18) << h.taxaColisao << setw(22) << o.taxaColisao << melhorMenor(h.taxaColisao, o.taxaColisao) << "\n";
    cout << setw(26) << "Achados" << setw(18) << h.encontrados << setw(22) << o.encontrados << melhorMaior(h.encontrados, o.encontrados) << "\n";
}

static void executarAutomatico(ProjetoContexto& ctx)
{
    cout << "============================================================\n";
    cout << " SISTEMA DE MONITORAMENTO INDUSTRIAL DE SENSORES\n";
    cout << " MODO AUTOMATICO\n";
    cout << "============================================================\n";

    auto tempoInsercao = medirMsLambda([&]() { inserirEmTodas(ctx); });

    linha("INTEGRACAO DAS ESTRUTURAS");
    cout << "Tempo de insercao integrada: " << fixed << setprecision(3) << tempoInsercao << " ms\n";
    cout << "AVL                : " << ctx.avl.size() << " registros\n";
    cout << "HashTable          : " << ctx.hash.size() << " registros\n";
    cout << "LinkedList         : " << ctx.lista.size() << " registros\n";
    cout << "SkipList           : " << ctx.skip.size() << " registros\n";
    cout << "BloomFilter        : " << ctx.bloom.size() << " registros\n";
    cout << "OptimizedHashTable : " << ctx.hashOpt.size() << " registros\n";

    linha("VALIDACAO DE BUSCA E REMOCAO");
    long long primeiro = ctx.dados[0].timestamp;
    long long meio = ctx.dados[ctx.quantidade / 2].timestamp;
    long long ultimo = ctx.dados[ctx.quantidade - 1].timestamp;
    cout << "Busca primeiro em AVL...............: " << (ctx.avl.contains(primeiro) ? "OK" : "ERRO") << "\n";
    cout << "Busca meio em HashTable.............: " << (ctx.hash.contains(meio) ? "OK" : "ERRO") << "\n";
    cout << "Busca ultimo em LinkedList..........: " << (ctx.lista.contains(ultimo) ? "OK" : "ERRO") << "\n";
    cout << "Busca meio em SkipList..............: " << (ctx.skip.contains(meio) ? "OK" : "ERRO") << "\n";
    cout << "Pertinencia no BloomFilter..........: " << (ctx.bloom.contains(primeiro) ? "OK" : "ERRO") << "\n";
    cout << "Busca ultimo em OptimizedHashTable..: " << (ctx.hashOpt.contains(ultimo) ? "OK" : "ERRO") << "\n";

    SensorAnalytics analytics;
    SensorData* filtrados = new SensorData[ctx.quantidade];

    analytics.printStatistics(ctx.dados, ctx.quantidade);
    analytics.printAnomalyReport(ctx.dados, ctx.quantidade);

    linha("FILTRO POR INTERVALO DE TIMESTAMP");
    int qtdFiltrados = analytics.filterByTimestampRange(ctx.dados,
                                                        ctx.quantidade,
                                                        ctx.dados[100].timestamp,
                                                        ctx.dados[199].timestamp,
                                                        filtrados);
    cout << "Registros filtrados entre os timestamps 100 e 199: " << qtdFiltrados << "\n";

    analytics.printSorted(ctx.dados, ctx.quantidade, "temperatura");

    BenchmarkResult resultados[5];
    BenchmarkResult bloom;
    executarBenchmarkCompleto(ctx.dados, ctx.quantidade, resultados, bloom);

    compararHashes(ctx.dados, ctx.quantidade);

    executarTestesRestritivos(ctx.dados, ctx.quantidade);

    EscalaResult escala[4];
    Visualization vis;
    executarEscalabilidade(escala, vis);

    linha("VISUALIZACAO RESUMIDA NO TERMINAL");
    cout << "Resumo do dataset: " << ctx.quantidade << " registros validos.\n";
    cout << "Memoria estimada AVL...............: " << ctx.avl.memoryUsage() / 1024.0 << " KB\n";
    cout << "Memoria estimada HashTable.........: " << ctx.hash.memoryUsage() / 1024.0 << " KB\n";
    cout << "Memoria estimada LinkedList........: " << ctx.lista.memoryUsage() / 1024.0 << " KB\n";
    cout << "Memoria estimada SkipList..........: " << ctx.skip.memoryUsage() / 1024.0 << " KB\n";
    cout << "Memoria estimada BloomFilter.......: " << ctx.bloom.memoryUsage() / 1024.0 << " KB\n";
    cout << "Memoria estimada OptimizedHash.....: " << ctx.hashOpt.memoryUsage() / 1024.0 << " KB\n";

    string labels[5] = {"AVL", "HashTable", "LinkedList", "SkipList", "OptimizedHash"};
    double ins[5], bus[5], rem[5];
    for(int i = 0; i < 5; i++)
    {
        ins[i] = resultados[i].insercaoMs;
        bus[i] = resultados[i].buscaMs;
        rem[i] = resultados[i].remocaoMs;
    }
    vis.printFinalDashboard(labels, ins, bus, rem, 5);

    delete[] filtrados;

    cout << "\n============================================================\n";
    cout << "SISTEMA EXECUTADO COM SUCESSO\n";
    cout << "VALIDACOES PRINCIPAIS CONCLUIDAS\n";
    cout << "============================================================\n";
}

static void inserirManual(ProjetoContexto& ctx)
{
    SensorData s;
    cout << "--- Inserir novo registro ---\n";
    cout << "ID do sensor    : ";
    cin >> s.idSensor;
    s.timestamp = 1000000000LL + ctx.quantidade + 1;
    cout << "Temperatura (C) : ";
    cin >> s.temperatura;
    cout << "Pressao (kPa)   : ";
    cin >> s.pressao;
    cout << "Umidade (%)     : ";
    cin >> s.umidade;
    cout << "Vibracao (mm/s) : ";
    cin >> s.vibracao;

    DataTreatment tratamento;
    s = tratamento.normalize(s);

    if(!tratamento.isValid(s))
    {
        cout << "Registro invalido mesmo apos tratamento. Nao inserido.\n";
        return;
    }

    ctx.avl.insert(s);
    ctx.hash.insert(s);
    ctx.lista.pushBack(s);
    ctx.skip.insert(s);
    ctx.bloom.insert(s.timestamp);
    ctx.hashOpt.insert(s);
    cout << "Registro validado, normalizado e inserido nas estruturas. Timestamp: " << s.timestamp << "\n";
}

static void menuInterativo(ProjetoContexto& ctx)
{
    /*
     * O modo interativo permite que o usuario acompanhe operacoes durante
     * a execucao, cumprindo a exigencia de interacao e visualizacao via terminal.
     * O modo automatico permanece como padrao para testes e CI.
     */
    inserirEmTodas(ctx);
    SensorAnalytics analytics;
    Visualization vis;
    int operacoes = 0;

    while(true)
    {
        cout << "\n============================================================\n";
        cout << " SISTEMA DE MONITORAMENTO INDUSTRIAL DE SENSORES\n";
        cout << "============================================================\n";
        cout << " Dataset carregado: " << ctx.quantidade << " registros validos\n\n";
        cout << " [1]  Inserir novo registro manualmente\n";
        cout << " [2]  Buscar por timestamp\n";
        cout << " [3]  Remover por timestamp\n";
        cout << " [4]  Exibir estatisticas completas\n";
        cout << " [5]  Relatorio de anomalias\n";
        cout << " [6]  Filtrar por intervalo de timestamp\n";
        cout << " [7]  Ordenar dataset por campo\n";
        cout << " [8]  Executar benchmark completo\n";
        cout << " [9]  Executar testes restritivos\n";
        cout << " [10] Visualizar estrutura\n";
        cout << " [11] Comparar Hash original vs Hash otimizada\n";
        cout << " [12] Benchmark de escalabilidade\n";
        cout << " [0]  Sair\n";
        cout << "============================================================\n";
        cout << "Escolha uma opcao: ";

        int opcao = 0;
        if(!(cin >> opcao))
            break;

        operacoes++;

        if(opcao == 0)
        {
            cout << "Sessao encerrada. Operacoes realizadas: " << operacoes - 1 << "\n";
            break;
        }

        if(opcao == 1)
        {
            inserirManual(ctx);
        }
        else if(opcao == 2)
        {
            long long ts;
            cout << "Timestamp: ";
            cin >> ts;
            cout << "AVL        : " << (ctx.avl.contains(ts) ? "ENCONTRADO" : "NAO ENCONTRADO") << "\n";
            cout << "HashTable  : " << (ctx.hash.contains(ts) ? "ENCONTRADO" : "NAO ENCONTRADO") << "\n";
            cout << "SkipList   : " << (ctx.skip.contains(ts) ? "ENCONTRADO" : "NAO ENCONTRADO") << "\n";
            cout << "BloomFilter: " << (ctx.bloom.contains(ts) ? "POSSIVELMENTE PRESENTE" : "AUSENTE") << "\n";
            SensorData* achado = ctx.hashOpt.find(ts);
            if(achado != nullptr)
                imprimirRegistro(*achado);
        }
        else if(opcao == 3)
        {
            long long ts;
            cout << "Timestamp: ";
            cin >> ts;
            ctx.avl.remove(ts);
            bool ok = ctx.hash.remove(ts);
            ctx.lista.remove(ts);
            ctx.skip.remove(ts);
            ctx.bloom.remove(ts);
            ctx.hashOpt.remove(ts);
            cout << "Remocao solicitada. HashTable indicou: " << (ok ? "REMOVIDO" : "NAO ENCONTRADO") << "\n";
        }
        else if(opcao == 4)
        {
            analytics.printStatistics(ctx.dados, ctx.quantidade);
        }
        else if(opcao == 5)
        {
            analytics.printAnomalyReport(ctx.dados, ctx.quantidade);
        }
        else if(opcao == 6)
        {
            long long ini, fim;
            cout << "Inicio: ";
            cin >> ini;
            cout << "Fim: ";
            cin >> fim;
            SensorData* filtrados = new SensorData[ctx.quantidade];
            int qtd = analytics.filterByTimestampRange(ctx.dados, ctx.quantidade, ini, fim, filtrados);
            cout << "Registros filtrados: " << qtd << "\n";
            delete[] filtrados;
        }
        else if(opcao == 7)
        {
            string campo;
            cout << "Campo: ";
            cin >> campo;
            analytics.printSorted(ctx.dados, ctx.quantidade, campo);
        }
        else if(opcao == 8)
        {
            BenchmarkResult resultados[5];
            BenchmarkResult bloom;
            executarBenchmarkCompleto(ctx.dados, ctx.quantidade, resultados, bloom);
        }
        else if(opcao == 9)
        {
            executarTestesRestritivos(ctx.dados, ctx.quantidade);
        }
        else if(opcao == 10)
        {
            ctx.skip.printStatistics();
            cout << "Visualizacao por niveis limitada ao proprio metodo da SkipList.\n";
        }
        else if(opcao == 11)
        {
            compararHashes(ctx.dados, ctx.quantidade);
        }
        else if(opcao == 12)
        {
            EscalaResult escala[4];
            executarEscalabilidade(escala, vis);
        }
        else
        {
            cout << "Opcao invalida.\n";
        }
    }
}

int main(int argc, char* argv[])
{
    ProjetoContexto ctx;
    ctx.dados = gerarDataset(10000,
                             ctx.gerados,
                             ctx.validos,
                             ctx.normalizados,
                             ctx.descartados,
                             true);
    ctx.quantidade = ctx.validos;

    if(argc > 1 && string(argv[1]) == "--interativo")
        menuInterativo(ctx);
    else
        executarAutomatico(ctx);

    delete[] ctx.dados;
    return 0;
}
