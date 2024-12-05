// cart.c 

#include "cart.h"


// Function to initialize a shopping cart
// Serves as the head node
void initializeCart(ShoppingCart* cart) {
	cart->cartProduct = NULL;
    cart->numIteminCart = 1;
    cart->numIteminStore = 0;
    cart->nextCartItem = NULL;
}

// Function to add a product to the cart
void addToCart(ShoppingCart* cart, Product* productToAdd, int numIteminCart) {

    // Check if the product is already in the cart
    for (ShoppingCart* currentCartItem = cart->nextCartItem; currentCartItem != NULL; currentCartItem = currentCartItem->nextCartItem) {
        if (currentCartItem->cartProduct->sku == productToAdd->sku) {

        	if(currentCartItem->numIteminStore != 0){
	            currentCartItem->numIteminCart += numIteminCart;
	            currentCartItem->numIteminStore--;
	            printf("%d %s in cart\n", currentCartItem->numIteminCart, productToAdd->description);
	            return;
	        }

	        else{ 
	        	printf("No more %s to add to the Cart\n", productToAdd->description);
	        	return;
	    	}
        }
    }

    if(productToAdd->quantity != 0){
	    // Product not found in the cart, add a new entry
	    ShoppingCart* newCartItem = (ShoppingCart*)malloc(sizeof(ShoppingCart));
	    newCartItem->cartProduct = productToAdd;
	    newCartItem->numIteminCart = numIteminCart;
	    newCartItem->numIteminStore = productToAdd->quantity - 1;

	    ShoppingCart* current = cart;

	    // Traverse the list to find the last node
	    while (current->nextCartItem != NULL) {
	        current = current->nextCartItem;
	    }

	    // Add the new product to the end
	    current->nextCartItem = newCartItem;
	}
	else{
		printf("No more %s to add to the Cart\n", productToAdd->description);
	    return;
	}

    printf("%d %s in cart\n", cart->numIteminCart, productToAdd->description);
}

// Function to display items in the cart
void displayCart(const struct ShoppingCart* cart) {
    if (cart->nextCartItem == NULL) {
        printf("Cart is empty\n");
        return;
    }

    for (ShoppingCart* currentCartItem = cart->nextCartItem; currentCartItem != NULL; currentCartItem = currentCartItem->nextCartItem) {
    	
        printf("%d %s $%.2f: %d %s in cart", currentCartItem->cartProduct->sku,
               currentCartItem->cartProduct->description, currentCartItem->cartProduct->price,
               currentCartItem->numIteminCart, currentCartItem->cartProduct->description);
        printf("\n");
    }
}

//Function to sell all the items in the cart;
void sellCart(ShoppingCart* cart, double userSetTaxRate, int tswitch){
	double subtotal = 0.00;
	for (ShoppingCart* currentCartItem = cart->nextCartItem; currentCartItem != NULL; currentCartItem = currentCartItem->nextCartItem) {

		if(currentCartItem->cartProduct->quantity < currentCartItem->numIteminCart){
			printf("Seems like a customer just bought an item in your cart\nCart has been Emptied... Please redo your Cart\n\n");

			freeCart(cart);
			initializeCart(cart);
			return;
		}
		

		subtotal+= currentCartItem->cartProduct->price;

        printf("%d %s $%.2f x %d\n", 
       			currentCartItem->cartProduct->sku,
            	currentCartItem->cartProduct->description, 
            	currentCartItem->cartProduct->price,
            	currentCartItem->numIteminCart
              );

        currentCartItem->cartProduct->quantity -= currentCartItem->numIteminCart;
    }

    // Calculate total price with tax
    double totalPrice = (tswitch) ? subtotal + (subtotal*userSetTaxRate)  : subtotal + (subtotal*TAX_RATE);

    double amountOfTax = (tswitch) ? subtotal * userSetTaxRate : subtotal * TAX_RATE;

    double selectedTaxRate = (tswitch) ? userSetTaxRate * 100 : TAX_RATE * 100;

    printf("(%.3f%%) sales tax: $%.2f\nTotal price: $%.2f\n", selectedTaxRate, amountOfTax, totalPrice );

    printf("Cart Sold and Emptied\n\n");

    freeCart(cart);
	initializeCart(cart);


}

// Function to put the cart back (Free the memory used)
void freeCart(ShoppingCart* cart) {
     
    for (ShoppingCart* currentCartItem = cart->nextCartItem; currentCartItem != NULL; currentCartItem = currentCartItem->nextCartItem){
    	currentCartItem->cartProduct= NULL;
    	free(currentCartItem);
    }
  
}
