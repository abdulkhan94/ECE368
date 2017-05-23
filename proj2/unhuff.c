#include "unhuff.h"
#include <string.h>


TreeNode * readHeader(FILE * infptr) //function used to readheader
{
  //Variables
  int d=0;
 
  unsigned char wbit=0;
  unsigned char cbyte=0;
 
  ListNode *head=NULL;
  unsigned char onebit=0;

  //Executions
  while(d==0)
  {
    readBit(infptr, &onebit, &wbit, &cbyte);
    if(onebit==1)
      {
	if((head->next)==NULL)
	  {
	    d=1;
	  }
	else if(head==NULL)
	  {
	    printf("ERROR1\n");
	  }
	else
	  {
	    head=merge_list(head,1);
	  }
      }
    else if(onebit==1)
      {
	int bcount;
	unsigned char value=0;
	for(bcount=0;bcount<7;bcount=bcount+1)
	  {
	    value<<=1;//shift left one
	    readBit(infptr,&onebit,&wbit,&cbyte);
	    value|=onebit;
	  }
	//push a tree node into a list
	TreeNode * t_node=create_TreeNode(value, 0);
	ListNode * w_node=create_list(t_node);
	head = insert_list(head,w_noden,1);
      }
  }
  TreeNode * root=head->tptr;
  free(head);
  return(root);
}

int decode(char * infile, char * outfile)
{
  //printf("decode beginning");
  //printf("%s\n", infile);
  FILE * iptr=fopen(infile,"r");
  //fseek(infptr, 0, SEEK_SET);
  if(iptr==NULL)
    {
      //perror("file null\n");
      return 0;
    }
  TreeNode *root=readHeader(iptr);
  //Tree_print(root,0);
  unsigned int numChar =0;
  fread(&numChar,sizeof(unsigned int), 1,iptr);
  unsigned char newline;
  fread(&newline, sizeof(unsigned char), 1, infptr);
  if(newline != '\n')
    {
      printf("ERROR!\n");
      return 0;
    }
  //Professor Lu's code below because I could not figure out on time and I was trying to see if it would run
  unsigned char whichbit=0;
  unsigned char onebit =0;
  unsigned char curbyte =0;
  FILE *outfptr = fopen(outfile, "w");
  while (numChar != 0)
    {
      TreeNode *tn = root;
      while((tn->left) != NULL)
    {//tn is not a leaf node
      readBit(infptr, &onebit, &whichbit, &curbyte);
      printf("one bit is %d\n", onebit);
      if(onebit == 0)
        {
          tn=tn->left;
        }
      else
        {
          tn = tn->right;
        }
    }
      //tn is a leaf node
      //printf("%c", tn->value);
      fprintf(outfptr,"%c", tn->val);
      numChar--;
    }
  destroy_tree(root);
  fclose(infptr);
  fclose(outfptr);
  return 1;
}

//All the functions below are from huff.c
ListNode *create_list(TreeNode *tree_node)
{
  ListNode *list_node= malloc(sizeof(ListNode));
  list_node->next=NULL;
  list_node->tptr=tree_node;
  return list_node;

}

//Edited this function to get third input
ListNode *insert_list(ListNode *head, ListNode *n, int mode)
{
  int freq_1;
  int freq_2;
  
  if(head==NULL)
    {
      return n;
    }
  
  if(n==NULL)
    {
      printf("error!\n");
    }

  if((n->next)!=NULL)
    {
      printf("error!\n");
    }

  if (mode==1)
    {
      n->next = head;
      return n;
    }
  if (mode==0)
    {
      head->next = insert_list(head->next,n,mode);
      return head;
    }


  freq_1=(head->tptr)->frequency;
  freq_2=(n->tptr)->frequency;
  if(freq_1 > freq_2)
    {
      n->next=head;
      return n;
    }
  
  head->next= insert_list(head->next,n,mode);
  return head;
}

void destroy_tree(TreeNode *n)
{
  if(n==NULL)
    return;
  
  destroy_tree(n->left);
  destroy_tree(n->right);
  free(n);

}

//Tree Stuff
TreeNode *create_TreeNode(char val, int freq)
{
  TreeNode *tree_node= malloc(sizeof(TreeNode));
  tree_node->left=NULL;
  tree_node->right=NULL;
  tree_node->val=val;
  tree_node->frequency= freq;
  return tree_node;
}


TreeNode *merge_TreeNode(TreeNode *tn_1, TreeNode *tn_2)
{
  TreeNode *tree_node=malloc(sizeof(TreeNode));
  tree_node->left=tn_1;
  tree_node->right=tn_2;
  tree_node->val=0;
  tree_node->frequency= tn_1->frequency + tn_2->frequency;
  return tree_node;
}



int readBit(FILE * fptr, unsigned char * bit, unsigned char * whichbit, unsigned char * curbyte)
{
  int ret = 1;
  if((*whichbit) == 0)
    {
      ret = fread(curbyte, sizeof(unsigned char), 1, fptr);
    }
  if(ret != 1)
    {
      //read fail
      return -1;
    }
  unsigned char temp = (*curbyte) >> (7 - (*whichbit));
  temp = temp & 0X01;
  *whichbit = ((*whichbit) + 1) % 8;
  *bit = temp;
  return 1;
}

static ListNode * merge_list(ListNode * head, int endec)
{
  ListNode *second = head->next; //second must not be NULL, otherwise, will not enter
  ListNode *third = second->next;//third may be NULL
  //get the tree nodes of the first 2 list nodes
  TreeNode * tn1= head->tptr;
  TreeNode *tn2=second->tptr;
  //remove the first 2 nodes
  free(head);
  free(second);
  head=third;
  TreeNode * mrg;
  //encode mode is 1 decode is 0
  if(endec == 0)
    {
      mrg= merge_TreeNode(tn1,tn2);
    }
  else
    {
      mrg= merge_TreeNode(tn2,tn1);
    }
  ListNode *ln = create_list(mrg);
  if(endec == 0)
    {//define SORTED is 2
      head = insert_list(head, ln, 2);
    }
  else
    {//STACK option is 1
      head = insert_list(head, ln, 1);
    }
  return head;
 
}

int main(int argc, char **argv)
{ 
  /*   
  char *fn=malloc(sizeof(char));
  char *fout=(char *)calloc((strlen(fn)+7),sizeof(char));
  printf("Enter input file: ");
  scanf("%s",fn);
  strcat(fout,fn);
  strcat(fout,".unhuff");
  decode(fn,fout);
  return 0;
 
 */
 
  decode("go.txt.huff", "go.txt.huff.unhuff");
  return(0);
  
} 
