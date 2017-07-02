#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h> 

#define MAXCHAR 20

/* STRUCTURES */
typedef struct PRODUCT{
	int code;
	char name[MAXCHAR];
	float price;
} PRODUCT;
/* END STRUCTURES */


/* AUXILIARY FUNCTION HEADERS */
void insertOption();
void printOption();
/* END OF AUXILIARY FUNCTION HEADERS */


void main() {
	int option;
		do{
			printf("\n1 - Insert new product\n2 - List all products\n0 - Exit\n");
			scanf("%d", &option);
			if ( option == 1 ) {
				insertOption();
			}

			else if ( option == 2){
				printOption();
			}

			else if( option == 0) { 
				return;
			}
	}while (1);
}

void insertOption() {

	PRODUCT *product = malloc (sizeof(PRODUCT));
	FILE *file = fopen("products.dat", "a+");

	printf("insert the name, code and price of the product, pressing enter at each step: ");

	__fpurge(stdin);										//this block of code assimilates the variables to its data in the structure
	fgets(product->name, (MAXCHAR*sizeof(char)), stdin);	//The __fpurge function cleans the input buffer, else it would read wrong characters
	__fpurge(stdin);
	scanf("%d", &product->code);
	__fpurge(stdin);
	scanf("%f", &product->price);
	__fpurge(stdin);

	fwrite( product , 1 , sizeof(PRODUCT) , file);			//writes changes in the file buffer loaded in memory and saves on disk
	fclose( file);
}

void printOption() {
	PRODUCT *aux = malloc(sizeof(PRODUCT));					//auxiliary pointer, used to print structure
	FILE *file = fopen("products.dat", "r");

	if (file == NULL || feof(file)){
		printf("products.dat don't exist or is empty");
		fclose(file);
		return;
	}

	while (1) {
		printf("\nName: %sCode: %d\nPrice: %0.2f\n", aux->name, aux->code, aux->price);
		fread( aux, sizeof(PRODUCT), 1, file);
	
		if (feof(file)){
			fclose(file);
			return;
		}
	}
	
	fclose(file);
}
