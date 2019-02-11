#include  "rand.h"
#include "mat.h"
#include <math.h>

#define weightInitMax 1.0
#define weightInitMin 0.0
#define TRANSFER_SLOPE 1.0

double transfer(double x){
	return 1/(1.0 + exp(-1.0 * TRANSFER_SLOPE * x));
}

int main(){

	initRand();

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
	mX.setName("mX");
	Matrix mT = mIn.extract(0, numIn, 0, 0);
	mT.setName("mT");

	mX.print();
	mT.print();

	//Normalize mX
	printf("mX min %f and max %f\n", mX.min(), mX.max());

	//Save min/max of each col to unnormalize later
	Matrix mXnorm = mX.normalizeCols();

	mX.print();

	//Add Bias col to mX
	Matrix mXb(mX.numRows(), mX.numCols() + 1, 1.0);
	mXb.setName("mX with Bias");
	mXb.insert(mX, 0, 0);

	mXb.print();

	//Create Weights column
	printf("mW rows: %d cols: %d\n", mXb.numCols(), mT.numCols());
	Matrix mW(mXb.numCols(), mT.numCols()); 
	mW.setName("Weights");
	mW.rand(weightInitMin, weightInitMax);
	mW.print();

	//compute Y
	Matrix mY = mXb.dot(mW);
	mY.setName("Output");

	mY.print();

	//Apply transfer function to Y
	mY.map(transfer);

	mY.print();

	//Edit weights
	double eta = 0.4;

	Matrix mAdj(mY);
	mAdj.sub(mT);
	Matrix mAdj2 = mXb.Tdot(mAdj);
	mAdj2.scalarMul(eta);
	
	mW.sub(mAdj2);
	
	mW.print();

	//compute Y2
	Matrix mY2 = mXb.dot(mW);
	mY2.setName("Output 2");

	mY2.print();

	//Apply transfer function to Y
	mY2.map(transfer);

	mY2.print();

	/*
	//Forward Propigate and Learn
	double eta = 0.4;
	for( int reps = 0; reps < 10; reps++ ){

		//compute Y
		Matrix mY = mXb.dot(mW);
		mY.setName("Output");
	
		mY.print();

		//update Weights
		Matrix diff(mY);
		diff.sub(mT).abs();
		diff.print();
		diff.scalarMul(eta);
		diff.print();

		mW.add(diff);
	}
	*/
	


	return 0;
}

