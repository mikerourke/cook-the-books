// ------------------------------------------------------------------- 
// SECTION 1: DECLARATIONS 
// ------------------------------------------------------------------- 
#include <stdlib.h>

struct Node {
  int id;
  int categoryId;
  float rawAmount;
  float cookedAmount;
  struct Node *next;
  struct Node *prev;
};

typedef enum {
  RAW = 1,
  COOKED = 2
} AmountType;

struct Node *transactionsHead = NULL;
struct Node *categoriesHead = NULL;

// ------------------------------------------------------------------- 
// SECTION 2: LINKED LIST OPERATIONS 
// ------------------------------------------------------------------- 
void deleteNode(struct Node **headNode, struct Node *delNode) {
    // Base case:
    if (*headNode == NULL || delNode == NULL) return;

    // If node to be deleted is head node:
    if (*headNode == delNode) *headNode = delNode->next;

    // Change next only if node to be deleted is NOT the last node:
    if (delNode->next != NULL) delNode->next->prev = delNode->prev;

    // Change prev only if node to be deleted is NOT the first node:
    if (delNode->prev != NULL) delNode->prev->next = delNode->next;

    // Finally, free the memory occupied by delNode:
    free(delNode);
}

void appendNode(struct Node **headNode, int id, int categoryId,
                float rawAmount, float cookedAmount) {
    // 1. Allocate node:
    struct Node *newNode = (struct Node *) malloc(sizeof(struct Node));
    struct Node *last = *headNode; // Used in Step 5

    // 2. Populate with data:
    newNode->id = id;
    newNode->categoryId = categoryId;
    newNode->rawAmount = rawAmount;
    newNode->cookedAmount = cookedAmount;

    // 3. This new node is going to be the last node, so make next NULL:
    newNode->next = NULL;

    // 4. If the Linked List is empty, then make the new node as head:
    if (*headNode == NULL) {
        newNode->prev = NULL;
        *headNode = newNode;
        return;
    }

    // 5. Otherwise, traverse till the last node:
    while (last->next != NULL) {
        last = last->next;
    }

    // 6. Change the next of last node:
    last->next = newNode;

    // 7. Make last node as previous of new node:
    newNode->prev = last;
}

// ------------------------------------------------------------------- 
// SECTION 3: TRANSACTIONS OPERATIONS 
// ------------------------------------------------------------------- 
struct Node *findNodeById(int id, struct Node *withinNode) {
    struct Node *node = withinNode;
    while (node != NULL) {
        if (node->id == id) return node;
        node = node->next;
    }
    return NULL;
}

void addTransaction(int id, int categoryId, float rawAmount,
                    float cookedAmount) {
    appendNode(&transactionsHead, id, categoryId, rawAmount, cookedAmount);
}

void editTransaction(int id, int categoryId, float rawAmount,
                     float cookedAmount) {
    struct Node *foundNode = findNodeById(id, transactionsHead);
    if (foundNode != NULL) {
        foundNode->categoryId = categoryId;
        foundNode->rawAmount = rawAmount;
        foundNode->cookedAmount = cookedAmount;
    }
}

void removeTransaction(int id) {
    struct Node *foundNode = findNodeById(id, transactionsHead);
    if (foundNode != NULL) deleteNode(&transactionsHead, foundNode);
}

// -------------------------------------------------------------------
// SECTION 4: TRANSACTIONS CALCULATIONS
// -------------------------------------------------------------------
void calculateGrandTotals(float *totalRaw, float *totalCooked) {
    struct Node *node = transactionsHead;
    while (node != NULL) {
        *totalRaw += node->rawAmount;
        *totalCooked += node->cookedAmount;
        node = node->next;
    }
}

float getGrandTotalForType(AmountType type) {
    float totalRaw = 0;
    float totalCooked = 0;
    calculateGrandTotals(&totalRaw, &totalCooked);

    if (type == RAW) return totalRaw;
    if (type == COOKED) return totalCooked;
    return 0;
}

float getFinalBalanceForType(AmountType type, float initialBalance) {
    float totalForType = getGrandTotalForType(type);
    return initialBalance + totalForType;
}

// -------------------------------------------------------------------
// SECTION 5: CATEGORIES CALCULATIONS
// -------------------------------------------------------------------
void upsertCategoryNode(int categoryId, float transactionRaw,
                        float transactionCooked) {
    struct Node *foundNode = findNodeById(categoryId, categoriesHead);
    if (foundNode != NULL) {
        foundNode->rawAmount += transactionRaw;
        foundNode->cookedAmount += transactionCooked;
    } else {
        appendNode(&categoriesHead, categoryId, categoryId, transactionRaw,
                   transactionCooked);
    }
}

void buildValuesByCategoryList() {
    struct Node *node = transactionsHead;
    while (node != NULL) {
        upsertCategoryNode(node->categoryId, node->rawAmount,
                           node->cookedAmount);
        node = node->next;
    }
}

void recalculateForCategories() {
    categoriesHead = NULL;
    buildValuesByCategoryList();
}

float getTotalForTypeAndCategory(AmountType type, int categoryId) {
    // Ensure the category totals have been calculated:
    if (categoriesHead == NULL) buildValuesByCategoryList();

    struct Node *categoryNode = findNodeById(categoryId, categoriesHead);
    if (categoryNode == NULL) return 0;

    if (type == RAW) return categoryNode->rawAmount;
    if (type == COOKED) return categoryNode->cookedAmount;
    return 0;
}
