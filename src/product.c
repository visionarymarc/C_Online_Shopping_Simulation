// product.c



#include "product.h"

/*
  Constructor for Product

  param sku: a user-specified integer to be the sku key for product
  param price: a user-specified double to be the price key for product
  param quantitiy: a user-specified integer to be the quantity of products created

  returns: a new product
  exits: if memory fails to allocate to hold the new product
*/
Product* createProduct(int sku, double price, int quantity) {
    Product* newItem = (Product*)malloc(sizeof(Product));
    if (newItem == NULL) {
        // Handle memory allocation failure
        exit(EXIT_FAILURE);
    }

    newItem->sku = sku;
    newItem->price = price;
    newItem->quantity = quantity;
    newItem->next = NULL;

    // Initialize the description as an empty string
    newItem->description = strdup("");
    if (newItem->description == NULL) {
        // Handle memory allocation failure
        exit(EXIT_FAILURE);
    }

    return newItem;
}


/*
  Adds a description to a user-specified product...
  Mainly to be where a full description is added word by word

  param item: the user-specified product the description will be added to
  param word: a user-specified word (recommended) or phrase to be added to the item

  exits: if memory fails to allocate to hold the new description
*/
void addDescription(Product* item, const char* word) {
    // Calculate the new length of the description
    size_t oldLength = item->description ? strlen(item->description) : 0;
    size_t wordLength = strlen(word);
    size_t newLength = oldLength + wordLength + 1; // 1 for null terminator

    // Reallocate memory for the updated description
    item->description = (char*)realloc(item->description, newLength);
    if (item->description == NULL) {
        // Handle memory allocation failure
        exit(EXIT_FAILURE);
    }

    // Concatenate the new word to the description
    if (oldLength > 0) {
        strcat(item->description, " "); // Separate words with a space if the description is not empty
    }
    strcat(item->description, word);

    // Trim trailing whitespace
    size_t len = newLength;
    while (len > 0 && isspace(item->description[len - 1])) {
        --len;
    }

    // Terminate the string at the last non-whitespace character
    item->description[len] = '\0';
    
} // end of addDescription

/*
  Adds a product to the end of a list of products
  Accessed through the first product in the list

  param head: the user-specified first product of the list to be traversed
  param newProduct: the user-specified new product to be added to the end of the list
*/
void addProductToEnd(Product* head, Product* newProduct) {
    Product* current = head;

    // Traverse the list to find the last node
    while (current->next != NULL) {
        current = current->next;
    }

    // Add the new product to the end
    current->next = newProduct;
}


/*
 Counts the number of Products in the list 

 param items: the user-specified first product of the list to be traversed

 returns: the count it counted
*/
int countProducts(Product* items) {
    int count = 0;

    //Iterates through the list and increments the count for each product, returning the total count.
    for (Product* currentItem = items; currentItem != NULL; currentItem = currentItem->next) {
        count++;
    }
    return count;
}



/*
 Compares two products based on their prices. 
 Uses strcmp to compare the descriptions of two products. 
 To be used with the sortAndPrint function as comparators for qsort.

 param a,b: Pointers to the products being compared

            const keyword in this context is used for declaring constant pointers 
            to data and signals that the comparator functions 
            won't modify the elements they are comparing.

 Returns: a positive value if the first product's price is greater,
          a negative value if it's smaller, and 
          zero if they are equal.

Note:
        The sortAndPrint function uses this function return value as a comparator internally, 
        Therefore in main, the function doesn't need additional parameters because qsort in sortAndPrint
        takes care of passing the necessary elements to the comparator during the sorting process.
*/
int compareByPrice(const void* a, const void* b) {
    const Product* productA = *(const Product**)a;
    const Product* productB = *(const Product**)b;

    return (productA->price > productB->price) - (productA->price < productB->price);
}



/*
 Compares two products based on their description alphabetically.
 Uses strcmp to compare the descriptions of two products. 
 To be used with the sortAndPrint function as comparators for qsort.

 param a,b: Pointers to the products being compared.

            const keyword in this context is used for declaring constant pointers 
            to data and signals that the comparator functions 
            won't modify the elements they are comparing.

 Returns: a positive value if the first description is lexicographically greater, 
          a negative value if it's smaller, and 
          zero if they are equal.

Note:
        The sortAndPrint function uses this function return value as a comparator internally, 
        Therefore in main, the function doesn't need additional parameters because qsort in sortAndPrint
        takes care of passing the necessary elements to the comparator during the sorting process.
*/
int compareByDescription(const void* a, const void* b) {
    const Product* productA = *(const Product**)a;
    const Product* productB = *(const Product**)b;

    return strcmp(productA->description, productB->description);
}


/* 
  Reverses the order of the array 

  param arr: The array of products.
  param length: The length of the array.
*/ 
void reverseArray(Product* arr[], int length) {
    int start = 0;
    int end = length - 1;

    /* Swap elements from the start and end until they meet in the middle.
    */
    while (start < end) {
        Product* temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;

        start++;
        end--;
    }
}

/* 
  Frees the memory allocated for each product in the list, including their descriptions.
  param items: the user-specified first product of the list to be traversed
*/
void freeList(Product* items) {
    Product* current = items;
    while (current != NULL) {
        Product* next = current->next;
        free(current->description);
        free(current);
        current = next;
    }
}


/* 
  Prints an array of Products using the printProduct function.

  param arr: The array of products.
  param length: The length of the array.
*/
void printArray(Product* arr[], int length) {
    for (int i = 0; i < length; i++) {
        printProduct(*(arr[i]));
    }
}



/*
  Sort and print the list based on a given comparator.

  Parameters:
    items: The first product in the list.

    comparator: The comparator function for sorting.
                Defines the order of elements used in qsort

    reverseOrder: Flag to indicate whether to reverse the order.

*/
void sortAndPrint(Product* items, int (*comparator)(const void*, const void*), int reverseOrder) {
    int numProducts = countProducts(items);
    Product* productArray[numProducts];
    int index = 0;

    // Copy items to productArray
    for (Product* currentItem = items; currentItem != NULL; currentItem = currentItem->next) {
        productArray[index] = currentItem;
        index++;
    }

    /*
      qsort: Sorts an array of Product pointers using the provided comparator function.
     
      Parameters:
                productArray: The array of Product pointers to be sorted.
                numProducts: The number of products in the array.
                comparator: The comparison function used to determine the order of the elements.

    */
    qsort(productArray, numProducts, sizeof(Product*), comparator);

    // If reverseOrder is true, reverse the order of the array
    if (reverseOrder) {
        reverseArray(productArray, numProducts);
    }

    // Print the sorted list
    printArray(productArray, numProducts);
}



/*
  Prints a Product's info keys (sku, description, price and quantity)
  param item: the user-specified product to be printed
*/
void printProduct(Product item) {
    printf("%i %s $%.2f: %d in stock\n", item.sku, item.description, item.price, item.quantity);
}



/*
  Prints an Original List of Product's info keys (sku, description, price and quantity)
  param head: the user-specified first product of the list to be traversed as it prints
*/
void printList(Product* head) {
    Product* current = head;

    while (current != NULL) {
        printProduct(*current);
        current = current->next;
    }
}
