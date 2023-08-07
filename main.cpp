#include "emd.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace t_simplex;
using namespace std;

typedef struct {
	double X;
	double Y;
	double Z;
} feature_t;

typedef struct {
	feature_t coord;
	double q;
} charge_t;

double squareDist(feature_t * facPtr,feature_t * warPtr) {
return ((*facPtr).X - (*warPtr).X)*((*facPtr).X - (*warPtr).X) + ((*facPtr).Y - (*warPtr).Y)*((*facPtr).Y - (*warPtr).Y) +
	((*facPtr).Z - (*warPtr).Z)*((*facPtr).Z - (*warPtr).Z);
};

double Dist(feature_t * facPtr,feature_t * warPtr) {
//Formula for approximating the distance between a factory and a warehouse;
return sqrt(((*facPtr).X - (*warPtr).X)*((*facPtr).X - (*warPtr).X) + ((*facPtr).Y - (*warPtr).Y)*((*facPtr).Y - (*warPtr).Y) +
	((*facPtr).Z - (*warPtr).Z)*((*facPtr).Z - (*warPtr).Z));
};

vector<charge_t> fitting(vector<charge_t> &oriCharge, vector<feature_t> &fitGrids) {
	vector<charge_t> curCharge;
	for (int i=0; i < fitGrids.size(); i++) {
		charge_t curChargePoint;
		curChargePoint.q = 0;
		curChargePoint.coord = fitGrids[i];
		curCharge.push_back(curChargePoint);
	}
	for (int i=0; i < oriCharge.size(); i++) {
		double minSD = 1000;
		int minJ = 0;
		for (int j=0; j < fitGrids.size(); j++) {
			double curSD = squareDist(&oriCharge[i].coord, &fitGrids[j]);
			if (curSD < minSD) {
				minSD = curSD;
				minJ = j;
			}
		}
		curCharge[minJ].q += oriCharge[i].q;
	}
	return curCharge;
}

int main(int argc, char** argv) {

	//read in the information of charge distribution and grids and then do fitting
	vector<feature_t> fitGrids;
	vector<charge_t> gsCharge;
	vector<charge_t> exCharge;
	
	ifstream inFile;
	inFile.open("fitting_grid.txt",ios::in);
	if (inFile){
		string lineBuffer;
		while (getline(inFile,lineBuffer)) {
			stringstream ss(lineBuffer);
			string strBuffer;
			vector<string> lineArray;
			feature_t fitGrid;
			while (getline(ss,strBuffer,' ')){
				lineArray.push_back(strBuffer);
			}
			fitGrid.X = std::stod(lineArray[0]);
			fitGrid.Y = std::stod(lineArray[1]);
			fitGrid.Z = std::stod(lineArray[2]);
			fitGrids.push_back(fitGrid);
		}
		inFile.close();
	}
	else {
		std::cout << "Failed to open the grid file for fitting." << endl;
	}

	inFile.open("charge_gs.txt",ios::in);
	if (inFile){
		string lineBuffer;
		while (getline(inFile,lineBuffer)) {
			stringstream ss(lineBuffer);
			string strBuffer;
			vector<string> lineArray;
			charge_t chargePoint;
			while (getline(ss,strBuffer,' ')){
				lineArray.push_back(strBuffer);
			}
			chargePoint.coord.X = std::stod(lineArray[0]);
			chargePoint.coord.Y = std::stod(lineArray[1]);
			chargePoint.coord.Z = std::stod(lineArray[2]);
			chargePoint.q = std::stod(lineArray[3]);
			gsCharge.push_back(chargePoint);
		}
		inFile.close();
	}
	else {
		std::cout << "Failed to open the ground state charge distribution file." << endl;
	}

	vector<charge_t> gsChargeFitted = fitting(gsCharge, fitGrids);
	vector<charge_t>().swap(gsCharge);

	inFile.open("charge_ex.txt",ios::in);
	if (inFile){
		string lineBuffer;
		while (getline(inFile,lineBuffer)) {
			stringstream ss(lineBuffer);
			string strBuffer;
			vector<string> lineArray;
			charge_t chargePoint;
			while (getline(ss,strBuffer,' ')){
				lineArray.push_back(strBuffer);
			}
			chargePoint.coord.X = std::stod(lineArray[0]);
			chargePoint.coord.Y = std::stod(lineArray[1]);
			chargePoint.coord.Z = std::stod(lineArray[2]);
			chargePoint.q = std::stod(lineArray[3]);
			exCharge.push_back(chargePoint);
		}
		inFile.close();
	}
	else {
		std::cout << "Failed to open the excited state charge distribution file." << endl;
	}

	vector<charge_t> exChargeFitted = fitting(exCharge, fitGrids);
	vector<charge_t>().swap(exCharge);

	ofstream outFile;
	outFile.open("emd.txt", ios::out);

	if (!outFile) {
		std::cout << "cannot open out file" << endl;
		exit(1);
	}

	outFile << "dEMD" << " " << "qCT" << " " << "muEMD" << endl;

	double chargeTransferTotal = 0;
	const int POINTS_NUM = fitGrids.size();
	feature_t* featureP = new feature_t[POINTS_NUM];
	feature_t* featureQ = new feature_t[POINTS_NUM];
	double* weightP = new double[POINTS_NUM];
	double* weightQ = new double[POINTS_NUM];
	double emdResult = 0;

	for (int i = 0; i < POINTS_NUM; i++) {
		featureP[i] = gsChargeFitted[i].coord;
		featureQ[i] = exChargeFitted[i].coord;
		weightP[i] = gsChargeFitted[i].q;
		weightQ[i] = exChargeFitted[i].q;
	}
	vector<charge_t>().swap(gsChargeFitted);
	vector<charge_t>().swap(exChargeFitted);

	for (int i = 0; i < POINTS_NUM; i++) {
		if (weightP[i] <= weightQ[i]) {
			double temWeight = weightQ[i];
			weightQ[i] = temWeight - weightP[i];
			weightP[i] = 0;
		}
		else {
			double temWeight = weightP[i];
			weightP[i] = temWeight - weightQ[i];
			weightQ[i] = 0;
		}
		chargeTransferTotal += weightP[i];
	}

	TsSignature<feature_t> srcSig(POINTS_NUM, featureP, weightP);
	TsSignature<feature_t> snkSig(POINTS_NUM, featureQ, weightQ);

	TsFlow* flow = new TsFlow[POINTS_NUM*2-1];
	int flowVars = 0;

	clock_t start,end;
	start = clock();
	emdResult = transportSimplex(&srcSig, &snkSig, Dist, flow, &flowVars)/chargeTransferTotal;
	end = clock();

	outFile << emdResult << " " << chargeTransferTotal << " " << emdResult*chargeTransferTotal << endl;
	delete[]featureP;
	delete[]featureQ;
	delete[]weightP;
	delete[]weightQ;
	outFile.close();
}
