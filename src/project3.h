// project3.h

#ifndef PROJECT3_H
#define PROJECT3_H

#include "product.h"
#include "cart.h"

#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>  // I added this to avoid a warning
#include <unistd.h>
#include <sys/file.h>

#define TAX_RATE 0.08625 // Assuming 8.625% tax rate, adjust accordingly



/*
  Reads in a line (Dynamic)
*/
int krgetline(char** dest);

/*
  Checks to see if a string is an integer
*/
int isInteger(const char *str);

/*
  Checks to see if a string is a double
*/

int isDouble(const char *str);

/*
  krFileCopy: copies the contents of a file to another file stream
*/
void krFileCopy(FILE *ifp, FILE *ofp);


#endif
