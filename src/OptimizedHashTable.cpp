/**
 * @file OptimizedHashTable.cpp
 * @brief Implementacao de tabela hash otimizada para leituras de sensores.
 *
 * PAPEL NO SISTEMA:
 * Armazena leituras por timestamp usando rehash automatico e mistura de bits.
 *
 * CHAVE UTILIZADA:
 * SensorData.timestamp.
 *
 * COMPLEXIDADE ESPERADA:
 * - insert()   : O(1) medio amortizado, O(n) no pior caso
 * - remove()   : O(1) medio, O(n) no pior caso
 * - contains() : O(1) medio, O(n) no pior caso
 * - size()     : O(1)
 *
 * JUSTIFICATIVA DE USO:
 * Cumpre o requisito de estrutura otimizada ao comparar hash original vs hash otimizada.
 *
 * LIMITACOES:
 * Rehashes custam tempo adicional e ainda pode haver colisao.
 *
 * DECISOES DE PROJETO:
 * Usa tamanhos primos, fator de carga maximo e funcao hash com mistura de bits.
 */
// [IA-GENERATED] Comentarios tecnicos e metodos auxiliares adicionados com assistência de IA. Ver IA_USAGE.md.
#include "OptimizedHashTable.h"

#include <iostream>
#include <iomanip>

using namespace std;

OptimizedHashTable::OptimizedHashTable(int initialSize, double maxLoad)
{
    if(initialSize <= 0)
    {
        initialSize = 101;
    }

    if(maxLoad <= 0.0 || maxLoad > 0.95)
    {
        maxLoad = 0.75;
    }

    table = nullptr;

    tableSize = nextPrime(initialSize);

    elementCount = 0;

    usedBuckets = 0;

    collisionCount = 0;

    rehashCount = 0;

    maxLoadFactor = maxLoad;

    allocateTable(tableSize);
}

OptimizedHashTable::~OptimizedHashTable()
{
    clear();

    delete[] table;
}

void OptimizedHashTable::allocateTable(int size)
{
    table = new OptimizedHashNode*[size];

    for(int i = 0; i < size; i++)
    {
        table[i] = nullptr;
    }
}

int OptimizedHashTable::hash(long long timestamp) const
{
    unsigned long long x = static_cast<unsigned long long>(timestamp);

    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    x = x ^ (x >> 31);

    return static_cast<int>(x % static_cast<unsigned long long>(tableSize));
}

bool OptimizedHashTable::isPrime(int value) const
{
    if(value <= 1)
    {
        return false;
    }

    if(value == 2)
    {
        return true;
    }

    if(value % 2 == 0)
    {
        return false;
    }

    for(int i = 3; i * i <= value; i += 2)
    {
        if(value % i == 0)
        {
            return false;
        }
    }

    return true;
}

int OptimizedHashTable::nextPrime(int value) const
{
    if(value <= 2)
    {
        return 2;
    }

    while(!isPrime(value))
    {
        value++;
    }

    return value;
}

void OptimizedHashTable::insertRehashed(const SensorData& sensor)
{
    int index = hash(sensor.timestamp);

    OptimizedHashNode* newNode = new OptimizedHashNode(sensor);

    if(table[index] == nullptr)
    {
        usedBuckets++;
    }

    newNode->next = table[index];

    table[index] = newNode;

    elementCount++;
}

void OptimizedHashTable::rehash()
{
    OptimizedHashNode** oldTable = table;

    int oldSize = tableSize;

    long long oldCollisions = collisionCount;

    long long oldRehashes = rehashCount;

    tableSize = nextPrime(tableSize * 2 + 1);

    allocateTable(tableSize);

    elementCount = 0;

    usedBuckets = 0;

    for(int i = 0; i < oldSize; i++)
    {
        OptimizedHashNode* current = oldTable[i];

        while(current != nullptr)
        {
            OptimizedHashNode* next = current->next;

            insertRehashed(current->data);

            delete current;

            current = next;
        }
    }

    delete[] oldTable;

    collisionCount = oldCollisions;

    rehashCount = oldRehashes + 1;
}

void OptimizedHashTable::clear()
{
    for(int i = 0; i < tableSize; i++)
    {
        OptimizedHashNode* current = table[i];

        while(current != nullptr)
        {
            OptimizedHashNode* temp = current;

            current = current->next;

            delete temp;
        }

        table[i] = nullptr;
    }

    elementCount = 0;

    usedBuckets = 0;

    collisionCount = 0;

    rehashCount = 0;
}

void OptimizedHashTable::insert(const SensorData& sensor)
{
    SensorData* existing = find(sensor.timestamp);

    if(existing != nullptr)
    {
        *existing = sensor;
        return;
    }

    double projectedLoad =
        static_cast<double>(elementCount + 1) / tableSize;

    if(projectedLoad > maxLoadFactor)
    {
        rehash();
    }

    int index = hash(sensor.timestamp);

    OptimizedHashNode* newNode = new OptimizedHashNode(sensor);

    if(table[index] == nullptr)
    {
        usedBuckets++;
    }
    else
    {
        collisionCount++;
    }

    newNode->next = table[index];

    table[index] = newNode;

    elementCount++;
}

bool OptimizedHashTable::remove(long long timestamp)
{
    int index = hash(timestamp);

    OptimizedHashNode* current = table[index];

    OptimizedHashNode* previous = nullptr;

    while(current != nullptr)
    {
        if(current->data.timestamp == timestamp)
        {
            if(previous == nullptr)
            {
                table[index] = current->next;
            }
            else
            {
                previous->next = current->next;
            }

            delete current;

            elementCount--;

            if(table[index] == nullptr)
            {
                usedBuckets--;
            }

            return true;
        }

        previous = current;

        current = current->next;
    }

    return false;
}

bool OptimizedHashTable::contains(long long timestamp) const
{
    int index = hash(timestamp);

    OptimizedHashNode* current = table[index];

    while(current != nullptr)
    {
        if(current->data.timestamp == timestamp)
        {
            return true;
        }

        current = current->next;
    }

    return false;
}

SensorData* OptimizedHashTable::find(long long timestamp)
{
    int index = hash(timestamp);

    OptimizedHashNode* current = table[index];

    while(current != nullptr)
    {
        if(current->data.timestamp == timestamp)
        {
            return &(current->data);
        }

        current = current->next;
    }

    return nullptr;
}

bool OptimizedHashTable::isEmpty() const
{
    return elementCount == 0;
}

int OptimizedHashTable::size() const
{
    return elementCount;
}

int OptimizedHashTable::capacity() const
{
    return tableSize;
}

int OptimizedHashTable::bucketsUsed() const
{
    return usedBuckets;
}

int OptimizedHashTable::emptyBuckets() const
{
    return tableSize - usedBuckets;
}

long long OptimizedHashTable::collisions() const
{
    return collisionCount;
}

long long OptimizedHashTable::rehashes() const
{
    return rehashCount;
}

double OptimizedHashTable::loadFactor() const
{
    return static_cast<double>(elementCount) / tableSize;
}

double OptimizedHashTable::occupancyRate() const
{
    return static_cast<double>(usedBuckets) / tableSize;
}

int OptimizedHashTable::maxChainLength() const
{
    int maxLength = 0;

    for(int i = 0; i < tableSize; i++)
    {
        int length = 0;

        OptimizedHashNode* current = table[i];

        while(current != nullptr)
        {
            length++;

            current = current->next;
        }

        if(length > maxLength)
        {
            maxLength = length;
        }
    }

    return maxLength;
}

int OptimizedHashTable::minChainLength() const
{
    if(usedBuckets == 0)
    {
        return 0;
    }

    int minLength = elementCount;

    for(int i = 0; i < tableSize; i++)
    {
        int length = 0;

        OptimizedHashNode* current = table[i];

        while(current != nullptr)
        {
            length++;

            current = current->next;
        }

        if(length > 0 && length < minLength)
        {
            minLength = length;
        }
    }

    return minLength;
}

double OptimizedHashTable::averageChainLength() const
{
    if(usedBuckets == 0)
    {
        return 0.0;
    }

    return static_cast<double>(elementCount) / usedBuckets;
}

bool OptimizedHashTable::validate() const
{
    if(table == nullptr)
    {
        return false;
    }

    if(tableSize <= 0 || elementCount < 0 || usedBuckets < 0)
    {
        return false;
    }

    int realElementCount = 0;

    int realUsedBuckets = 0;

    for(int i = 0; i < tableSize; i++)
    {
        OptimizedHashNode* current = table[i];

        if(current != nullptr)
        {
            realUsedBuckets++;
        }

        while(current != nullptr)
        {
            realElementCount++;

            current = current->next;
        }
    }

    if(realElementCount != elementCount)
    {
        return false;
    }

    if(realUsedBuckets != usedBuckets)
    {
        return false;
    }

    return true;
}

void OptimizedHashTable::printAnalysis() const
{
    cout << "\n================ HASH TABLE OTIMIZADA - ANALISE ================\n\n";

    cout << "Elementos...............: "
         << elementCount
         << endl;

    cout << "Capacidade..............: "
         << tableSize
         << endl;

    cout << "Buckets usados..........: "
         << usedBuckets
         << endl;

    cout << "Buckets vazios..........: "
         << emptyBuckets()
         << endl;

    cout << "Colisoes registradas....: "
         << collisionCount
         << endl;

    cout << "Rehashes realizados.....: "
         << rehashCount
         << endl;

    cout << fixed << setprecision(4);

    cout << "Fator de carga..........: "
         << loadFactor()
         << endl;

    cout << "Taxa de ocupacao........: "
         << occupancyRate() * 100.0
         << "%"
         << endl;

    cout << setprecision(2);

    cout << "Maior cadeia............: "
         << maxChainLength()
         << endl;

    cout << "Menor cadeia nao vazia..: "
         << minChainLength()
         << endl;

    cout << "Tamanho medio cadeia....: "
         << averageChainLength()
         << endl;

    cout << "Integridade.............: "
         << (validate() ? "OK" : "ERRO")
         << endl;

    cout << "\nOtimizacoes aplicadas:\n";
    cout << "1. Redimensionamento automatico por fator de carga.\n";
    cout << "2. Tamanho da tabela ajustado para numero primo.\n";
    cout << "3. Funcao hash com mistura de bits.\n";
    cout << "4. Insercao no inicio da cadeia para custo O(1).\n";

    cout << "================================================================\n";
}

double OptimizedHashTable::collisionRate() const
{
    if(elementCount <= 0)
        return 0.0;

    return (100.0 * static_cast<double>(collisionCount)) /
           static_cast<double>(elementCount);
}

std::size_t OptimizedHashTable::memoryUsage() const
{
    return static_cast<std::size_t>(tableSize) * sizeof(OptimizedHashNode*) +
           static_cast<std::size_t>(elementCount) * sizeof(OptimizedHashNode);
}
