# Online Shopping System Simulation

This repository contains a C-based interactive program simulating an **online shopping system**. The project demonstrates advanced data structure usage, dynamic memory management, and system-level programming in C.

## **Features**
- Parses product data (SKU, description, price) into a **linked list** to create an inventory system.
- Interactive mode allowing users to:
  - Search products by SKU or description.
  - Add products to a shopping cart.
  - Sell and manage cart items with real-time updates to inventory.
- Implements sorting by **price** or **description** with options for ascending/descending order.
- Tax rate customization and dynamic cart calculations.

## **Project Highlights**
- **Data Structures:** 
  - Utilizes linked lists to manage products and cart items efficiently.
  - Implements custom sorting and filtering logic using comparator functions.
- **Dynamic Memory Management:** 
  - Handles allocation, reallocation, and freeing of memory for product descriptions and cart items.
- **System-Level Programming:**
  - Redirects input streams and manages file I/O for seamless user interaction.

## **How to Run**
1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/C_Online_Shopping_Simulation.git
   cd C_Online_Shopping_Simulation

2. Build the program:
  ```bash
  make

3. Run the program with sample input:
  ```bash
   ./shopping_simulation < input.txt

- Input data is provided in input.txt

## **Usage Examples**
- **Command-line Options:** 
  -p: Sort by price.
  -d: Sort by description.
  -r: Reverse order.
  -f <file>: Load products from a file.
  -t <tax_rate>: Set custom tax rate.

- **Interactive Commands:** 
  - SKU: Search for a product by SKU.
  - L: List all products in the inventory.
  - S <SKU>: Sell an item by SKU.
  - C <SKU>: Add an item to the shopping cart.
  - C s: Sell all items in the cart.
  - C e: Empty the cart.
  - Q: Quit the program.

## **Sample Input**
  225 Harmonica
  325 Oboe reeds
  404 7.99
  325 24.99
  Harmonica
  404 Drumsticks
  8501 French bow 82.99
  225 3.99

## **Project Files**
  - project3.c and project3.h: Main program logic.
  - product.c and product.h: Product management functions.
  - cart.c and cart.h: Shopping cart functionality.
  - Makefile: Build configuration.
  - input.txt: Sample input for testing.







