/* POLYLIB.H FILE */
/* THIS FILE IS A PART OF A C LIBRARY WHICH PROVIDES THE FUNCTIONS */ 
/* FOR POLYNOMIAL ADDITION,MULTIPLICATION AND DIVISION */
/* AUTHOR: sampox */
/* Copyright ©2012 1010011010@boun.cr */
/* ALL RIGHTS RESERVED WITH THE FOLLOWING EXCEPTION: */
/* I ALLOW USING AND MODIFYING THE CODE FOR EDUCATIONAL PURPOSES, */
/* AS LONG AS YOU LEAVE THIS DISCLAIMER IN. */

#include <stdio.h>
#include <stdlib.h>
#include "Polylib.h"

/* typedef struct term {
	int coeff;
	int deg;
	struct term * next;
	struct term * prev;
} termNode; */ 

//MAKE A SINGLE TERM IN THE POLYNOMIAL (THE POLYNOMIAL IS JUST A COLLECTION OF THESE)
termNode* makeTerm(int coeff, int deg) {
	termNode* newTerm = malloc(sizeof(termNode));
	newTerm->coeff=coeff;
	newTerm->deg=deg;
	newTerm->next=NULL;
	newTerm->prev=NULL;
	return newTerm;
}

//DELETE A TERM FROM THE POLYNOMIAL
void deleteTerm(termNode* toDelete) {
	free(toDelete);
}

//PRINT THE POLYNOMIAL
void printPoly(termNode* P) {
	while(P!=NULL) {
		if(P->next==NULL) {
			if(P->deg==0) {
				if(P->coeff!=0) {
				printf("%d\n", P->coeff); 
				} 
			} else if(P->coeff==1) {
				printf("x^%d\n", P->deg);
			} else {
				printf("%dx^%d\n", P->coeff, P->deg);
			}
			break;
		}
		
		if(P->deg==0) 
			printf("%d\n", P->coeff);
		else if(P->coeff==1) 
			printf("x^%d + ", P->deg);
		else 
			printf("%dx^%d + ", P->coeff, P->deg);
		P=P->next;
		} 
}

//CLEAN ZERO AND EMPTY TERMS FROM THE POLYNOMIAL
termNode* cleanPoly(termNode* P) {	
	if(P==NULL) 
		return NULL;
	while(P->coeff==0) {	//ZERO CHECK && NULL CHECKS
	P=P->next;
	}
	if(P==NULL) 
		return NULL;

	termNode* newP=P; 

	while(P->next!=NULL) {
		if(P->next->coeff==0) {
			deleteTerm(P->next);
			P->next=NULL;
		} else {
		P=P->next;
		}
		}
	if(newP->coeff!=0)
		return newP;
	else
		return NULL;
}

//ADD POLYNOMIALS
termNode* addPoly(termNode* P, termNode* Q) {
	
	termNode* addedTerm;

	if(P==NULL || Q==NULL) {
		if(P==NULL && Q)
			return Q; 
		else if(P && Q==NULL) 
			return P;
		else 
			return NULL;			
	}

	else if(P->deg == Q->deg) {
		addedTerm=makeTerm(P->coeff+Q->coeff,P->deg);
		addedTerm->next=addPoly(P->next,Q->next);	
	}
	else if(P->deg > Q->deg) {
		addedTerm=makeTerm(P->coeff,P->deg);
		addedTerm->next=addPoly(P->next,Q);
		}
	else if(P->deg < Q->deg) {
		addedTerm=makeTerm(Q->coeff,Q->deg);
		addedTerm->next=addPoly(P,Q->next);
		}
	return cleanPoly(addedTerm);
}

//MULTIPLY POLYNOMIALS
termNode* mulPoly(termNode* P, termNode* Q) {
	termNode* mulTerm=makeTerm(0,0);
	termNode* Qp = Q;
	while(P!=NULL)  {
		Q=Qp;
		while(Q!=NULL) {
			mulTerm=addPoly(mulTerm, makeTerm(P->coeff*Q->coeff,P->deg+Q->deg));
			Q=Q->next; 
		} 
	P=P->next; 
	}
	return mulTerm;
}

//NEGATION OF THE POLYNOMIAL 
termNode* mulNeg(termNode *P) {
    termNode *st = P;
    while(st != NULL)
    {
        st->coeff = st->coeff * (-1);
        st = st->next;
    }
    return P;
}

//DIVIDE POLYNOMIALS, if which==1 it returns the quotient of the division, else the remainder
termNode* divPoly(termNode *P,termNode *Q,int which) {
    termNode *R = P;
    termNode *helpTerm = makeTerm(0,0);
    termNode *t = makeTerm(0,0);
    termNode *divPoly = makeTerm(0,0);
    int n = P->deg;
    int m = Q->deg;
    for(int i = n-m; i >= 0; i--)
    {
        R = cleanPoly(R);
        if(R->deg == m + i)
        {
            termNode *temp = makeTerm((int)R->coeff/Q->coeff,i);
            divPoly = addPoly(divPoly,temp);
            helpTerm->coeff = R->coeff/Q->coeff;
            helpTerm->deg = i;
            t = mulPoly(Q,helpTerm);
            t = mulNeg(t);
            R = addPoly(R,t);
            R = cleanPoly(R);
        }
        else
        {
            helpTerm->coeff = 0;
            helpTerm->deg = i;
            divPoly = addPoly(divPoly,helpTerm);
        }
    }
    if(which == 1)	
        return cleanPoly(divPoly);  //QUOTIENT
    else
        return cleanPoly(R);  //REMAINDER
}

//TAKE MODULUS FROM POLYNOMIAL
termNode* modPoly(termNode* P, int mod) {
	termNode* newP=makeTerm(0,0);
	while(P!=NULL) {
		P->coeff=((P->coeff%mod)+mod)%mod;
		if(P->coeff!=0) 
			newP=addPoly(makeTerm(P->coeff,P->deg),newP);
		P=P->next;		
		}
	return newP;
}

//ADDPOLY EXCEPT WITH MODULUS
termNode* addModPoly(termNode* P, termNode* Q, int mod) {
	return modPoly(addPoly(P,Q),mod);
}

//MULPOLY EXCEPT WITH MODULUS
termNode* mulModPoly(termNode* P, termNode* Q, int mod) {
	return modPoly(mulPoly(P,Q),mod);
}

//DIVPOLY EXCEPT WITH MODULUS
termNode* divModPoly(termNode *P, termNode* Q, int which, int mod) {
	return modPoly(divPoly(P,Q,which),mod);
}

//HOW TO USE THE FUNCTIONS (EXAMPLE main()):
//NOTE THAT DEFINING THE POLYNOMIALS CAN BE DONE IN A LOT SMARTER WAY
//BUT THIS WAS JUST FOR THE SAKE OF TESTING THE FUNCTIONALITY

int main(void) {


//DEFINE THE POLYNOMIAL P = 2x^6 +2x^5 +x^3 +x^2 +2x +2
termNode* P = makeTerm(2,6);
P->next=makeTerm(2,5);
P->next->next=makeTerm(1,3);
P->next->next->next=makeTerm(1,2);
P->next->next->next->next=makeTerm(2,1);
P->next->next->next->next->next=makeTerm(2,0);

//DEFINE THE POLYNOMIAL Q = x^4 +x^3 +2x^2 +x +2
termNode* Q = makeTerm(1,4);
Q->next = makeTerm(1,3);
Q->next->next = makeTerm(2,2);
Q->next->next->next = makeTerm(1,1);
Q->next->next->next->next=makeTerm(2,0);

//DIVIDE P & Q (MOD 3), STORE THE RESULT (!!QUOTIENT NOT REMAINDER!! since which==1) IN X AND PRINT X
termNode* X = divModPoly(P,Q,1,3);
printPoly(X);


}
