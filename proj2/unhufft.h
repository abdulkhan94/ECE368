#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct leaf
{
  struct leaf * left; //left sub-tree
  struct leaf * right; //right sub-tree
  int value;
  char letter;
} HuffNode;

typedef struct listnode
{
  struct listnode * next;
  HuffNode * tnptr;
} ListNode;

HuffNode *HuffNode_Create(int value, char letter);
void HuffNode_destroy(HuffNode * tree);
int Tree_heightHelper(HuffNode *tn, int height);
int Tree_height(HuffNode *tn);
void Tree_leafHelper(HuffNode *tn, int *num);
int Tree_leaf(HuffNode * tn);
void buildCodeBookHelper(HuffNode *tn, int * * codebook, int * row, int col);
void buildCodeBook(HuffNode * root, int ** codebook);
//void printCodeBook(int **codebook, int numRow);
int writeBit(FILE * fptr, unsigned char bit, unsigned char * whichbit, unsigned char * curbyte);
void char2bits(FILE * outfptr, int ch, unsigned char * whichbit, unsigned char * curbyte);
//void huffingFile(int *mappingTable,FILE *toHuff, FILE *gotHuffed);
  void Tree_headerHelper(HuffNode *tn, FILE * outfptr, unsigned char * whichbit, unsigned char * curbyte);
void Tree_header(HuffNode *tn, unsigned int numChar,char * outfile);
int compress(char *infile, char *outfile, int ** codebook,int*mapping);
int padZero(FILE *fptr, unsigned char * whichbit, unsigned char * curbyte);

HuffNode *readHeader(FILE * infptr);
int readBit(FILE * fptr, unsigned char * bit, unsigned char * whichbit, unsigned char * curbyte);
HuffNode * Tree_merge(HuffNode *tn1, HuffNode * tn2);

