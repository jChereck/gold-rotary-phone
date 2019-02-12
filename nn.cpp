#include  "rand.h"
#include "mat.h"
#include <math.h>

#define weightInitMax 0000.0
#define weightInitMin -10.000
#define TRANSFER_SLOPE 0.50
#define ITERATIONS 4500
#define ETA 0.010

void predict(int numIn, Matrix mW);
Matrix train(int numIn);

double transfer(double x){
	return 1/(1.0 + exp(-1.0 * TRANSFER_SLOPE * x));
}

int main(){

	//Initialize randomization function for weight initialization
	initRand();

	//read in number of inputs from stdin
	int numIn, readStatus;
	readStatus = scanf("%d", &numIn);

	if( readStatus == 0 ){
		printf("Error Reading Input\n");
		exit(-1);
	}
	
	//Train on data from stdin and return weights
	Matrix Weights = train(numIn);
	
	//Test with trained weights on new data from stdin
	printf("BEGIN TESTING\n");
	predict(numIn, Weights);

	return 0;
}

void predict(int numIn, Matrix mW){

	//Read in prediction matrix data
	Matrix mIn("Prediction Data In");
	mIn.read();

	//Duplicate prediction data to manipulate
	Matrix mX(mIn, "Prediction Data");

	//Normalize mX
	mX.normalizeCols();

	//Add Bias col to mX (as mXb)
	Matrix mXb(mX.numRows(), mX.numCols() + 1, 1.0);
	mXb.setName("mX with Bias");
	mXb.insert(mX, 0, 0);

	//Compute output for prediction
	Matrix mY2 = mXb.dot(mW);
	mY2.setName("Final Output");

	//Apply transfer function to final output
	mY2.map(transfer);

	//Print output to assignment specs
	for(int i = 0; i < mY2.numRows(); i++){
		mIn.writeLine(i);
		mY2.writeLine(i);
		printf("\n");
	}

	return;
}

Matrix train(int numIn){

	//Read in training matrix data
	Matrix mIn("Raw Training Data");
	mIn.read();

	//Split raw input into X and T matricies
	Matrix mX = mIn.extract(0, 0, 0, numIn);
	mX.setName("mX");
	Matrix mT = mIn.extract(0, numIn, 0, 0);
	mT.setName("mT");

	//Normalize mX
	mX.normalizeCols();

	//Add Bias col to mX (as mXb)
	Matrix mXb(mX.numRows(), mX.numCols() + 1, 1.0);
	mXb.setName("mX with Bias");
	mXb.insert(mX, 0, 0);

	//Create Weights column
	Matrix mW(mXb.numCols(), mT.numCols()); 
	mW.setName("Weights");
	mW.rand(weightInitMin, weightInitMax);

	for(int i = 0; i < ITERATIONS; i++){
		//compute Y
		Matrix mY = mXb.dot(mW);
		mY.setName("Output");
	
		//Apply transfer function to Y
		mY.map(transfer);
	
		//Edit weights
		Matrix mAdj(mY);
		mAdj.sub(mT);
		Matrix mAdj2 = mXb.Tdot(mAdj);
		mAdj2.scalarMul(ETA);
		
		mW.sub(mAdj2);
	}
	
	return mW;
}
