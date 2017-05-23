#include <stdio.h>
#include "sorting.h"


long *Load_File(char *Filename, int *Size)
{
  FILE *arr; 
  int i=0;
  int temporary;//Used to store the total number of integers at the top of the file
  arr=fopen(Filename,"r"); //Opens the file in read mode
  int check1;
  int check2=0;

  if(arr==NULL) //If array is NULL then break code
    {
      return NULL;
    }

  check1=fscanf(arr, "%d", &temporary); //Reads the values in arr
  long *array1 = malloc(sizeof(long)*temporary); //Assign memory for array1 
  
  while(!feof(arr)) //Loop keeps running until it gets to the end of arr
    {
      check2=fscanf(arr,"%ld",&array1[i]);
      i++;
    }
  if (check2==i)
    {
      i++;
    }
  i--;
  fclose(arr);
  *Size=temporary;
  return array1;

}

int Save_File(char *Filename, long *Array, int Size)
{
  int counter=0; //Counter used to run through all the indices in the array
  FILE *arr1;
  arr1=fopen(Filename,"w"); //File opened in write mode
  for(counter=0;counter<Size; counter++)//Cycles through all the elements in the array
    {
      fprintf(arr1,"%ld\n",Array[counter]);
    }
  return counter; //Used to print the number of elements saved 

}


void Shell_Insertion_Sort(long *Array, int Size, double *N_Comp, double *N_Move)
{

  int k=1; //Starting index in the sequence array
  int p=0; //2^p number of elements
  int last_2ind=0; 
  int last_3ind=0;
  int l=0;
  while(k<=Size) //This loop is used to get to the last possible value of the sequence that is not larger than the size.
    {
      k=k*3;
      p++;
    }
  k=k/3; //To get to the correct value of k which will later be our gap sequence
  p=p-1;
  int seq_size=((p+1)*(p+2))/2; //Formula used to calaculate the size of the array that has all the sequences stored in it
  int *seq_array= malloc(sizeof(int)*seq_size); //We use the size of the sequence array to properly assign memory
  seq_array[0]=1; //The algorithm used to calculate starts from the second element and the first element should be 1
  for(l=1;l<seq_size;l++) //Pratt algorithm is used to generate this. Essentially I generate the next number in the sequence by using previous numbers and multiplying them by 2's and 3's. The two variables last_3ind and last_2ind are used to keep track of the indices where the numbers 2 and 3 have been multiplied already and then it goes on from there   
    {
      /*      if(seq_array[last_2ind]*2 > seq_array[last_3ind]*3) //If the number in the sequence array multiplied by 2 is less than the same sequence number multiplied by the 3 we assign the value at the current index as the value at last_3ind multiplied by 3
        {
          seq_array[l]=seq_array[last_3ind]*3;
          last_3ind++;
        }
      else if(seq_array[last_2ind]*2 < seq_array[last_3ind]*3)  //Same as above but with opposite case. So you do the same with last_2ind and 2
	{
	  seq_array[l]=seq_array[last_2ind]*2;
	  last_2ind++;
	}
      else //This case is used to avoid repeats in the sequence array.
	{
	  seq_array[l]=seq_array[last_2ind]*2;
	  last_2ind++;
	  last_3ind++;
	  }
      */
      switch(seq_array[last_2ind]*2 > seq_array[last_3ind]*3)
	{
	case(1):
	  seq_array[l]=seq_array[last_3ind]*3;
          last_3ind++;
	  break;
	default:
	  break;
	}
      switch(seq_array[last_2ind]*2 < seq_array[last_3ind]*3)
	{
	case(1):
	  seq_array[l]=seq_array[last_2ind]*2;
          last_2ind++;
	  break;
	default:
	  break;
	}

      switch(seq_array[last_2ind]*2 == seq_array[last_3ind]*3)
	{
        case(1):
	  seq_array[l]=seq_array[last_2ind]*2;
          last_2ind++;
          last_3ind++;
	  break;
	default:
	  break;
	}
     
    }


  //Where the shell sort actually starts after our sequence array has been taken care of 
  int i=seq_size; //Our sequence array is in ascending order so we start from the last number in the sequence array which is the largest. 
  int gap;// The gap we will use
  int oloop;//outer loop in the insertion sort
  int iloop;//inner loop in the insertion sort
  int temparr;//Will be used to store the number in the array temporarily as the switch is made
  int mcount=0; //Move counter
  int ccount=0; //Comparison counter
  while(i>=0) //Going back in the sequence array so we end when it gets to the first element in the sequence array (the smallest gap value)
    {
      gap=seq_array[i]; //assigns the gap value 
      //Insertion sort goes here
      for(oloop=gap; oloop<=Size-1;oloop=oloop+1) //The outerloop starts from the specified gap in the actual Array and then increments by 1 every cycle
	{
	  temparr= Array[oloop]; //Stores the value at that index. Used later to make final swap
	  mcount++; 
	  iloop=oloop; 
	  while((iloop>=gap) && (Array[iloop-gap]>temparr)) //It compares two values which are seperated by the gap and makes a swap if necessary 
	    {
	      Array[iloop]=Array[iloop-gap]; //The value at the index [iloop-gap] is greater than the value at index [iloop] so swapping is started
	      ccount++;
	      iloop=iloop-gap; //The inner loop keeps going back as is supposed to in insertion sort
	      mcount++;       
	    }
	  Array[iloop]=temparr; //The final switch is made here from the temporary value we stored before
	  mcount++;
	}
      i--; //Decrements the while so the gap sequence value changes
    }
  *N_Move=mcount;
  *N_Comp=ccount;
}

void Improved_Bubble_Sort(long*Array, int Size, double *N_Comp, double *N_Move)
{
  int i=0; //index counter
  int temporary;
  int counter=0;//This is used to see if we still need run the code to swap values. If it is zero then we continue running the loop and when it turns into 1 we 
  int testerv=0;
  int val= (int)(Size/1.3); //The first value in the gap sequence is generated using size of the array. After this all the gap sequences are generated by using the previous gap sequence 
  if ( (val==10) || (val==9)) //Condition put in the question
    {
      val=11; 
    }
  int gap= val; //First gap is in val and is assigned to the variable gap
  int c_count=0;
  int m_count=0;
  while(gap!=0) //As long as the gap is not zero the loop keeps running
    {
      testerv++;
      //printf("%d",&testerv);
	
      counter=0; //It is refreshed so that loop below runs. This will refresh after we change our gap value
      while(!counter)
	{
	  counter=1;//If this remains 1 and doesn't go into the if statement the loop ends and it generates the next gap to be used 
	  for(i=gap;i<Size;i++) //Basic bubble sort algorithm is applied here with little changes like comaparing with a certain value after the specified gap rather than comparing with the neighbors
	    {
	      if((Array[i-gap]>Array[i])&&(i-gap>=0) ) //To determine whether we need to make a swap
		{
		  temporary = Array[i-gap];//Standard swapping technique is used here
		  Array[i-gap]= Array[i];
		  testerv++;
		  //printf("%d",&testerv);
		  Array[i] = temporary;
		  c_count= c_count+1; //Increment counters
		  m_count= m_count+2;
		  counter=0;//To keep the loop running
		}
	    }
	}
      val= (int)(gap/1.3); //Here we calculate the new gap by using the old gap value. 
      if ((val==10) || (val==9))
	{
	  val=11;
	}
      gap=val;
    }
  *N_Comp=c_count;
  *N_Move=m_count;
}
