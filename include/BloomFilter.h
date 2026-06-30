#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

// [IA-GENERATED] Comentario tecnico de Counting Bloom Filter e metodos auxiliares documentados em IA_USAGE.md.

#include <cstddef>

/**
 * @brief Counting Bloom Filter.
 *
 * Esta implementacao usa contadores inteiros em vez de bits.
 *
 * O Bloom Filter classico nao permite remocao segura, pois zerar um bit
 * pode afetar outros elementos que colidiram na mesma posicao. Com
 * contadores, a insercao incrementa e a remocao decrementa as posicoes
 * associadas a chave.
 *
 * Consequencia: a estrutura consome mais memoria do que um Bloom Filter
 * classico baseado em bits, mas permite simular remocao de forma controlada.
 *
 * Importante: Bloom Filter e probabilistico. Ele pode indicar falso positivo,
 * mas nao deve indicar falso negativo quando usado corretamente.
 *
 * Por isso, seus resultados de busca nao devem ser comparados diretamente
 * com AVL, HashTable, LinkedList ou SkipList como se fossem buscas exatas.
 */
class BloomFilter
{
private:
    int* counters;
    int tableSize;
    int numberOfHashes;
    int elementCount;
    int occupiedCells;
    long long collisionCount;

    int hash(long key, int index) const;

public:
    BloomFilter(int size = 10007, int hashes = 3);
    ~BloomFilter();

    void clear();
    void insert(long key);
    bool contains(long key) const;
    bool remove(long key);
    bool empty() const;
    int size() const;
    int capacity() const;
    int hashCount() const;
    int occupiedPositions() const;
    long long collisions() const;
    double collisionRate() const;
    std::size_t memoryUsage() const;
    int getCounter(int position) const;
    int maxCounter() const;
    double averageCounter() const;
    double loadFactor() const;
    double estimatedFalsePositiveRate() const;
    bool validate() const;
    void printAnalysis() const;
};

#endif
