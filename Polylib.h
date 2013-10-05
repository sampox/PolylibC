/* POLYLIB.H FILE */
/* THIS FILE IS A PART OF A C LIBRARY WHICH PROVIDES THE FUNCTIONS */ 
/* FOR POLYNOMIAL ADDITION,MULTIPLICATION AND DIVISION */
/* AUTHOR: sampox */
/* Copyright ©2012 1010011010@boun.cr */
/* ALL RIGHTS RESERVED WITH THE FOLLOWING EXCEPTION: */
/* I ALLOW USING AND MODIFYING THE CODE FOR EDUCATIONAL PURPOSES, */
/* AS LONG AS YOU LEAVE THIS DISCLAIMER IN. */

#ifndef POLYLIB_H
#define POLYLIB_H

typedef struct term {
	int coeff;
	int deg;
	struct term * next;
	struct term * prev;
} termNode;

termNode* makeTerm(int coeff, int deg);
void deleteTerm(termNode* toDelete);
void printPoly(termNode* P);
termNode* cleanPoly(termNode* P);
termNode* addPoly(termNode* P, termNode* Q);
termNode* mulPoly(termNode* P, termNode* Q);
termNode* mulNeg(termNode *P);
termNode* divPoly(termNode *P,termNode *Q,int which);
termNode* modPoly(termNode* P, int mod);
termNode* addModPoly(termNode* P, termNode* Q, int mod);
termNode* mulModPoly(termNode* P, termNode* Q, int mod);
termNode* divModPoly(termNode *P, termNode* Q, int which, int mod);

#endif
