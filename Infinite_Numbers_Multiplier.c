/*This project multiplies multiplies two infinite digit numbers*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/*Represents each digit of the number*/
struct number {
    int exp;
    int num;
    struct number *nextNumber;
};
typedef struct number Number;
typedef Number* NumberPtr;

/*Function declarations*/
char* readInfinite(FILE* file);
NumberPtr reverseList(NumberPtr head);
NumberPtr createStruct(char[],int*);
NumberPtr multiplication(NumberPtr, NumberPtr);
void printList(NumberPtr,int);
void printResultList(NumberPtr head, int, int) ;
clock_t startTime, endTime;
double runTime;
FILE *cfPtr1, *cfPtr2;
int max = 10000;

/* Main method*/
int main() {
	startTime= clock();
	/*Opens the input file to read*/
	cfPtr1 = fopen("input.txt", "r");
	 if (!cfPtr1) {
        printf("This file can not be opened\n");
        return NULL;
    }
    /* Opens the output file for writing*/
	cfPtr2 = fopen("output.txt", "w");
	/*Reads infite numbers*/	
    char* num1= readInfinite(cfPtr1);
    char* num2 = readInfinite(cfPtr1);
    fclose(cfPtr1);
	int point1, point2;
    /*Creates lists, multiplies them, and prints into the file*/
    NumberPtr list1 = (createStruct(num1,&point1));
    fprintf(cfPtr2, "Multiplicand: ");
    printList(list1,point1);
    NumberPtr list2 = ( createStruct(num2,&point2));
    fprintf(cfPtr2, "\n\nMultiplier: ");
    printList(list2,point2);
    fprintf(cfPtr2, "\n\nResult: ");
    printResultList(multiplication(list1,list2),point1,point2);
	endTime = clock();
	runTime = (double)(endTime-startTime)/CLOCKS_PER_SEC;
	fprintf(cfPtr2, "\nExecution time: %f seconds",runTime);
    fclose(cfPtr2);
    return 0;
}
/*Reads infinite numbers*/
char* readInfinite(FILE* file) {
    int size = 0;
    char* inf = malloc(max * sizeof(char));
    if (inf == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    char c;
    /*Allocates space in memory for every new 10,000 digits*/
    while ((c = fgetc(file)) != EOF && c != '\n') {
       
        if (size>= max - 1) {
  	
            max *= 2;  
            inf = realloc(inf, max * sizeof(char));
            if (inf == NULL) {
                printf("Memory reallocation failed.\n");
                exit(1);
            }
        }
        inf[size++] = c;
    }
    inf[size] = '\0';  
    return inf;
}
/*Creates a linked list for numbers*/
NumberPtr createStruct(char num[], int* dot){  
    int i, j, point=0;
    /*Finds the place of decimal point*/
    for (i = 0; num[i] != '\0' && i < max; i++) {
    	if(num[i]=='.')
    	point=i;
    }
    j = i-1;
    if (point!=0)
    *dot =j-point;
    else
    *dot=0;
    NumberPtr startPtr = NULL;
    NumberPtr curPtr = NULL;
/*Creates a linked list and intializes a new struct for each digit*/
    for (i = 0; num[i] != '\0' && i < max; i++) {
    	if (point != 0 && i==point){
    		j++;
    		continue;
		}	
        NumberPtr newPtr = (NumberPtr)malloc(sizeof(Number));
        if (newPtr == NULL) {
            printf("Memory allocation failed.\n");
            exit(1);
        }
        newPtr->exp = j-i;  
        newPtr->num = num[i] - '0';  
        newPtr->nextNumber = NULL;
        if (startPtr == NULL) 
            startPtr = newPtr;
        else 
            curPtr->nextNumber = newPtr;
        curPtr = newPtr;
    }
    return startPtr;
}
/*Multiplies 2 numbers in linked list*/
NumberPtr multiplication(NumberPtr curDig1, NumberPtr curDig2) {
    NumberPtr strPtr = NULL;  
	NumberPtr dig1, dig2 ;
	/*Multiplies digit by digit*/
    for (dig2 = curDig2; dig2 != NULL; dig2 = dig2->nextNumber) {
        NumberPtr resultPtr = NULL;     
        NumberPtr prevPtr = NULL;       
        for (dig1 = curDig1; dig1 != NULL; dig1 = dig1->nextNumber) {
            NumberPtr newDigit = (NumberPtr)malloc(sizeof(Number));
            if (newDigit == NULL) {
                printf("No memory available.\n");
                exit(1);
            }
            newDigit->exp = dig1->exp + dig2->exp; 
            newDigit->num = dig1->num * dig2->num;   
            newDigit->nextNumber = NULL;
            if (strPtr == NULL) 
                strPtr = newDigit;
			else {               
                resultPtr = strPtr;
                prevPtr = NULL;
                while (resultPtr != NULL && resultPtr->exp < dig1->exp + dig2->exp) {
                    prevPtr = resultPtr;
                    resultPtr = resultPtr->nextNumber;
                }
                if (resultPtr != NULL && resultPtr->exp ==  dig1->exp + dig2->exp) {
                    resultPtr->num +=dig1->num * dig2->num ;
                    free(newDigit);
				}				 
				else {
                    if (prevPtr == NULL) {
                        newDigit->nextNumber = strPtr;
                        strPtr = newDigit;
                    } else {
                    	newDigit->nextNumber = resultPtr;
                        prevPtr->nextNumber = newDigit;
                    }
                }
            }
        }
        resultPtr = strPtr;
        /*Moves the carry to the next digit properly*/
        while (resultPtr != NULL) {
            if (resultPtr->num >= 10) {
                int carry = resultPtr->num / 10;
                resultPtr->num %= 10;
                if (resultPtr->nextNumber == NULL) {
                    NumberPtr nextDigit = (NumberPtr)malloc(sizeof(Number));
                    if (nextDigit == NULL) {
                        printf("No memory available.\n");
                        exit(1);
                    }
                    nextDigit->exp = resultPtr->exp + 1;
                    nextDigit->num = carry;
                    nextDigit->nextNumber = NULL;
                    resultPtr->nextNumber = nextDigit;
                } else {
                    resultPtr->nextNumber->num += carry;
                }
            }
            resultPtr = resultPtr->nextNumber;
        }
    }  
    return (strPtr);
}
/*Prints the input numbers*/
void printList(NumberPtr head, int point) {                                                                         
    NumberPtr curPtr = head;
    int i,j ;
    for (i=0 ;curPtr != NULL; i++){
    	curPtr= curPtr->nextNumber;
	}
	j=i;
	curPtr= head;	
    for ( ;curPtr != NULL; i--) {
    	/*Finds the place of decimal point*/
    	if (i==point && point<j-1)
    	fprintf(cfPtr2,".");
        fprintf(cfPtr2,"%d", curPtr->num );
        curPtr = curPtr->nextNumber;
    }
}
/*Prints the result of multiplication*/
void printResultList(NumberPtr head, int point1, int point2) {
int point = point1 + point2;  
	NumberPtr prev = NULL;
    NumberPtr current = head;
    NumberPtr next = NULL;
    int i ,j= 0;
    /*Reverses the linked list*/
    while (current != NULL) {
        next = current->nextNumber;
        current->nextNumber = prev;
        prev = current;
        current = next;
        i++;
    }
   j=i;
    for ( ;prev != NULL; i--) {
    	/*Finds the place of decimal point*/
    	if (i==point && point<j)
    	fprintf(cfPtr2,".");
        fprintf(cfPtr2,"%d", prev->num );
        prev = prev->nextNumber;
    }
}

