#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unhufft.h"

HuffNode *HuffNode_Create(int value, char letter)
{
  HuffNode * newHuffNode = malloc(sizeof(HuffNode));
  
  newHuffNode -> value= value;
  newHuffNode -> letter = letter;
  newHuffNode -> left = NULL;
  newHuffNode -> right = NULL;
  return newHuffNode;
}

ListNode * ListNode_create(HuffNode *tn)
{
  ListNode * ln=malloc(sizeof(ListNode));
  ln->next = NULL;
  ln ->tnptr=tn;
  return ln;
}

void HuffNode_destroy(HuffNode * tree)
{
  if(tree == NULL){return;}
  HuffNode_destroy(tree -> left);
  HuffNode_destroy(tree -> right);
  free(tree);
}

ListNode * List_insert(ListNode *head, ListNode *ln, int mode)
{// modes STACK is 1, QUEUE is 0
  if(ln == NULL)
    {					       
      printf("ERROR! ln is NULL\n");
      return NULL;
    }
  if((ln->next) != NULL)
    {
      printf("ERROR! ln -> next is not NULL\n");
    }
  if(head == NULL)
    {
      return ln;
    }
  if(mode == 1)
    {
      ln->next = head;
      return ln;
    }
  if (mode == 0)
    {
      head -> next = List_insert(head->next, ln, mode);
      return head;
    }
  //insert in increasing order
  int freq1= (head -> tnptr) -> value;
  int freq2 = (ln->tnptr)->value;
  if(freq1 > freq2)
    {//ln should be the first node
      ln->next = head;
      return ln;
    }
  //ln should be after head
  head -> next = List_insert(head -> next, ln, mode);
  return head;
}

ListNode * MergeListNode(ListNode * head, int endec)
{
  ListNode *second = head->next; //second must not be NULL, otherwise, will not enter
  ListNode *third = second->next;//third may be NULL
  //get the tree nodes of the first 2 list nodes
  HuffNode * tn1= head->tnptr;
  HuffNode *tn2=second->tnptr;
  //remove the first 2 nodes
  free(head);
  free(second);
  head=third;
  HuffNode * mrg;
  //encode mode is 0 decode is 1
  if(endec == 0)
    {
      mrg=Tree_merge(tn1,tn2);
    }
  else
    {
      mrg=Tree_merge(tn2,tn1);
    }
  ListNode *ln = ListNode_create(mrg);
  if(endec==0)
    {//define SORTED is 2
      head = List_insert(head, ln, 2);
    }
  else
    {//STACK option is 1
      head = List_insert(head, ln, 1);
    }
  return head;
  
}

HuffNode * Tree_merge(HuffNode *tn1, HuffNode * tn2)
{
  HuffNode * tn= malloc(sizeof(HuffNode));
  tn->left=tn1;
  tn->right=tn2;
  //tn->letter=0;  //don't care
  tn->letter=-1; //don't care
  tn->value=tn1->value + tn2->value;
  return tn;
}
/*
void Tree_print(HuffNode * tn, int level)
{
  if(tn==NULL){return;}
  HuffNode * lc = tn->left; //left child
  HuffNode * rc = tn->right; //right child
  Tree_print(lc, level + 1);
  Tree_print(rc, level + 1);
  int depth;
  for(depth=0; depth < level; depth++)
    {
      printf("    ");
    }
  printf("value = %d ", tn->value);
  if((lc == NULL) && (rc == NULL))
    {//a leaf node
      printf("letter = %d, '%c'", tn->letter, tn->letter);
    }
  printf("\n");
  }*/

HuffNode *readHeader(FILE * infptr)
{//modes: STACK =1, QUEUE =0
  int done=0;
  unsigned char whichbit =0;
  unsigned char curbyte =0;
  unsigned char onebit =0;
  ListNode *head= NULL;
  while(done == 0)
    {
      readBit(infptr, &onebit, &whichbit, &curbyte);
      /*printf("onebit of header is %d\n",onebit);
      printf("whichbit of header is %d\n",whichbit);
      printf("curbyte of header is %c\n\n",curbyte);*/


      if(onebit==1)
	{// a leaf node, get 7 move bits
	  int bitcount;
	  unsigned char value=0;
	  for(bitcount = 0; bitcount < 7; bitcount++)
	    {
	      value <<= 1; //shift left by one
	      readBit(infptr, &onebit, &whichbit, &curbyte);
	      value |= onebit;
	    }
	  //push a tree node into the list
	  //HuffNode * tn=HuffNode_Create(value,0);
	  HuffNode * tn=HuffNode_Create(0,value);
	  ListNode * ln=ListNode_create(tn);
	  head=List_insert(head, ln, 1);
	}
      else
	{
	  if(head == NULL)
	    {
	      printf("ERROR, head should not be NULL\n");
	    }
	  if((head->next)==NULL)
	    {//the tree has been completely built
	      done=1;
	    }
	  else
	    {//DECODEMODE = 1, ENCODEMODE=0
	      head = MergeListNode(head, 1);
	    }
	}
    }
  //unecessary to read the remaining unused bits
  HuffNode *root=head->tnptr;
  //the linked list is no longer needed
  free(head);
  return root;
}

int decode(char * infile, char * outfile)
{
  //printf("%s\n", infile);
  FILE * infptr=fopen(infile, "r");
  //fseek(infptr, 0, SEEK_SET);
  if(infptr == NULL)
    {
      //perror("file null\n");
      return 0;
    }
  HuffNode * root=readHeader(infptr);
  //Tree_print(root,0);
  //read the num of chars
  unsigned int numChar =0;
  fread(&numChar, sizeof(unsigned int), 1, infptr);
  //printf("numChar = %d\n", numChar);
  //read '\n'
  unsigned char newline;
  fread(&newline, sizeof(unsigned char), 1, infptr);
  if(newline != '\n')
    {
      printf("ERROR!\n");
    }
  unsigned char whichbit=0;
  unsigned char onebit =0;
  unsigned char curbyte =0;
  FILE *outfptr = fopen(outfile, "w");
  while (numChar != 0)
    {
      printf("%d\n",numChar);
      HuffNode *tn = root;
      while((tn->left) != NULL)
	{//tn is not a leaf node
	  readBit(infptr, &onebit, &whichbit, &curbyte);
	  /*
	  printf("one bit is %d\n", onebit);
	  printf("whichbit of header is %d\n",whichbit);
	  printf("curbyte of header is %c\n\n",curbyte);*/
	  if(onebit == 0)
	    {
	      tn=tn->left;
	    }
	  else
	    {
	      printf("asdfghjkl");
	      tn = tn->right;
	    }
	}
      //tn is a leaf node
      //printf("%c", tn->letter);
      fprintf(outfptr,"%c", tn->letter);
      numChar--;
    }
  HuffNode_destroy(root);
  fclose(infptr);
  fclose(outfptr);
  return 1;
}

int readBit(FILE * fptr, unsigned char * bit, unsigned char * whichbit, unsigned char * curbyte)
{
  int ret=1;
  if((*whichbit) == 0)
    {//read a byte from the file
      ret = fread(curbyte, sizeof(unsigned char), 1, fptr);
    }
  if(ret!=1)
    {//read fail
      //printf("you failure\n");
      return -1;
    }
  //shift the bit to the correct location
  unsigned char temp= (*curbyte) >> (7-(*whichbit));
  temp=temp & 0X01; //get only 1 bit, ignore the other bits
  //increase by 1
  *whichbit = ((*whichbit) + 1) % 8;
  *bit=temp;
  //printf("bit or temp is %d\n",temp);
  return 1;
}

int main(int Argc, char ** Argv)
{
  if (Argc<2)
    {
      printf("not enough arguments");
      return 0;
    }
  char*filename=Argv[1];
  char *fileout=malloc((strlen(filename)+7+1)*sizeof(char));
  strcat(fileout, filename);
  strcat(fileout, ".unhuff");
  //printf("filename is %s\n", filename);
  //printf("fileout is %s\n", fileout);
  decode(filename,fileout);
  free(fileout);
  return 0;
}
