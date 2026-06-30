/**
 * @file BloomFilter.cpp
 * @brief Implementacao de Counting Bloom Filter para teste probabilistico de pertinencia.
 *
 * PAPEL NO SISTEMA:
 * Verifica se um timestamp possivelmente esta presente com baixo custo de consulta.
 *
 * CHAVE UTILIZADA:
 * SensorData.timestamp.
 *
 * COMPLEXIDADE ESPERADA:
 * - insert()   : O(k)
 * - remove()   : O(k), por usar contadores
 * - contains() : O(k)
 * - size()     : O(1)
 *
 * JUSTIFICATIVA DE USO:
 * Bloom Filter e uma estrutura fora da ementa e ilustra trade-off entre memoria e precisao.
 *
 * LIMITACOES:
 * Pode gerar falsos positivos e por isso nao e benchmarkado como busca exata.
 *
 * DECISOES DE PROJETO:
 * Usa contadores inteiros para permitir remocao controlada, aceitando maior uso de memoria.
 */
// [IA-GENERATED] Comentarios tecnicos e metodos auxiliares adicionados com assistência de IA. Ver IA_USAGE.md.
#include "BloomFilter.h"

#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

BloomFilter::BloomFilter(int size, int hashes)
{
    if(size <= 0)
    {
        size = 10007;
    }

    if(hashes <= 0)
    {
        hashes = 3;
    }

    tableSize = size;

    numberOfHashes = hashes;

    elementCount = 0;

    occupiedCells = 0;

    collisionCount = 0;

    counters = new int[tableSize];

    for(int i = 0; i < tableSize; i++)
    {
        counters[i] = 0;
    }
}

BloomFilter::~BloomFilter()
{
    delete[] counters;
}

int BloomFilter::hash(long key, int index) const
{
    unsigned long long x = static_cast<unsigned long long>(key);

    unsigned long long seed = static_cast<unsigned long long>(index + 1);

    x += seed * 0x9e3779b97f4a7c15ULL;

    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;

    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;

    x = x ^ (x >> 31);

    return static_cast<int>(x % static_cast<unsigned long long>(tableSize));
}

void BloomFilter::clear()
{
    for(int i = 0; i < tableSize; i++)
    {
        counters[i] = 0;
    }

    elementCount = 0;

    occupiedCells = 0;

    collisionCount = 0;
}

void BloomFilter::insert(long key)
{
    for(int i = 0; i < numberOfHashes; i++)
    {
        int position = hash(key, i);

        if(counters[position] == 0)
        {
            occupiedCells++;
        }
        else
        {
            collisionCount++;
        }

        counters[position]++;
    }

    elementCount++;
}

bool BloomFilter::contains(long key) const
{
    for(int i = 0; i < numberOfHashes; i++)
    {
        int position = hash(key, i);

        if(counters[position] == 0)
        {
            return false;
        }
    }

    return true;
}

bool BloomFilter::remove(long key)
{
    if(!contains(key))
    {
        return false;
    }

    for(int i = 0; i < numberOfHashes; i++)
    {
        int position = hash(key, i);

        if(counters[position] > 0)
        {
            counters[position]--;

            if(counters[position] == 0)
            {
                occupiedCells--;
            }
        }
    }

    if(elementCount > 0)
    {
        elementCount--;
    }

    return true;
}

bool BloomFilter::empty() const
{
    return elementCount == 0;
}

int BloomFilter::size() const
{
    return elementCount;
}

int BloomFilter::capacity() const
{
    return tableSize;
}

int BloomFilter::hashCount() const
{
    return numberOfHashes;
}

int BloomFilter::occupiedPositions() const
{
    return occupiedCells;
}

long long BloomFilter::collisions() const
{
    return collisionCount;
}

int BloomFilter::getCounter(int position) const
{
    if(position < 0 || position >= tableSize)
    {
        return -1;
    }

    return counters[position];
}

int BloomFilter::maxCounter() const
{
    int maxValue = 0;

    for(int i = 0; i < tableSize; i++)
    {
        if(counters[i] > maxValue)
        {
            maxValue = counters[i];
        }
    }

    return maxValue;
}

double BloomFilter::averageCounter() const
{
    if(occupiedCells == 0)
    {
        return 0.0;
    }

    long long total = 0;

    for(int i = 0; i < tableSize; i++)
    {
        if(counters[i] > 0)
        {
            total += counters[i];
        }
    }

    return static_cast<double>(total) / occupiedCells;
}

double BloomFilter::loadFactor() const
{
    if(tableSize == 0)
    {
        return 0.0;
    }

    return static_cast<double>(occupiedCells) / tableSize;
}

double BloomFilter::estimatedFalsePositiveRate() const
{
    if(elementCount == 0)
    {
        return 0.0;
    }

    double m = static_cast<double>(tableSize);
    double k = static_cast<double>(numberOfHashes);
    double n = static_cast<double>(elementCount);

    return pow(1.0 - exp(-(k * n) / m), k);
}

bool BloomFilter::validate() const
{
    if(counters == nullptr)
    {
        return false;
    }

    if(tableSize <= 0 || numberOfHashes <= 0)
    {
        return false;
    }

    if(elementCount < 0 || occupiedCells < 0)
    {
        return false;
    }

    int realOccupied = 0;

    long long totalCounters = 0;

    for(int i = 0; i < tableSize; i++)
    {
        if(counters[i] < 0)
        {
            return false;
        }

        if(counters[i] > 0)
        {
            realOccupied++;
        }

        totalCounters += counters[i];
    }

    if(realOccupied != occupiedCells)
    {
        return false;
    }

    if(totalCounters != static_cast<long long>(elementCount) * numberOfHashes)
    {
        return false;
    }

    return true;
}

void BloomFilter::printAnalysis() const
{
    cout << "\n================ BLOOM FILTER - ANALISE ================\n\n";

    cout << "Tipo da estrutura          : Counting Bloom Filter" << endl;

    cout << "Elementos inseridos        : "
         << elementCount
         << endl;

    cout << "Capacidade da tabela       : "
         << tableSize
         << endl;

    cout << "Funcoes hash               : "
         << numberOfHashes
         << endl;

    cout << "Posicoes ocupadas          : "
         << occupiedCells
         << endl;

    cout << "Colisoes estimadas         : "
         << collisionCount
         << endl;

    cout << fixed << setprecision(4);

    cout << "Fator de ocupacao          : "
         << loadFactor() * 100.0
         << "%"
         << endl;

    cout << "Taxa falso positivo estim. : "
         << estimatedFalsePositiveRate() * 100.0
         << "%"
         << endl;

    cout << setprecision(2);

    cout << "Maior contador             : "
         << maxCounter()
         << endl;

    cout << "Media dos contadores usados: "
         << averageCounter()
         << endl;

    cout << "Filtro vazio               : "
         << (empty() ? "SIM" : "NAO")
         << endl;

    cout << "Integridade                : "
         << (validate() ? "OK" : "ERRO")
         << endl;

    cout << "\nInterpretacao:\n";
    cout << "Se contains(x) retornar NAO, o elemento certamente nao esta presente.\n";
    cout << "Se contains(x) retornar SIM, o elemento possivelmente esta presente.\n";
    cout << "Como esta versao usa contadores, a remocao e possivel por decremento.\n";

    cout << "========================================================\n";
}

double BloomFilter::collisionRate() const
{
    if(elementCount <= 0)
        return 0.0;

    return (100.0 * static_cast<double>(collisionCount)) /
           static_cast<double>(elementCount * numberOfHashes);
}

std::size_t BloomFilter::memoryUsage() const
{
    return static_cast<std::size_t>(tableSize) * sizeof(int);
}
