#include  "rand.h"
#include "mat.h"

int main(){

	int numIn, readStatus;
	readStatus = scanf("%d", &numIn);

	if( readStatus == 0 ){
		printf("Error Reading Input\n");
		exit(-1);
	}
	
	printf("Inputs: %d\n", numIn);
	
	//Read in training matrix data
	Matrix mIn("Raw Training Data");
	mIn.read();

	mIn.print();

	//Split raw input into X and T matricies
	printf("Splitting Training Data\n");

	Matrix mX = mIn.extract(0, 0, 0, numIn);
	Matrix mT = mIn.extract(0, numIn, 0, 0);

	mX.print();
	mT.print();

	//Normalize mX
	printf("mX min %f and max %f\n", mX.min(), mX.max());

	double mXcolMin[mX.numCols()];
	double mXcolMax[mX.numCols()];

	for( int i = 0; i < mX.numCols(); i++ ){
		printf("min: %f max: %f\n", mX.minCol(i), mX.maxCol(i));
		mXcolMin[i] = mX.minCol(i);
		mXcolMax[i] = mX.maxCol(i);
	}

	for( int i = 0; i < mX.numCols(); i++ ){
		printf("min: %f max: %f\n", mXcolMin[i], mXcolMax[i]);
	}
	
	mX.normalizeCols();

	mX.print();
	
	
	
	return 0;
}

//
