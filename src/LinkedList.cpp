/**
 * @file LinkedList.cpp
 * @brief Implementacao de lista encadeada simples para leituras de sensores.
 *
 * PAPEL NO SISTEMA:
 * Armazena registros em ordem de chegada e serve como baseline linear.
 *
 * CHAVE UTILIZADA:
 * SensorData.timestamp nas operacoes de busca e remocao.
 *
 * COMPLEXIDADE ESPERADA:
 * - pushBack() : O(1), pois ha ponteiro tail
 * - remove()   : O(n)
 * - contains() : O(n)
 * - size()     : O(1)
 *
 * JUSTIFICATIVA DE USO:
 * A lista encadeada e uma estrutura classica simples e evidencia o custo de busca linear.
 *
 * LIMITACOES:
 * Busca arbitraria percorre elemento por elemento, tornando-a ruim para grandes volumes.
 *
 * DECISOES DE PROJETO:
 * Mantem head e tail para insercao no final em O(1), preservando fluxo temporal dos sensores.
 */
// [IA-GENERATED] Comentarios tecnicos e metodos auxiliares adicionados com assistência de IA. Ver IA_USAGE.md.
#include "LinkedList.h"

LinkedList::LinkedList()
{
    head = nullptr;
    tail = nullptr;

    elementCount = 0;
}

LinkedList::~LinkedList()
{
    clear();
}

void LinkedList::clear()
{
    ListNode* current = head;

    while(current != nullptr)
    {
        ListNode* temp = current;

        current = current->next;

        delete temp;
    }

    head = nullptr;
    tail = nullptr;

    elementCount = 0;
}

int LinkedList::size() const
{
    return elementCount;
}

void LinkedList::pushFront(const SensorData& sensor)
{
    ListNode* node = new ListNode(sensor);

    node->next = head;

    head = node;

    if(tail == nullptr)
        tail = node;

    elementCount++;
}

void LinkedList::pushBack(const SensorData& sensor)
{
    ListNode* node = new ListNode(sensor);

    if(head == nullptr)
    {
        head = node;
        tail = node;
    }
    else
    {
        tail->next = node;
        tail = node;
    }

    elementCount++;
}

#include <iostream>

using namespace std;

void LinkedList::printList() const
{
    cout << "\n===== LISTA ENCADEADA =====\n\n";

    ListNode* current = head;

    while(current != nullptr)
    {
        cout << "ID: "
             << current->data.idSensor
             << " | Timestamp: "
             << current->data.timestamp
             << endl;

        current = current->next;
    }

    cout << "\nTotal de elementos: "
         << elementCount
         << endl;
}

bool LinkedList::contains(long timestamp) const
{
    ListNode* current = head;

    while(current != nullptr)
    {
        if(current->data.timestamp == timestamp)
            return true;

        current = current->next;
    }

    return false;
}

SensorData* LinkedList::find(long timestamp)
{
    ListNode* current = head;

    while(current != nullptr)
    {
        if(current->data.timestamp == timestamp)
            return &(current->data);

        current = current->next;
    }

    return nullptr;
}

bool LinkedList::remove(long timestamp)
{
    ListNode* current = head;
    ListNode* previous = nullptr;

    while(current != nullptr)
    {
        if(current->data.timestamp == timestamp)
        {
            if(previous == nullptr)
            {
                head = current->next;

                if(current == tail)
                    tail = nullptr;
            }
            else
            {
                previous->next = current->next;

                if(current == tail)
                    tail = previous;
            }

            delete current;

            elementCount--;

            return true;
        }

        previous = current;
        current = current->next;
    }

    return false;
}

SensorData* LinkedList::front()
{
    if(head == nullptr)
        return nullptr;

    return &(head->data);
}

SensorData* LinkedList::back()
{
    if(tail == nullptr)
        return nullptr;

    return &(tail->data);
}

bool LinkedList::empty() const
{
    return head == nullptr;
}

void LinkedList::printStatistics() const
{
    cout << "\n========== ESTATISTICAS ==========\n\n";

    cout << "Quantidade........: "
         << elementCount
         << endl;

    cout << "Lista vazia.......: "
         << (empty() ? "SIM" : "NAO")
         << endl;

    if(head != nullptr)
    {
        cout << "Primeiro timestamp: "
             << head->data.timestamp
             << endl;
    }

    if(tail != nullptr)
    {
        cout << "Ultimo timestamp..: "
             << tail->data.timestamp
             << endl;
    }
}

void LinkedList::printAnalysis() const
{
    cout << "\n=========================================\n";
    cout << " ANALISE DA LINKED LIST\n";
    cout << "=========================================\n\n";

    cout << "Quantidade de elementos : "
         << elementCount
         << endl;

    cout << "Lista vazia            : "
         << (empty() ? "SIM" : "NAO")
         << endl;

    if(head != nullptr)
    {
        cout << "Primeiro elemento      : "
             << head->data.timestamp
             << endl;

        cout << "Ultimo elemento        : "
             << tail->data.timestamp
             << endl;
    }
    else
    {
        cout << "Primeiro elemento      : NULL" << endl;
        cout << "Ultimo elemento        : NULL" << endl;
    }

    cout << "\nSequencia:\n\n";

    ListNode* current = head;

    while(current != nullptr)
    {
        cout << current->data.timestamp;

        if(current->next != nullptr)
            cout << " -> ";

        current = current->next;
    }

    if(head == nullptr)
        cout << "NULL";
    else
        cout << " -> NULL";

    cout << endl;

    cout << "\nIntegridade............: ";

    bool ok = true;

    if(elementCount == 0)
    {
        if(head != nullptr || tail != nullptr)
            ok = false;
    }
    else
    {
        if(head == nullptr || tail == nullptr)
            ok = false;
    }

    cout << (ok ? "OK" : "ERRO")
         << endl;

    cout << "=========================================\n";
}

bool LinkedList::popFront()
{
    if(head == nullptr)
        return false;

    ListNode* temp = head;

    head = head->next;

    if(head == nullptr)
        tail = nullptr;

    delete temp;

    elementCount--;

    return true;
}

bool LinkedList::popBack()
{
    if(head == nullptr)
        return false;

    if(head == tail)
    {
        delete head;

        head = nullptr;
        tail = nullptr;

        elementCount--;

        return true;
    }

    ListNode* current = head;

    while(current->next != tail)
    {
        current = current->next;
    }

    delete tail;

    tail = current;

    tail->next = nullptr;

    elementCount--;

    return true;
}

void LinkedList::reverse()
{
    ListNode* previous = nullptr;
    ListNode* current = head;
    ListNode* next = nullptr;

    tail = head;

    while(current != nullptr)
    {
        next = current->next;

        current->next = previous;

        previous = current;

        current = next;
    }

    head = previous;
}

std::size_t LinkedList::memoryUsage() const
{
    return static_cast<std::size_t>(elementCount) * sizeof(ListNode);
}
