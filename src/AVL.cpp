/**
 * @file AVL.cpp
 * @brief Implementacao de arvore AVL para o sistema de monitoramento industrial.
 *
 * PAPEL NO SISTEMA:
 * Armazena leituras de sensores ordenadas por timestamp e oferece busca exata balanceada.
 *
 * CHAVE UTILIZADA:
 * SensorData.timestamp, pois cada leitura gerada recebe um instante unico.
 *
 * COMPLEXIDADE ESPERADA:
 * - insert()   : O(log n)
 * - remove()   : O(log n)
 * - contains() : O(log n)
 * - size()     : O(1)
 *
 * JUSTIFICATIVA DE USO:
 * A AVL representa uma arvore balanceada classica, adequada para comparar busca
 * logaritmica com hash, lista encadeada e skip list.
 *
 * LIMITACOES:
 * Cada no e alocado dinamicamente, o que pode aumentar fragmentacao de memoria.
 *
 * DECISOES DE PROJETO:
 * O balanceamento e mantido por rotacoes e o teste R2 usa maxSize para simular limite
 * de memoria sem alterar a logica central da arvore.
 */
// [IA-GENERATED] Comentarios tecnicos e metodos auxiliares adicionados com assistência de IA. Ver IA_USAGE.md.
#include "AVL.h"

using namespace std;

/*=========================================================
    CONSTRUTOR / DESTRUTOR
=========================================================*/

AVL::AVL()
{
    root = nullptr;
    nodeCount = 0;
    maxSize = -1;
}

AVL::~AVL()
{
    clear();
}

/*=========================================================
    ALTURA
=========================================================*/

int AVL::height(AVLNode* node)
{
    if(node == nullptr)
        return 0;

    return node->height;
}

int AVL::height()
{
    return height(root);
}

void AVL::updateHeight(AVLNode* node)
{
    if(node == nullptr)
        return;

    int leftHeight = height(node->left);
    int rightHeight = height(node->right);

    node->height = 1 + (leftHeight > rightHeight ?
                        leftHeight : rightHeight);
}

int AVL::getBalance(AVLNode* node)
{
    if(node == nullptr)
        return 0;

    return height(node->left) - height(node->right);
}

/*=========================================================
    INFORMAÃ‡Ã•ES
=========================================================*/

bool AVL::isEmpty()
{
    return root == nullptr;
}

int AVL::size()
{
    return nodeCount;
}

/*=========================================================
    LIMPEZA
=========================================================*/

void AVL::destroy(AVLNode* node)
{
    if(node == nullptr)
        return;

    destroy(node->left);
    destroy(node->right);

    delete node;
}

void AVL::clear()
{
    destroy(root);

    root = nullptr;

    nodeCount = 0;
}

/*=========================================================
    STUBS
=========================================================*/

AVLNode* AVL::leftRotate(AVLNode* x)
{
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    updateHeight(x);
    updateHeight(y);

    return y;
}

AVLNode* AVL::rightRotate(AVLNode* y)
{
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    updateHeight(y);
    updateHeight(x);

    return x;
}

AVLNode* AVL::balance(AVLNode* node)
{
    if(node == nullptr)
        return node;

    updateHeight(node);

    int factor = getBalance(node);

    // Caso LL
    if(factor > 1 &&
       getBalance(node->left) >= 0)
    {
        return rightRotate(node);
    }

    // Caso LR
    if(factor > 1 &&
       getBalance(node->left) < 0)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Caso RR
    if(factor < -1 &&
       getBalance(node->right) <= 0)
    {
        return leftRotate(node);
    }

    // Caso RL
    if(factor < -1 &&
       getBalance(node->right) > 0)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

AVLNode* AVL::insert(AVLNode* node,
                     const SensorData& sensor)
{
    // CriaÃ§Ã£o do novo nÃ³
    if(node == nullptr)
    {
        nodeCount++;
        return new AVLNode(sensor);
    }

    // InserÃ§Ã£o BST
    if(sensor.timestamp < node->data.timestamp)
    {
        node->left = insert(node->left, sensor);
    }
    else if(sensor.timestamp > node->data.timestamp)
    {
        node->right = insert(node->right, sensor);
    }
    else
    {
        // NÃ£o permite timestamps duplicados
        return node;
    }

    // Rebalanceamento
    return balance(node);
}

void AVL::insert(const SensorData& sensor)
{
    /*
     * R2 usa maxSize para simular memoria limitada: a arvore rejeita
     * novas leituras quando atinge a capacidade configurada.
     */
    if(maxSize > 0 && nodeCount >= maxSize)
        return;

    root = insert(root, sensor);
}

AVLNode* AVL::remove(AVLNode* node,
                     long timestamp)
{
    if(node == nullptr)
        return nullptr;

    //-------------------------------------------------
    // Procurando o nÃ³
    //-------------------------------------------------

    if(timestamp < node->data.timestamp)
    {
        node->left = remove(node->left, timestamp);
    }
    else if(timestamp > node->data.timestamp)
    {
        node->right = remove(node->right, timestamp);
    }
    else
    {
        //-------------------------------------------------
        // NÃ³ encontrado
        //-------------------------------------------------

        // Sem filhos

        if(node->left == nullptr &&
           node->right == nullptr)
        {
            delete node;
            nodeCount--;
            return nullptr;
        }

        // Um filho

        if(node->left == nullptr)
        {
            AVLNode* temp = node->right;

            delete node;
            nodeCount--;

            return temp;
        }

        if(node->right == nullptr)
        {
            AVLNode* temp = node->left;

            delete node;
            nodeCount--;

            return temp;
        }

        //-------------------------------------------------
        // Dois filhos
        //-------------------------------------------------

        AVLNode* successor =
            findMin(node->right);

        node->data = successor->data;

        node->right =
            remove(node->right,
                   successor->data.timestamp);
    }

    return balance(node);
}

void AVL::remove(long timestamp)
{
    root = remove(root, timestamp);
}

AVLNode* AVL::findMin(AVLNode* node)
{
    while(node != nullptr && node->left != nullptr)
        node = node->left;

    return node;
}

AVLNode* AVL::search(AVLNode* node,
                     long timestamp)
{
    if(node == nullptr)
        return nullptr;

    if(timestamp == node->data.timestamp)
        return node;

    if(timestamp < node->data.timestamp)
        return search(node->left, timestamp);

    return search(node->right, timestamp);
}

SensorData* AVL::find(long timestamp)
{
    AVLNode* node = search(root, timestamp);

    if(node == nullptr)
        return nullptr;

    return &(node->data);
}

bool AVL::contains(long timestamp)
{
    return search(root, timestamp) != nullptr;
}

void AVL::inOrder(AVLNode* node)
{
    if(node == nullptr)
        return;

    inOrder(node->left);

    cout
        << "Timestamp: "
        << node->data.timestamp
        << " | Sensor: "
        << node->data.idSensor
        << endl;

    inOrder(node->right);
}

void AVL::preOrder(AVLNode* node)
{
    if(node == nullptr)
        return;

    cout
        << "Timestamp: "
        << node->data.timestamp
        << " | Sensor: "
        << node->data.idSensor
        << endl;

    preOrder(node->left);

    preOrder(node->right);
}

void AVL::postOrder(AVLNode* node)
{
    if(node == nullptr)
        return;

    postOrder(node->left);

    postOrder(node->right);

    cout
        << "Timestamp: "
        << node->data.timestamp
        << " | Sensor: "
        << node->data.idSensor
        << endl;
}

void AVL::printInOrder()
{
    inOrder(root);
}

void AVL::printPreOrder()
{
    preOrder(root);
}

void AVL::printPostOrder()
{
    postOrder(root);
}

void AVL::setMaxSize(int max)
{
    maxSize = max;
}

int AVL::getMaxSize() const
{
    return maxSize;
}

std::size_t AVL::memoryUsage() const
{
    return static_cast<std::size_t>(nodeCount) * sizeof(AVLNode);
}
