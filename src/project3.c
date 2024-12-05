/*
* Marcus Valbrun (N00920668@students.ncc.edu)
* CSC217 D1 23FA
* Project #3: Inventory, Part 2
* Due 2023-18-12 11:59 pm
* project3.c
*/

#include "project3.h"

/*
main: Project 3 Start: Inventory, Part 2
		Reads an unknown amount of product information keys (sku, description, price)
        from stdin and line by line creates a list of products with said info
        no matter how parsed or separated those three keys are.

        Enters Interactive Mode

        returns: 0 If the program ends successfully
*/ 
int main(int argc, char* argv[]) {

    /* START OF HANDLING SWITCHES  */

        // Variables to track switches
        int orderByPrice = 0;
        int orderByDescription = 0;
        int reverseOrder = 0;
        char *userFileName = NULL; // Variable to store the filename
        char *outputFileName = NULL; // Variable to store the output filename
        char *cartOutputFileName = NULL; // Variable to store the cart output filename
        int outputFileFD = -1;
        int cartOutputFD = -1;
        double userSetTaxRate = 0.00; int tswitch = 0;


        // Iterate through the arguments to handle various combinations of switches
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-p") == 0) {
                orderByPrice = 1;
            } 
            else if (strcmp(argv[i], "-d") == 0) {
                orderByDescription = 1;
            } 
            else if (strcmp(argv[i], "-r") == 0) {
                reverseOrder = 1;
            } 
            else if (strcmp(argv[i], "-pr") == 0 || strcmp(argv[i], "-rp") == 0) {
                reverseOrder = 1;
                orderByPrice = 1;
            } 
            else if (strcmp(argv[i], "-dr") == 0 || strcmp(argv[i], "-rd") == 0) {
                reverseOrder = 1;
                orderByDescription = 1;
            } 
            else if (strcmp(argv[i], "-f") == 0) {
                // Check if there is a filename following -f
                if (i + 1 < argc) {
                    userFileName = argv[++i];
                } 
                else {
                    // Error: -f supplied without a filename
                    fprintf(stderr, "Error: -f switch requires a filename immediately following it\n");
                    exit(EXIT_FAILURE);
                }
            } 
            else if (strcmp(argv[i], "-o") == 0) {
                // Check if there is an output filename following -o
                if (i + 1 < argc) {
                    outputFileName = argv[++i];
                } 
                else {
                    // Error: -o supplied without a filename
                    fprintf(stderr, "Error: -o switch requires a filename immediately following it\n");
                    exit(EXIT_FAILURE);
                }
            }
            else if (strcmp(argv[i], "-c") == 0) {
                // Check if there is an output filename following -o
                if (i + 1 < argc) {
                    cartOutputFileName = argv[++i];
                } 
                else {
                    // Error: -o supplied without a filename
                    fprintf(stderr, "Error: -o switch requires a filename immediately following it\n");
                    exit(EXIT_FAILURE);
                }
            }
            else if (strcmp(argv[i], "-t") == 0) {
                tswitch = 1;
                // Check if there is an output filename following -o
                if (i + 1 < argc) {
                    userSetTaxRate = atof( argv[++i] ) / 100;
                } 
                else {
                    // Error: -o supplied without a filename
                    fprintf(stderr, "Error: -t switch requires a tax rate in the form of a double following it\n");
                    exit(EXIT_FAILURE);
                }
            }
            else {
                // Handle unrecognized switches
                fprintf(stderr, "Error: Unrecognized switch: %s\n", argv[i]);
                exit(EXIT_FAILURE);
            }
        }

        // Check for conflicting switches
        if (orderByPrice && orderByDescription) {
            fprintf(stderr, "Error: Cannot specify both -p and -d switches\n");
            exit(EXIT_FAILURE);
        }

        if (reverseOrder && !orderByPrice && !orderByDescription) {
            fprintf(stderr, "Error: -r switch must be used with either -p or -d\n");
            exit(EXIT_FAILURE);
        }


        // Check if -f was supplied and process the file
        if (userFileName != NULL) {
            int stockInputFD = open(userFileName, O_RDONLY);

            if (stockInputFD == -1) {
                // Error: File not found or not readable
                fprintf(stderr, "Error: Unable to open file '%s' for reading\n", userFileName);
                exit(EXIT_FAILURE);
            }

            // Make the User input file stream the same as stdin's file stream the program can run as normal from Project 2
            int inputFileDescriptor = open(userFileName, O_RDONLY);
            dup2(inputFileDescriptor, 0); /* make stdin same as inputFileDescriptor */
        }

        // Check if -o was supplied and process the file
        if (outputFileName != NULL) {
            outputFileFD = open(outputFileName, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (outputFileFD == -1) {
                // Error: File not writable
                fprintf(stderr, "Error: Unable to open file '%s' for writing\n", outputFileName);
                exit(EXIT_FAILURE);
            }
        }
        // Check if -c was supplied and process the file
        if (cartOutputFileName != NULL) {
            cartOutputFD = open(cartOutputFileName, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (cartOutputFD == -1) {
                // Error: File not writable
                fprintf(stderr, "Error: Unable to open file '%s' for writing\n", outputFileName);
                exit(EXIT_FAILURE);
            }
        }

    /* END OF HANDLING SWITCHES */


    /* START OF PRODUCT LIST CREATION FROM STDIN */

    	Product* items = createProduct(000, 0.00, 0); // initial
        
        char* currentLine = NULL; // An array of pointers

        //Grab the first Line and add it to the items list.
        krgetline(&currentLine);

        char* token = strtok(currentLine, " \t\n");

        // tokenize the line to get the product info keys (sku, price, description)
        while (token != NULL) {
            if (isInteger(token)) {
                items->sku = atoi(token);
            } else if (isDouble(token)) {
                items->price = atof(token);
            } else {
                addDescription(items, token);
            }

            // Get the next token
            token = strtok(NULL, " \t\n");
        }// end tokenize loop

        items->quantity++; //update the quantity of that item

        // Do the same for the rest of the lines until there are no more
        while (krgetline(&currentLine) != 0) {
            
            // Tokenize the line
            char* token = strtok(currentLine, " \t\n");

            // Temporary variables to store product info keys
            int tempSku = 000;
            double tempPrice = 0.00;
            char tempDesc[strlen(currentLine) + 10]; // Dynamically initialize length to 
            tempDesc[0] = '\0';                       // length of line plus some... more than enough


            while (token != NULL) {
                if(isInteger(token)) tempSku = atoi(token);
                else if(isDouble(token)) tempPrice = atof(token);
                else { 
                // Concatenate the non-integer/non-double token to the temp string
                    strcat(tempDesc, token);
                    strcat(tempDesc, " ");
                }

                // Get the next token
                token = strtok(NULL, " \t\n");
            }

             // Trim trailing whitespace from the temp string
            if (strlen(tempDesc) > 0) {
                tempDesc[strlen(tempDesc) - 1] = '\0';
            }

            int match = 0; // bool value to determine if a match 
                           // between the list of items & the line was found

            /* 
            Iterate through the list of items using a traveser
            If any of the temp product keys matches one of the products keys
                update quantity of that list
                store the other keys info if they were entered as well
                update match then move on to the next line
            */  
            for (Product* currentItem = items; currentItem != NULL; currentItem = currentItem->next) {

                if (tempSku == currentItem->sku) {
                    if(tempPrice != 0.00) currentItem->price = tempPrice;
                    if( (strcmp(tempDesc, "") != 0) && (strcmp(currentItem->description, "") == 0) ) addDescription(currentItem,tempDesc);
                    currentItem->quantity++;
                    match = 1;
                    break;   
                } 

                else if ( ( tempPrice == currentItem->price && ( strcmp(tempDesc, currentItem->description) == 0 ) ) 
                       || ( tempPrice == currentItem->price &&  (tempSku == currentItem->sku) ) )  
                {
                    if(tempSku != 000) currentItem->sku = tempSku;
                    if( (strcmp(tempDesc, "") != 0) && (strcmp(currentItem->description, "") == 0) ) addDescription(currentItem,tempDesc);
                    currentItem->quantity++;
                    match = 1;
                    break;
                }
                
                else if ( strcmp(tempDesc, currentItem->description) == 0 ){
                    if(tempSku != 000) currentItem->sku = tempSku;
                    if(tempPrice != 0.00) currentItem->price = tempPrice;
                    currentItem->quantity++;
                    match = 1;
                    break;
                }   
            }

            //if there was no match found in the line... 
            //its a new product... create it and add it to the end of list.
            if (match == 0) {
                Product* newItem = createProduct(tempSku, tempPrice, 1);
                if(strcmp(tempDesc, "") != 0) addDescription(newItem,tempDesc);
                addProductToEnd(items, newItem);
            }

            free(currentLine); // Free the allocated memory for each line

        }// end of line grabber loop 



    /* END OF PRODUCT LIST CREATION */



    /* START OF PROJECT 3 INTERACTIVE MODE */

        // Redirect input back from terminal
       freopen("/dev/tty", "r", stdin);

        printf("Enter 'Q' or 'q' to quit\n");

        char* userInput = NULL;
        

        // Temporary variables to store user inputs
        int userSku = 000;
        char* userDesc = NULL;

        
        ShoppingCart userCart; 
        initializeCart(&userCart);

        // Loop until the user enters "Q" or "q"
        while (krgetline(&userInput)) {

            userDesc = (char*)malloc(sizeof(&userInput));

            // SKU SEARCH 
            if (isInteger(userInput)) {

                userSku = atoi(userInput);
                int noMatch = 1;

                for (Product* currentItem = items; currentItem != NULL; currentItem = currentItem->next) {
                    if (userSku == currentItem->sku) {
                        printProduct(*currentItem);
                        noMatch = 0;
                        break; // Exit loop once the product is found
                    }     
                }

                if(noMatch) printf("%d: Unknown SKU\n", userSku);
            }

            //END OF SKU SEARCH

            // L PRINT CHECK
            else if (strcasecmp(userInput, "l\n") == 0) {
                if(outputFileFD){
                    // Make the standard output (stdout) the same as outputFileFD
                    dup2(outputFileFD, 1); /* make stdout same as outputFileFD */
                }

                // Sorting logic based on switches
                if (orderByPrice) {
                    sortAndPrint(items, compareByPrice, reverseOrder);
                    printf("\n");
                } else if (orderByDescription) {
                    sortAndPrint(items, compareByDescription, reverseOrder);
                    printf("\n");
                } else {
                    // Default behavior, print the original list
                    printList(items);
                    printf("\n");
                }


                if(outputFileFD){
                    // Redirect output back to terminal
                    freopen("/dev/tty", "a", stdout);
                }

            } //END OF L


            // "S SKU" CHECK AKA SELL AN ITEM CHECK 
            else if (sscanf(userInput, "S %d", &userSku) == 1 || sscanf(userInput, "s %d", &userSku) == 1) {
                // Check if the SKU is provided
                if (userSku <= 0) {
                    printf("s: You didn't specify a SKU\n");
                } 
                else {
                    int noMatch = 1;

                    for (Product* currentItem = items; currentItem != NULL; currentItem = currentItem->next) {

                        if (userSku == currentItem->sku) {

                            if (currentItem->quantity > 0) {

                                // Calculate total price with tax
                                double totalPrice = (tswitch) ? currentItem->price * (1 + userSetTaxRate)  : currentItem->price * (1 + TAX_RATE);

                                double amountOfTax = (tswitch) ? currentItem->price * userSetTaxRate : currentItem->price * TAX_RATE;

                                double selectedTaxRate = (tswitch) ? userSetTaxRate * 100 : TAX_RATE * 100;


                                printf("%d %s $%.2f + $%.2f tax (%.3f%%) = $%.2f\n",
                                       currentItem->sku, currentItem->description, currentItem->price,
                                       amountOfTax, selectedTaxRate, totalPrice);

                                // Update stock count
                                currentItem->quantity--;

                                // Print updated stock count
                                printf("%d %s $%.2f: %d in stock\n", currentItem->sku, currentItem->description,
                                       currentItem->price, currentItem->quantity);

                            } 

                            else printf("%d %s: Sold out\n", currentItem->sku, currentItem->description);
                            
                            noMatch = 0;
                            break; // Exit loop once the product is found
                        }
                    }

                    if (noMatch) printf("%d: Unknown SKU\n", userSku);
                    
                }
            } 

            // "S SKU" CHECK AKA SELL AN ITEM CHECK 
            else if (sscanf(userInput, "S %[^\n]", userDesc) == 1 || sscanf(userInput, "s %[^\n]", userDesc) == 1) {
                // Check if the Description is provided
                if (sizeof(userDesc) < 2) {
                    printf("s: You didn't specify a Description\n");
                } 
                else {
                    int noMatch = 1;

                    for (Product* currentItem = items; currentItem != NULL; currentItem = currentItem->next) {

                        if (strcasecmp(userDesc,currentItem->description) == 0) {

                            if (currentItem->quantity > 0) {

                                // Calculate total price with tax
                                double totalPrice = (tswitch) ? currentItem->price * (1 + userSetTaxRate)  : currentItem->price * (1 + TAX_RATE);

                                double amountOfTax = (tswitch) ? currentItem->price * userSetTaxRate : currentItem->price * TAX_RATE;

                                double selectedTaxRate = (tswitch) ? userSetTaxRate * 100 : TAX_RATE * 100;


                                printf("%d %s $%.2f + $%.2f tax (%.3f%%) = $%.2f\n",
                                       currentItem->sku, currentItem->description, currentItem->price,
                                       amountOfTax, selectedTaxRate, totalPrice);

                                // Update stock count
                                currentItem->quantity--;

                                // Print updated stock count
                                printf("%d %s $%.2f: %d in stock\n", currentItem->sku, currentItem->description,
                                       currentItem->price, currentItem->quantity);

                            } 

                            else printf("%d %s: Sold out\n", currentItem->sku, currentItem->description);
                            
                            noMatch = 0;
                            break; // Exit loop once the product is found
                        }
                    }

                    if (noMatch) printf("%s: Unknown Description\n", userDesc);
                    
                }
            } 


            // "S SKU" CHECK but NO N 
            else if (strcasecmp(userInput, "s\n") == 0) printf("s: You didnt't specify a SKU or Description\n");


            //END OF S


            // "C " SHOPPING CART CHECK
            // If the user just enters "C" or "c" display the current state of the cart
            else if (strcasecmp(userInput, "c\n") == 0) displayCart(&userCart);

                //C VIA SKU
            else if (sscanf(userInput, "C %d", &userSku) == 1 || sscanf(userInput, "c %d", &userSku) == 1) {
                    int noMatch = 1;

                    for (Product* currentItem = items; currentItem != NULL; currentItem = currentItem->next) {

                        if (userSku == currentItem->sku) {
                            addToCart(&userCart, currentItem, 1);
                            noMatch = 0;
                            break; // Exit loop once the product is found
                        }

                    }

                    if (noMatch) printf("%d: Unknown SKU\n", userSku);
            }


                //C VIA DESCRIPTION
            else if (sscanf(userInput, "C %[^\n]", userDesc) == 1 || sscanf(userInput, "c %[^\n]", userDesc) == 1) {

                    //C S AKA DISPLAY CART CHECK

                if(strcasecmp(userDesc, "s") == 0){

                    if(cartOutputFD){
                        // Make the standard output (stdout) the same as outputFileFD
                        dup2(cartOutputFD, 1); /* make stdout same as outputFileFD */
                    }

                        if(userCart.nextCartItem != NULL) sellCart(&userCart, userSetTaxRate, tswitch);
                
                        else printf("Cart is empty\n");

                    if(cartOutputFD){
                        // Redirect output back to terminal
                        freopen("/dev/tty", "a", stdout);
                    }   

                }

                    // C E AKA EMPTY CART CHECK
                else if (strcasecmp(userDesc, "e") == 0 ){

                    if(userCart.nextCartItem != NULL){
                        freeCart(&userCart);
                        initializeCart(&userCart);
                        printf("Cart has been emptied, Items have been put back\n");
                    }
                    
                    else printf("Cart is already empty\n");

                }

                    // C ADD TO CART VIA DESC CHECK
                else{

                    int noMatch = 1;

                    for (Product* currentItem = items; currentItem != NULL; currentItem = currentItem->next) {

                        if (strcasecmp(userDesc,currentItem->description) == 0) {
                            addToCart(&userCart, currentItem, 1);
                            noMatch = 0;
                            break; // Exit loop once the product is found
                        }
                    }

                    if ( noMatch ) printf("%s: Unknown Description\n", userDesc);
                }

            }

            // END OF " C " SHOPPING CART CHECK



            // QUIT CHECK
             else if (strcasecmp(userInput, "q\n") == 0) {
                free(userInput);
                printf("%s\n", "Thank you for using my program.");
                exit(0);
            }


            //DESC SEARCH / OTHER SEARCH
            else {
                int noMatch = 1;

                for (Product* currentItem = items; currentItem != NULL; currentItem = currentItem->next) {
                    char* tempDesc = NULL;
                    tempDesc = (char*)malloc(sizeof(currentItem->description + 2));
                    strcpy(tempDesc, currentItem->description);
                    strcat(tempDesc, "\n");
                    if (strcasecmp(userInput, tempDesc) == 0) {
                        printProduct(*currentItem);
                        noMatch = 0;
                        free(tempDesc);
                        break; // Exit loop once the product is found
                    }     
                }
                if(noMatch) printf("%d: Unknown Description or Unrecodnizable Input\n", userSku);
            }


            // Free allocated memory for userInput
            free(userInput);
            free(userDesc);
        }

    /* END OF PROJECT 3 INTERACTIVE MODE */

	// Free allocated memory
    freeList(items);
    freeCart(&userCart); 
 	return 0; // This line will never be reached because the loop exits with a return statement
	
} // end of program
            



// BEGNINNING OF FUNCTION DEFINITIONS


/*
  krFileCopy: copies the contents of a file to another file stream
*/
void krFileCopy(FILE *ifp, FILE *ofp) {
    int c;

    while ((c = getc(ifp)) != EOF)
        putc(c, ofp);
}


/* 
  krgetline: reads a line from stdin into a array of char dest... no limit
  param dest: Array of char ptrs ... the destination where the line from input will be written
  returns: the length of the line that was read
*/
int krgetline(char** dest) {
    int c = 0;
    int i = 0;

    // Allocate memory for the line
    *dest = (char*)malloc(1); // Start with a small allocation

    while ((c = getchar()) != EOF && c != '\n') {
        (*dest)[i] = c;

        // Increase the size of the allocated memory if needed
        *dest = (char*)realloc(*dest, i + 2);

        ++i;
    }

    if (c == '\n') {
        (*dest)[i] = c;
        ++i;
    }

    (*dest)[i] = '\0'; // Null-terminate the line

    return i;
} //end of krgetline()


/*
  Checks to see if a string is an integer
  returns 1: If it's an integer
  returns 0: If it's not an integer
*/
int isInteger(const char *str) {
    if (str == NULL || *str == '\0') {
        return 0; // Not an integer
    }

    while (*str != '\0') {
        if (!isdigit(*str) && *str != '\n') {
            return 0;
        }
        str++;
    }

    return 1;
}


/*
  Checks to see if a string is a double
  returns 1: If it's an integer
  returns 0: If it's not an integer
*/
int isDouble(const char *str) {
    char *endptr;
    strtod(str, &endptr);

    if (*endptr != '\0') {
        return 0; 
    }

    return 1;
} // end of isDouble()

