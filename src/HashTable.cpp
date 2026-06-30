/**
 * @file HashTable.cpp
 * @brief Implementacao de tabela hash com encadeamento separado.
 *
 * PAPEL NO SISTEMA:
 * Armazena leituras de sensores para busca exata rapida por timestamp.
 *
 * CHAVE UTILIZADA:
 * SensorData.timestamp.
 *
 * COMPLEXIDADE ESPERADA:
 * - insert()   : O(1) medio, O(n) no pior caso
 * - remove()   : O(1) medio, O(n) no pior caso
 * - contains() : O(1) medio, O(n) no pior caso
 * - size()     : O(1)
 *
 * JUSTIFICATIVA DE USO:
 * A tabela hash e uma estrutura classica da ementa e serve como referencia
 * de acesso medio constante.
 *
 * LIMITACOES:
 * Colisoes podem formar cadeias longas e degradar o desempenho.
 *
 * DECISOES DE PROJETO:
 * Usa encadeamento separado e registra colisoes para permitir benchmark comparativo
 * com a tabela hash otimizada.
 */
// [IA-GENERATED] Comentarios tecnicos e metodos auxiliares adicionados com assistência de IA. Ver IA_USAGE.md.
#include "HashTable.h"

HashTable::HashTable(int size)
{
    tableSize = size;

    elementCount = 0;

    collisionCount = 0;

    usedBuckets = 0;

    table = new HashNode*[tableSize];

    for(int i = 0; i < tableSize; i++)
        table[i] = nullptr;
}

HashTable::~HashTable()
{
    clear();

    delete[] table;
}

void HashTable::clear()
{
    for(int i = 0; i < tableSize; i++)
    {
        HashNode* current = table[i];

        while(current != nullptr)
        {
            HashNode* temp = current;
            current = current->next;
            delete temp;
        }

        table[i] = nullptr;
    }

    elementCount = 0;
    usedBuckets = 0;
    collisionCount = 0;
}

int HashTable::size() const
{
    return elementCount;
}

int HashTable::capacity() const
{
    return tableSize;
}

int HashTable::hash(long timestamp) const
{
    return timestamp % tableSize;
}

void HashTable::insert(const SensorData& sensor)
{
    int index = hash(sensor.timestamp);

    if(table[index] == nullptr)
    {
        table[index] = new HashNode(sensor);

        usedBuckets++;
    }
    else
    {
        collisionCount++;

        HashNode* current = table[index];

        while(current->next != nullptr)
            current = current->next;

        current->next = new HashNode(sensor);
    }

    elementCount++;
}

int HashTable::collisions() const
{
    return collisionCount;
}

int HashTable::bucketsUsed() const
{
    return usedBuckets;
}

double HashTable::loadFactor() const
{
    return (double)elementCount /
           (double)tableSize;
}

bool HashTable::contains(long timestamp) const
{
    int index = hash(timestamp);

    HashNode* current = table[index];

    while(current != nullptr)
    {
        if(current->data.timestamp == timestamp)
            return true;

        current = current->next;
    }

    return false;
}

SensorData* HashTable::find(long timestamp)
{
    int index = hash(timestamp);

    HashNode* current = table[index];

    while(current != nullptr)
    {
        if(current->data.timestamp == timestamp)
            return &(current->data);

        current = current->next;
    }

    return nullptr;
}

bool HashTable::remove(long timestamp)
{
    int index = hash(timestamp);

    HashNode* current = table[index];
    HashNode* previous = nullptr;

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

            if(table[index] == nullptr)
                usedBuckets--;

            delete current;

            elementCount--;

            return true;
        }

        previous = current;
        current = current->next;
    }

    return false;
}

void HashTable::printTable() const
{
    cout << "\n===== TABELA HASH =====\n\n";

    for(int i=0;i<tableSize;i++)
    {
        cout << "Bucket[" << i << "] : ";

        HashNode* current = table[i];

        if(current == nullptr)
        {
            cout << "vazio";
        }
        else
        {
            while(current != nullptr)
            {
                cout << current->data.timestamp;

                if(current->next != nullptr)
                    cout << " -> ";

                current = current->next;
            }
        }

        cout << endl;
    }
}

int HashTable::emptyBuckets() const
{
    int count = 0;

    for(int i = 0; i < tableSize; i++)
    {
        if(table[i] == nullptr)
            count++;
    }

    return count;
}

int HashTable::maxChainLength() const
{
    int maximum = 0;

    for(int i = 0; i < tableSize; i++)
    {
        int length = 0;

        HashNode* current = table[i];

        while(current != nullptr)
        {
            length++;
            current = current->next;
        }

        if(length > maximum)
            maximum = length;
    }

    return maximum;
}

int HashTable::minChainLength() const
{
    int minimum = elementCount;

    if(elementCount == 0)
        return 0;

    for(int i = 0; i < tableSize; i++)
    {
        int length = 0;

        HashNode* current = table[i];

        while(current != nullptr)
        {
            length++;
            current = current->next;
        }

        if(length > 0 && length < minimum)
            minimum = length;
    }

    return minimum;
}

double HashTable::averageChainLength() const
{
    if(usedBuckets == 0)
        return 0.0;

    return (double)elementCount /
           (double)usedBuckets;
}

double HashTable::occupancyRate() const
{
    return (double)usedBuckets /
           (double)tableSize * 100.0;
}

void HashTable::printAnalysis() const
{
    cout << "\n========== ANALISE DA HASH ==========\n\n";

    cout << "Capacidade.............: "
         << capacity() << endl;

    cout << "Elementos..............: "
         << size() << endl;

    cout << "Buckets usados.........: "
         << bucketsUsed() << endl;

    cout << "Buckets vazios.........: "
         << emptyBuckets() << endl;

    cout << "Maior cadeia...........: "
         << maxChainLength() << endl;

    cout << "Menor cadeia...........: "
         << minChainLength() << endl;

    cout << "Comprimento medio......: "
         << averageChainLength() << endl;

    cout << "Colisoes...............: "
         << collisions() << endl;

    cout << "Load Factor............: "
         << loadFactor() << endl;

    cout << "Taxa de ocupacao.......: "
         << occupancyRate() << "%" << endl;
}

double HashTable::collisionRate() const
{
    if(elementCount <= 0)
        return 0.0;

    return (100.0 * static_cast<double>(collisionCount)) /
           static_cast<double>(elementCount);
}

std::size_t HashTable::memoryUsage() const
{
    return static_cast<std::size_t>(tableSize) * sizeof(HashNode*) +
           static_cast<std::size_t>(elementCount) * sizeof(HashNode);
}
