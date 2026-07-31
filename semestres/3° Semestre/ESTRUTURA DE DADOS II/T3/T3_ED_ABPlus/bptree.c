#include "bptree.h" 

// FUNÇÃO: createNode
// OBJETIVO: Criar um novo nó da árvore B+.
// Elarec ebe um valor booleano dizendo se o nó é folha ou não.
BPTreeNode *createNode(bool isLeaf) {

    // Aloca memória para o novo nó
    BPTreeNode *n = (BPTreeNode *)malloc(sizeof(BPTreeNode));

    // Caso não tenha memória suficiente
    if (!n) { 
        fprintf(stderr, "Memoria insuficiente\n"); 
        exit(EXIT_FAILURE); 
    }

    // Define se o nó será folha ou nó interno
    n->isLeaf = isLeaf;

    // Começa com zero chaves armazenadas
    n->numKeys = 0;

    // Para lista encadeada de folhas (só usado em folhas)
    n->next = NULL;

    // Zera o vetor de chaves
    memset(n->keys, 0, sizeof(n->keys));

    // Inicializa os ponteiros de filhos como NULL
    for (int i = 0; i < ORDER + 1; i++)
        n->children[i] = NULL;

    return n;
}

// FUNÇÃO: createTree
// OBJETIVO: criar uma árvore B+ vazia
BPTree *createTree() {

    // Aloca a estrutura principal da árvore
    BPTree *t = (BPTree *)malloc(sizeof(BPTree));

    // A raiz começa sendo uma folha vazia
    t->root = createNode(true);

    return t;
}

// FUNÇÃO: findLeaf
// OBJETIVO: encontrar a folha correta onde uma determinada chave deve estar
// é usada para inserção, remoção e busca
static BPTreeNode *findLeaf(BPTreeNode *root, int key) {

    // Começa na raiz
    BPTreeNode *cur = root;

    // Enquanto não chegar nas folhas, continua descendo
    while (!cur->isLeaf) {

        int i = 0;

        // Encontra qual filho seguir de acordo com a chave
        while (i < cur->numKeys && key >= cur->keys[i])
            i++;

        // Vai para o filho correto
        cur = cur->children[i];
    }

    return cur;
}

// FUNÇÃO: findParent
// OBJETIVO: dada a raiz e um nó filho, encontrar o nó pai dele
// Necessário para redistribuir chaves quando folhas dividem ou fundem
static BPTreeNode *findParent(BPTreeNode *root, BPTreeNode *child) {

    // Se chegou numa folha ou em NULL, não há pai
    if (!root || root->isLeaf)
        return NULL;

    // Verifica se algum dos filhos diretos é o nó procurado
    for (int i = 0; i <= root->numKeys; i++) {
        if (root->children[i] == child)
            return root;
    }

    // Se não achou, procura recursivamente nos filhos
    for (int i = 0; i <= root->numKeys; i++) {
        BPTreeNode *p = findParent(root->children[i], child);
        if (p) return p;
    }

    // Não achou pai
    return NULL;
}

// FUNÇÃO: search
// OBJETIVO: procurar uma chave na árvore e retornar true/false
bool search(BPTree *tree, int key) {

    // Se a árvore estiver vazia, retorna falso
    if (!tree || !tree->root)
        return false;

    // Encontra a folha onde a chave deveria estar
    BPTreeNode *leaf = findLeaf(tree->root, key);

    // Procura linearmente na folha
    for (int i = 0; i < leaf->numKeys; i++)
        if (leaf->keys[i] == key)
            return true;

    return false;
}

// FUNÇÃO: insertIntoLeafSorted
// OBJETIVO: inserir uma nova chave dentro da folha mantendo ordenação
static void insertIntoLeafSorted(BPTreeNode *leaf, int key) {

    // Começa pela última posição preenchida
    int i = leaf->numKeys - 1;

    // Move elementos para a direita enquanto forem maiores que a chave nova
    while (i >= 0 && leaf->keys[i] > key) {
        leaf->keys[i + 1] = leaf->keys[i];
        i--;
    }

    // Insere a chave na posição correta
    leaf->keys[i + 1] = key;

    // aumenta o número de chaves
    leaf->numKeys++;
}

// FUNÇÃO: insertIntoParent
// OBJETIVO: inserir uma chave promovida dentro do pai após uma divisão de nó
static void insertIntoParent(BPTree *tree, BPTreeNode *left, int key, BPTreeNode *right) {

    // Primeiro acha o nó pai
    BPTreeNode *parent = findParent(tree->root, left);

    // Se não tem pai, significa que estamos criando uma nova raiz
    if (!parent) {
        BPTreeNode *newRoot = createNode(false);
        newRoot->keys[0] = key;
        newRoot->numKeys = 1;
        newRoot->children[0] = left;
        newRoot->children[1] = right;
        tree->root = newRoot;
        return;
    }

    // Descobre onde inserir a chave dentro do pai
    int idx = 0;
    while (idx < parent->numKeys && parent->keys[idx] < key)
        idx++;

    // Move chaves e ponteiros à direita para abrir espaço
    for (int i = parent->numKeys - 1; i >= idx; i--)
        parent->keys[i + 1] = parent->keys[i];

    for (int i = parent->numKeys + 1; i > idx + 1; i--)
        parent->children[i] = parent->children[i - 1];

    // Insere chave e novo ponteiro
    parent->keys[idx] = key;
    parent->children[idx + 1] = right;
    parent->numKeys++;

    // Se o pai agora estiver cheio, precisa dividir também
    if (parent->numKeys > MAX_KEYS) {

        int total = parent->numKeys;
        int mid = total / 2;              // índice do meio
        int promote = parent->keys[mid];  // chave que sobe para o pai do pai

        BPTreeNode *newInt = createNode(false);

        // Copia metade direita das chaves para o novo nó interno
        int j = 0;
        for (int i = mid + 1; i < total; i++)
            newInt->keys[j++] = parent->keys[i];

        newInt->numKeys = j;

        // Copia também os filhos correspondentes
        int childCount = total + 1;
        int startChild = mid + 1;
        int k = 0;
        for (int i = startChild; i < childCount; i++)
            newInt->children[k++] = parent->children[i];

        // Ajusta o pai antigo para ficar só com metade esquerda
        parent->numKeys = mid;
        for (int i = parent->numKeys + 1; i <= MAX_KEYS + 1; i++)
            parent->children[i] = NULL;

        // Agora chama recursivamente para inserir a promoção no nível acima
        insertIntoParent(tree, parent, promote, newInt);
    }
}

// FUNÇÃO: splitLeaf
// OBJETIVO: dividir uma folha cheia em duas folhas
static void splitLeaf(BPTree *tree, BPTreeNode *leaf) {

    int total = leaf->numKeys;

    // Ponto de divisão (metade)
    int split = (total + 1) / 2;

    // Novo nó folha
    BPTreeNode *newLeaf = createNode(true);

    // Copia a metade direita das chaves para a folha nova
    int j = 0;
    for (int i = split; i < total; i++)
        newLeaf->keys[j++] = leaf->keys[i];

    // Define quantas chaves ficaram no novo nó
    newLeaf->numKeys = j;

    // Atualiza a folha original com só metade esquerda
    leaf->numKeys = split;

    // Ajusta lista encadeada de folhas
    newLeaf->next = leaf->next;
    leaf->next = newLeaf;

    // A chave promovida é a primeira da nova folha
    int promoted = newLeaf->keys[0];

    // Insere chave promovida no pai
    insertIntoParent(tree, leaf, promoted, newLeaf);
}


// FUNÇÃO: insert
// OBJETIVO: inserir uma chave na árvore B+
void insert(BPTree *tree, int key) {

    // Se a árvore não existir, cria
    if (!tree->root)
        tree->root = createNode(true);

    // Encontra a folha correta
    BPTreeNode *leaf = findLeaf(tree->root, key);

    // Insere na folha mantendo ordenação
    insertIntoLeafSorted(leaf, key);

    // Se passou do limite, precisa dividir
    if (leaf->numKeys > MAX_KEYS)
        splitLeaf(tree, leaf);
}


// FUNÇÃO: removeFromLeaf
// OBJETIVO: remover uma chave de uma folha e deslocar as demais para esquerda
static void removeFromLeaf(BPTreeNode *leaf, int pos) {

    // Move tudo um espaço para a esquerda
    for (int i = pos; i < leaf->numKeys - 1; i++)
        leaf->keys[i] = leaf->keys[i + 1];

    leaf->numKeys--;
}

// FUNÇÃO: updateParentKeyForChild
// OBJETIVO: quando menor chave de uma folha muda, atualizar o pai
static void updateParentKeyForChild(BPTree *tree, BPTreeNode *child, int oldKey, int newKey) {

    BPTreeNode *parent = findParent(tree->root, child);

    while (parent) {

        int idx = -1;

        // Encontra qual posição do pai aponta para o filho
        for (int i = 0; i <= parent->numKeys; i++) {
            if (parent->children[i] == child) {
                idx = i;
                break;
            }
        }

        // Se não achou naquele nível, continua subindo
        if (idx == -1) {
            parent = findParent(tree->root, parent);
            continue;
        }

        // Atualiza a chave no pai
        if (idx > 0) {
            parent->keys[idx - 1] = newKey;
            return;
        } else {
            child = parent;
            parent = findParent(tree->root, child);
        }
    }
}

//  AS FUNÇÕES A SEGUIR IMPLEMENTAM O MECANISMO DE EMPRÉSTIMO (BORROW) E MERGE
//  PARA O CASO DE REMOÇÃO EM FOLHAS E NÓS INTERNOS

// FUNÇÃO: borrowFromLeftLeaf
// OBJETIVO: pegar uma chave emprestada da folha à esquerda
static bool borrowFromLeftLeaf(BPTree *tree, BPTreeNode *leaf) {

    BPTreeNode *parent = findParent(tree->root, leaf);
    if (!parent) return false;

    int idx = -1;

    // Descobre posição da folha dentro do pai
    for (int i = 0; i <= parent->numKeys; i++)
        if (parent->children[i] == leaf) {
            idx = i;
            break;
        }

    // Se não existe irmão à esquerda
    if (idx <= 0)
        return false;

    // Irmão à esquerda
    BPTreeNode *left = parent->children[idx - 1];

    // Só pode pegar emprestado se ele tiver mais que o mínimo
    if (left->numKeys > MIN_KEYS) {

        // Desloca espaço na folha atual
        for (int i = leaf->numKeys - 1; i >= 0; i--)
            leaf->keys[i + 1] = leaf->keys[i];

        // Pega a maior chave da folha esquerda
        leaf->keys[0] = left->keys[left->numKeys - 1];

        // Atualiza contadores
        leaf->numKeys++;
        left->numKeys--;

        // Atualiza a chave usada no pai
        parent->keys[idx - 1] = leaf->keys[0];

        return true;
    }

    return false;
}

// FUNÇÃO: borrowFromRightLeaf
// OBJETIVO: pegar chave emprestada da folha à direita
static bool borrowFromRightLeaf(BPTree *tree, BPTreeNode *leaf) {

    BPTreeNode *parent = findParent(tree->root, leaf);
    if (!parent) return false;

    int idx = -1;

    // Descobre posição do nó no pai
    for (int i = 0; i <= parent->numKeys; i++)
        if (parent->children[i] == leaf) {
            idx = i;
            break;
        }

    // Se não há irmão à direita
    if (idx == -1 || idx >= parent->numKeys)
        return false;

    BPTreeNode *right = parent->children[idx + 1];

    if (right->numKeys > MIN_KEYS) {

        // Coloca menor chave do irmão à direita no final da folha atual
        leaf->keys[leaf->numKeys] = right->keys[0];
        leaf->numKeys++;

        // Ajusta folha direita
        for (int i = 0; i < right->numKeys - 1; i++)
            right->keys[i] = right->keys[i + 1];

        right->numKeys--;

        // Atualiza chave no pai
        parent->keys[idx] = right->keys[0];

        return true;
    }

    return false;
}


// FUNÇÃO: mergeLeaves
// OBJETIVO: mesclar duas folhas quando ambas ficam abaixo do mínimo
static void mergeLeaves(BPTree *tree, BPTreeNode *left, BPTreeNode *right) {

    // Copia todas as chaves da direita para a esquerda
    for (int i = 0; i < right->numKeys; i++)
        left->keys[left->numKeys + i] = right->keys[i];

    left->numKeys += right->numKeys;

    // Ajusta lista encadeada
    left->next = right->next;

    // Agora remove referência ao nó direito no pai
    BPTreeNode *parent = findParent(tree->root, left);

    // Se não existir pai, significa que virou raiz
    if (!parent) {
        if (tree->root != left)
            tree->root = left;

        free(right);
        return;
    }

    int idx = -1;

    // Descobre qual filho é o 'right'
    for (int i = 0; i <= parent->numKeys; i++)
        if (parent->children[i] == right) {
            idx = i;
            break;
        }

    if (idx == -1) {
        free(right);
        return;
    }

    // Remove ponteiro para o nó direito no pai
    for (int i = idx; i < parent->numKeys; i++)
        parent->children[i] = parent->children[i + 1];

    parent->children[parent->numKeys] = NULL;

    // Ajusta as chaves do pai
    for (int i = idx - 1; i < parent->numKeys - 1; i++)
        parent->keys[i] = parent->keys[i + 1];

    parent->numKeys--;

    free(right);

    // Caso pai tenha ficado sem chaves e seja raiz
    if (parent == tree->root && parent->numKeys == 0) {
        tree->root = parent->children[0];
        free(parent);
        return;
    }
}


// (Outras funções internas de borrow/merge para nós internos)
// O funcionamento é equivalente ao das folhas, porém envolvendo também filhos.


// --- borrowFromLeftInternal ---------------------------------------------------
static bool borrowFromLeftInternal(BPTree *tree, BPTreeNode *node, int childIndex) {

    BPTreeNode *child = node->children[childIndex];

    // Se não existe irmão à esquerda
    if (childIndex <= 0)
        return false;

    BPTreeNode *leftSibling = node->children[childIndex - 1];

    if (leftSibling->numKeys > MIN_KEYS) {

        // Desloca as chaves e filhos dentro do nó filho
        for (int i = child->numKeys - 1; i >= 0; i--)
            child->keys[i + 1] = child->keys[i];

        for (int i = child->numKeys + 1; i > 0; i--)
            child->children[i] = child->children[i - 1];

        // Traz chave do pai para o início
        child->keys[0] = node->keys[childIndex - 1];

        // Liga o último filho do irmão esquerdo
        child->children[0] = leftSibling->children[leftSibling->numKeys];

        child->numKeys++;

        // Move chave do irmão esquerdo para o pai
        node->keys[childIndex - 1] = leftSibling->keys[leftSibling->numKeys - 1];

        leftSibling->numKeys--;
        leftSibling->children[leftSibling->numKeys + 1] = NULL;

        return true;
    }

    return false;
}


// --- borrowFromRightInternal --------------------------------------------------
static bool borrowFromRightInternal(BPTree *tree, BPTreeNode *node, int childIndex) {

    BPTreeNode *child = node->children[childIndex];

    if (childIndex >= node->numKeys)
        return false;

    BPTreeNode *rightSibling = node->children[childIndex + 1];

    if (rightSibling->numKeys > MIN_KEYS) {

        // Traz chave do pai para o filho
        child->keys[child->numKeys] = node->keys[childIndex];
        child->children[child->numKeys + 1] = rightSibling->children[0];
        child->numKeys++;

        // Move primeira chave do irmão para o pai
        node->keys[childIndex] = rightSibling->keys[0];

        // Aperta o irmão direito
        for (int i = 0; i < rightSibling->numKeys - 1; i++)
            rightSibling->keys[i] = rightSibling->keys[i + 1];

        for (int i = 0; i < rightSibling->numKeys; i++)
            rightSibling->children[i] = rightSibling->children[i + 1];

        rightSibling->children[rightSibling->numKeys] =
            rightSibling->children[rightSibling->numKeys + 1];

        rightSibling->numKeys--;

        return true;
    }

    return false;
}


// --- mergeInternalChildren ----------------------------------------------------
static void mergeInternalChildren(BPTree *tree, BPTreeNode *parent, int index) {

    // Nós a serem fundidos
    BPTreeNode *left  = parent->children[index];
    BPTreeNode *right = parent->children[index + 1];

    // Move chave do pai para dentro do nó esquerdo
    left->keys[left->numKeys] = parent->keys[index];
    left->numKeys++;

    // Copia todas as chaves do nó direito para o esquerdo
    for (int i = 0; i < right->numKeys; i++)
        left->keys[left->numKeys + i] = right->keys[i];

    // Copia também os seus filhos
    for (int i = 0; i <= right->numKeys; i++)
        left->children[left->numKeys + i] = right->children[i];

    left->numKeys += right->numKeys;

    // Agora remove a chave do pai
    for (int i = index; i < parent->numKeys - 1; i++)
        parent->keys[i] = parent->keys[i + 1];

    // Remove ponteiro para o nó direito no pai
    for (int i = index + 1; i <= parent->numKeys; i++)
        parent->children[i] = parent->children[i + 1];

    parent->children[parent->numKeys] = NULL;
    parent->numKeys--;

    free(right);

    // Se o pai ficou sem chaves e era raiz, promove o filho
    if (parent == tree->root && parent->numKeys == 0) {
        tree->root = left;
        free(parent);
    }
}


// FUNÇÃO: fixAfterDeletion
// OBJETIVO: depois da remoção, verificar se houve violação dos limites mínimos
// e aplicar borrow ou merge se necessário
static void fixAfterDeletion(BPTree *tree, BPTreeNode *node) {

    // Caso especial: se o nó for raiz
    if (node == tree->root) {

        // Se a raiz não for folha e tiver ficado vazia, rebaixa nível
        if (!node->isLeaf && node->numKeys == 0) {
            BPTreeNode *newRoot = node->children[0];
            tree->root = newRoot;
            free(node);
        }
        return;
    }

    // ---------------- FOLHAS ----------------
    if (node->isLeaf) {

        // Se ainda está dentro do mínimo, não precisa arrumar
        if (node->numKeys >= MIN_KEYS)
            return;

        BPTreeNode *parent = findParent(tree->root, node);
        if (!parent) return;

        int idx = -1;

        // Descobre o índice do nó dentro do pai
        for (int i = 0; i <= parent->numKeys; i++)
            if (parent->children[i] == node) {
                idx = i;
                break;
            }

        // Tenta pegar chave emprestada da esquerda
        if (idx > 0 && borrowFromLeftLeaf(tree, node))
            return;

        // Tenta da direita
        if (idx < parent->numKeys && borrowFromRightLeaf(tree, node))
            return;

        // Se chegou aqui, tem que fundir
        if (idx > 0) {
            BPTreeNode *left = parent->children[idx - 1];
            mergeLeaves(tree, left, node);
            fixAfterDeletion(tree, parent);
        }
        else {
            BPTreeNode *right = parent->children[idx + 1];
            mergeLeaves(tree, node, right);
            fixAfterDeletion(tree, parent);
        }
    }

    // ---------------- NÓ INTERNO ----------------
    else {

        // Se ainda tem chaves suficientes, nada a fazer
        if (node->numKeys >= MIN_KEYS)
            return;

        BPTreeNode *parent = findParent(tree->root, node);
        if (!parent) return;

        int idx = -1;

        // Acha posição no pai
        for (int i = 0; i <= parent->numKeys; i++)
            if (parent->children[i] == node) {
                idx = i;
                break;
            }

        // Tenta pegar emprestado da esquerda
        if (borrowFromLeftInternal(tree, parent, idx))
            return;

        // Da direita
        if (borrowFromRightInternal(tree, parent, idx))
            return;

        // Se nenhuma funcionou, faz merge
        if (idx > 0) {
            mergeInternalChildren(tree, parent, idx - 1);
            fixAfterDeletion(tree, parent);
        }
        else {
            mergeInternalChildren(tree, parent, idx);
            fixAfterDeletion(tree, parent);
        }
    }
}


// FUNÇÃO: removeKey
// OBJETIVO: remover uma chave da árvore B+
void removeKey(BPTree *tree, int key) {

    if (!tree || !tree->root)
        return;

    // Primeiro encontra a folha onde a chave deveria estar
    BPTreeNode *leaf = findLeaf(tree->root, key);

    int pos = -1;

    // Procura posição da chave dentro da folha
    for (int i = 0; i < leaf->numKeys; i++) {
        if (leaf->keys[i] == key) {
            pos = i;
            break;
        }
    }

    // Se não encontrou chave, termina aqui
    if (pos == -1) return;

    // Remove a chave da folha
    removeFromLeaf(leaf, pos);

    // Se foi removida da raiz e ela ficou vazia, nada grave (árvore vazia)
    if (leaf == tree->root) {
        if (leaf->numKeys == 0) {
            // (caso de árvore vazia)
        }
        return;
    }

    // Ajuste de chaves no pai caso a menor chave tenha mudado
    BPTreeNode *parent = findParent(tree->root, leaf);

    if (parent) {

        // Se removemos a menor chave da folha
        if (pos == 0) {

            int idx = -1;
            for (int i = 0; i <= parent->numKeys; i++)
                if (parent->children[i] == leaf) {
                    idx = i;
                    break;
                }

            if (idx > 0)
                parent->keys[idx - 1] = leaf->keys[0];
            else {

                BPTreeNode *cur = parent;
                BPTreeNode *child = leaf;

                while (cur) {

                    int id = -1;

                    for (int i = 0; i <= cur->numKeys; i++)
                        if (cur->children[i] == child) {
                            id = i;
                            break;
                        }

                    if (id > 0) {
                        cur->keys[id - 1] = leaf->keys[0];
                        break;
                    }

                    child = cur;
                    cur = findParent(tree->root, cur);
                }
            }
        }
    }

    // Se o nó ficou com chaves abaixo do mínimo, corrige
    if (leaf->numKeys < MIN_KEYS)
        fixAfterDeletion(tree, leaf);
}


// FUNÇÃO: printTreeRecursive
// OBJETIVO: função auxiliar para imprimir a árvore em pré-ordem com recuo
static void printTreeRecursive(BPTreeNode *node, int level) {

    if (!node)
        return;

    // Exibe chaves do nó no nível atual
    printf("Nivel %d: [", level);
    for (int i = 0; i < node->numKeys; i++) {
        if (i) printf(" ");
        printf("%d", node->keys[i]);
    }
    printf("]\n");

    // Se não for folha, imprime filhos recursivamente
    if (!node->isLeaf) {
        for (int i = 0; i <= node->numKeys; i++)
            printTreeRecursive(node->children[i], level + 1);
    }
}


// FUNÇÃO: printTree
// OBJETIVO: chamar a impressão completa da árvore + exibir folhas encadeadas
void printTree(BPTree *tree) {

    if (!tree || !tree->root) {
        printf("Arvore vazia\n");
        return;
    }

    printf("\nArvore B+:\n");

    // Imprime a estrutura hierárquica
    printTreeRecursive(tree->root, 0);

    // Agora imprime a lista de folhas
    BPTreeNode *cur = tree->root;

    // Desce até achar a primeira folha
    while (cur && !cur->isLeaf)
        cur = cur->children[0];

    if (cur) {
        printf("\nFolhas (encadeadas):\n");
        while (cur) {
            printf("[");
            for (int i = 0; i < cur->numKeys; i++) {
                if (i) printf(" ");
                printf("%d", cur->keys[i]);
            }
            printf("] -> ");
            cur = cur->next;
        }
        printf("NULL\n");
    }
}