#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	//Checking that indexes aren't equal
	if(l < r)
	{
		//Splitting array in half
		int mid = (l + r) / 2;

		//Recursion to split down
		mergeSort(pData, l, mid);
		mergeSort(pData, mid + 1, r);

		//Code of a Merge Function Below

		//Making Variables
		int i, j, k;
		int n1 = mid - l + 1;
		int n2 = r - mid;

		//Making Temp Arrays Using Alloc Function
		int *lArray = Alloc(sizeof(int) * n1);
		int *rArray = Alloc(sizeof(int) * n2);

		//Copying Data into Arrays

		//Left Array
		for(i = 0; i < n1; i++)
		{
			lArray[i] = pData[l + i];
		}

		//Right Array
		for(i = 0; i < n2; i++)
		{
			rArray[i] = pData[mid + 1 + i];
		}

		//Setting and Resetting Indexes
		i = 0; //Index for left array
		j = 0; //Index for right array
		k = l; //Index for merged array

		//Comparing Arrays
		while(i < n1 && j < n2)
		{
			//If Left subarray is smaller
			if(lArray[i] <= rArray[j])
			{
				pData[k] = lArray[i];
				i++;
			}

			//If Right subarray is smaller
			else
			{
				pData[k] = rArray[j];
				j++;
			}

			//Increasing index of merged array
			k++;
		}

		//Copying any remaining values in arrays
		while(i < n1)
		{
			pData[k] = lArray[i];
			i++;
			k++;
		}

		while(j < n2)
		{
			pData[k] = rArray[i];
			j++;
			k++;
		}


		//Using Dealloc Function on Temp Arrays
		DeAlloc(lArray);
		DeAlloc(rArray);
	}




}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");

	if(dataSz < 100)
	{
		for (i=0;i<dataSz;++i)
		{
			printf("%d ",pData[i]);
		}
		printf("\n\n");
		return;
	}

	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}