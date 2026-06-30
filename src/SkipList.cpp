/**
 * @file SkipList.cpp
 * @brief Implementacao de Skip List para busca probabilistica ordenada.
 *
 * PAPEL NO SISTEMA:
 * Armazena leituras ordenadas por timestamp com varios niveis de atalhos.
 *
 * CHAVE UTILIZADA:
 * SensorData.timestamp.
 *
 * COMPLEXIDADE ESPERADA:
 * - insert()   : O(log n) esperado
 * - remove()   : O(log n) esperado
 * - contains() : O(log n) esperado
 * - size()     : O(1)
 *
 * JUSTIFICATIVA DE USO:
 * A Skip List e uma estrutura fora da ementa, comparavel a arvores balanceadas em busca ordenada.
 *
 * LIMITACOES:
 * O desempenho depende da distribuicao aleatoria dos niveis.
 *
 * DECISOES DE PROJETO:
 * Usa promocao probabilistica e visualizacao por niveis para mostrar a estrutura interna.
 */
// [IA-GENERATED] Comentarios tecnicos e metodos auxiliares adicionados com assistência de IA. Ver IA_USAGE.md.
#include "SkipList.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iomanip>

using namespace std;

SkipList::SkipList()
{
    probability = 0.5f;

    currentLevel = 0;

    elementCount = 0;

    SensorData dummy;

    header = new SkipNode(dummy, MAX_LEVEL);

    srand((unsigned)time(nullptr));
}

SkipList::~SkipList()
{
    clear();

    delete header;
}

int SkipList::randomLevel()
{
    int level = 0;

    while(((float)rand() / RAND_MAX) < probability &&
          level < MAX_LEVEL)
    {
        level++;
    }

    return level;
}

void SkipList::clear()
{
    SkipNode* current = header->forward[0];

    while(current != nullptr)
    {
        SkipNode* next = current->forward[0];

        delete current;

        current = next;
    }

    for(int i = 0; i <= MAX_LEVEL; i++)
    {
        header->forward[i] = nullptr;
    }

    currentLevel = 0;

    elementCount = 0;
}

bool SkipList::empty() const
{
    return elementCount == 0;
}

int SkipList::size() const
{
    return elementCount;
}

void SkipList::insert(const SensorData& sensor)
{
    SkipNode* update[MAX_LEVEL + 1];

    SkipNode* current = header;

    for(int i = currentLevel; i >= 0; i--)
    {
        while(current->forward[i] != nullptr &&
              current->forward[i]->data.timestamp < sensor.timestamp)
        {
            current = current->forward[i];
        }

        update[i] = current;
    }

    current = current->forward[0];

    if(current != nullptr &&
       current->data.timestamp == sensor.timestamp)
    {
        current->data = sensor;
        return;
    }

    int newLevel = randomLevel();

    if(newLevel > currentLevel)
    {
        for(int i = currentLevel + 1; i <= newLevel; i++)
        {
            update[i] = header;
        }

        currentLevel = newLevel;
    }

    SkipNode* newNode = new SkipNode(sensor, newLevel);

    for(int i = 0; i <= newLevel; i++)
    {
        newNode->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = newNode;
    }

    elementCount++;
}

void SkipList::printLevelZero() const
{
    cout << "\n===== SKIP LIST - NIVEL 0 =====\n\n";

    SkipNode* current = header->forward[0];

    while(current != nullptr)
    {
        cout << current->data.timestamp;

        if(current->forward[0] != nullptr)
            cout << " -> ";

        current = current->forward[0];
    }

    cout << " -> NULL\n";
}

int SkipList::level() const
{
    return currentLevel;
}

bool SkipList::contains(long timestamp) const
{
    SkipNode* current = header;

    for(int i = currentLevel; i >= 0; i--)
    {
        while(current->forward[i] != nullptr &&
              current->forward[i]->data.timestamp < timestamp)
        {
            current = current->forward[i];
        }
    }

    current = current->forward[0];

    if(current != nullptr &&
       current->data.timestamp == timestamp)
    {
        return true;
    }

    return false;
}

SensorData* SkipList::find(long timestamp)
{
    SkipNode* current = header;

    for(int i = currentLevel; i >= 0; i--)
    {
        while(current->forward[i] != nullptr &&
              current->forward[i]->data.timestamp < timestamp)
        {
            current = current->forward[i];
        }
    }

    current = current->forward[0];

    if(current != nullptr &&
       current->data.timestamp == timestamp)
    {
        return &(current->data);
    }

    return nullptr;
}

bool SkipList::remove(long timestamp)
{
    SkipNode* update[MAX_LEVEL + 1];

    SkipNode* current = header;

    for(int i = currentLevel; i >= 0; i--)
    {
        while(current->forward[i] != nullptr &&
              current->forward[i]->data.timestamp < timestamp)
        {
            current = current->forward[i];
        }

        update[i] = current;
    }

    current = current->forward[0];

    if(current == nullptr ||
       current->data.timestamp != timestamp)
    {
        return false;
    }

    for(int i = 0; i <= current->level; i++)
    {
        if(update[i]->forward[i] == current)
        {
            update[i]->forward[i] = current->forward[i];
        }
    }

    delete current;

    while(currentLevel > 0 &&
          header->forward[currentLevel] == nullptr)
    {
        currentLevel--;
    }

    elementCount--;

    return true;
}

void SkipList::searchWithTrace(long timestamp) const
{
    cout << "\n========== BUSCA NA SKIP LIST ==========\n\n";

    cout << "Procurando timestamp: "
         << timestamp
         << endl;

    SkipNode* current = header;

    int comparisons = 0;

    for(int i = currentLevel; i >= 0; i--)
    {
        cout << "\nNivel "
             << i
             << ": ";

        bool moved = false;

        while(current->forward[i] != nullptr &&
              current->forward[i]->data.timestamp < timestamp)
        {
            comparisons++;

            cout << current->forward[i]->data.timestamp
                 << " -> ";

            current = current->forward[i];

            moved = true;
        }

        if(!moved)
        {
            cout << "sem movimento";
        }
        else
        {
            cout << "parou";
        }
    }

    current = current->forward[0];

    cout << "\n\nComparacoes realizadas: "
         << comparisons
         << endl;

    if(current != nullptr &&
       current->data.timestamp == timestamp)
    {
        cout << "Resultado: ENCONTRADO\n";

        cout << "ID Sensor: "
             << current->data.idSensor
             << endl;

        cout << "Timestamp: "
             << current->data.timestamp
             << endl;
    }
    else
    {
        cout << "Resultado: NAO ENCONTRADO\n";
    }

    cout << "========================================\n";
}

void SkipList::printLevels() const
{
    cout << "\n================ SKIP LIST - VISUALIZACAO POR NIVEIS ================\n\n";

    if(elementCount == 0)
    {
        cout << "Lista vazia.\n";
        cout << "=====================================================================\n";
        return;
    }

    long* timestamps = new long[elementCount];
    int* levels = new int[elementCount];

    SkipNode* current = header->forward[0];

    int index = 0;

    while(current != nullptr)
    {
        timestamps[index] = current->data.timestamp;
        levels[index] = current->level;

        current = current->forward[0];
        index++;
    }

    for(int i = currentLevel; i >= 0; i--)
    {
        cout << "Nivel "
             << i
             << " | HEAD";

        for(int j = 0; j < elementCount; j++)
        {
            if(levels[j] >= i)
            {
                cout << " --> "
                     << setw(6)
                     << timestamps[j];
            }
            else
            {
                cout << " -----------";
            }
        }

        cout << " --> NULL\n";
    }

    cout << "\nElementos............: "
         << elementCount
         << endl;

    cout << "Maior nivel utilizado: "
         << currentLevel
         << endl;

    cout << "\nLegenda:\n";
    cout << "Cada timestamp exibido em um nivel indica que o no foi promovido ate aquele nivel.\n";
    cout << "Os espacos tracejados indicam posicoes ocupadas apenas nos niveis inferiores.\n";

    cout << "=====================================================================\n";

    delete[] timestamps;
    delete[] levels;
}

void SkipList::printStatistics() const
{
    cout << "\n================ SKIP LIST - ESTATISTICAS ================\n\n";

    cout << "Quantidade de elementos : "
         << elementCount
         << endl;

    cout << "Lista vazia             : "
         << (empty() ? "SIM" : "NAO")
         << endl;

    cout << "Nivel maximo permitido  : "
         << MAX_LEVEL
         << endl;

    cout << "Maior nivel utilizado   : "
         << currentLevel
         << endl;

    cout << "Probabilidade promocao  : "
         << probability
         << endl;

    if(elementCount == 0)
    {
        cout << "\nDistribuicao inexistente, pois a lista esta vazia.\n";
        cout << "===========================================================\n";
        return;
    }

    int exactLevel[MAX_LEVEL + 1];
    int visibleAtLevel[MAX_LEVEL + 1];

    for(int i = 0; i <= MAX_LEVEL; i++)
    {
        exactLevel[i] = 0;
        visibleAtLevel[i] = 0;
    }

    SkipNode* current = header->forward[0];

    int highestNodeLevel = 0;
    int totalLevel = 0;
    int totalHeight = 0;

    while(current != nullptr)
    {
        exactLevel[current->level]++;

        for(int i = 0; i <= current->level; i++)
        {
            visibleAtLevel[i]++;
        }

        if(current->level > highestNodeLevel)
        {
            highestNodeLevel = current->level;
        }

        totalLevel += current->level;
        totalHeight += current->level + 1;

        current = current->forward[0];
    }

    double averageLevel = (double) totalLevel / elementCount;
    double averageHeight = (double) totalHeight / elementCount;

    cout << fixed << setprecision(2);

    cout << "Nivel medio dos nos    : "
         << averageLevel
         << endl;

    cout << "Altura media dos nos   : "
         << averageHeight
         << endl;

    cout << "\nDistribuicao por nivel exato:\n\n";

    for(int i = 0; i <= currentLevel; i++)
    {
        double percentage = (100.0 * exactLevel[i]) / elementCount;

        cout << "Nivel "
             << setw(2)
             << i
             << ": "
             << setw(3)
             << exactLevel[i]
             << " no(s)  | "
             << setw(6)
             << percentage
             << "%"
             << endl;
    }

    cout << "\nNos visiveis em cada nivel:\n\n";

    for(int i = currentLevel; i >= 0; i--)
    {
        double percentage = (100.0 * visibleAtLevel[i]) / elementCount;

        cout << "Nivel "
             << setw(2)
             << i
             << ": "
             << setw(3)
             << visibleAtLevel[i]
             << " no(s)  | "
             << setw(6)
             << percentage
             << "%"
             << endl;
    }

    cout << "\nVerificacao de integridade:\n\n";

    bool integrity = true;

    if(visibleAtLevel[0] != elementCount)
    {
        integrity = false;
    }

    if(highestNodeLevel != currentLevel)
    {
        integrity = false;
    }

    cout << "Nivel 0 contem todos os nos : "
         << (visibleAtLevel[0] == elementCount ? "SIM" : "NAO")
         << endl;

    cout << "currentLevel consistente    : "
         << (highestNodeLevel == currentLevel ? "SIM" : "NAO")
         << endl;

    cout << "Integridade geral           : "
         << (integrity ? "OK" : "ERRO")
         << endl;

    cout << "\nInterpretacao:\n";
    cout << "Nos com nivel maior aparecem tambem nos niveis inferiores.\n";
    cout << "Quanto maior o nivel, menor tende a ser a quantidade de nos.\n";
    cout << "Essa distribuicao e causada pela promocao probabilistica.\n";

    cout << "===========================================================\n";
}
bool SkipList::validateOrder() const
{
    SkipNode* current = header->forward[0];

    if(current == nullptr)
    {
        return elementCount == 0;
    }

    int count = 0;

    while(current != nullptr)
    {
        if(current->level < 0 || current->level > MAX_LEVEL)
        {
            return false;
        }

        if(current->forward[0] != nullptr &&
           current->data.timestamp >= current->forward[0]->data.timestamp)
        {
            return false;
        }

        count++;

        current = current->forward[0];
    }

    return count == elementCount;
}

bool SkipList::validateIntegrity() const
{
    if(header == nullptr)
    {
        return false;
    }

    if(elementCount == 0)
    {
        if(currentLevel != 0)
        {
            return false;
        }

        for(int i = 0; i <= MAX_LEVEL; i++)
        {
            if(header->forward[i] != nullptr)
            {
                return false;
            }
        }

        return true;
    }

    int count = 0;
    int highestLevelFound = 0;

    SkipNode* current = header->forward[0];

    while(current != nullptr)
    {
        if(current->level < 0 || current->level > MAX_LEVEL)
        {
            return false;
        }

        if(current->level > highestLevelFound)
        {
            highestLevelFound = current->level;
        }

        count++;

        current = current->forward[0];
    }

    if(count != elementCount)
    {
        return false;
    }

    if(highestLevelFound != currentLevel)
    {
        return false;
    }

    for(int i = currentLevel + 1; i <= MAX_LEVEL; i++)
    {
        if(header->forward[i] != nullptr)
        {
            return false;
        }
    }

    for(int i = currentLevel; i >= 0; i--)
    {
        current = header;

        while(current->forward[i] != nullptr)
        {
            SkipNode* next = current->forward[i];

            if(next->level < i)
            {
                return false;
            }

            if(next->forward[i] != nullptr &&
               next->data.timestamp >= next->forward[i]->data.timestamp)
            {
                return false;
            }

            current = next;
        }
    }

    return validateOrder();
}

std::size_t SkipList::memoryUsage() const
{
    /*
     * Estimativa conservadora: soma o objeto SkipNode e o vetor de ponteiros
     * esperado por no. O nivel medio de uma skip list com p=0.5 fica proximo
     * de 1 ponteiro extra, por isso usamos currentLevel+1 como aproximacao simples.
     */
    return static_cast<std::size_t>(elementCount) *
           (sizeof(SkipNode) + sizeof(SkipNode*) * static_cast<std::size_t>(currentLevel + 1));
}
