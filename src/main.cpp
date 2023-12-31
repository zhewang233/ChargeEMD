#include "emd.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstring>

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

bool ifPathExist(const std::string& name) {
	ifstream f(name.c_str());
	return f.good();
}

int main(int argc, char** argv) {

	//read in the information of charge distribution and grids and then do fitting
	vector<feature_t> fitGrids;
	vector<charge_t> gsCharge;
	vector<charge_t> exCharge;

	if (argc == 2 && (strcmp(argv[1],"-h") == 0 || strcmp(argv[1],"--help") == 0)) {
		std::cout << "Usage: ./emd [grid file for fitting] [ground state charge distribution file] [excited state charge distribution file] [output file]" << endl;
		std::cout << "If the output file is not specified, the results will be printed on the screen." << endl;
		exit(1);
	}
	else

	if (argc < 4) {
		std::cout << "Please input the path of the grid file for fitting, the path of the ground state charge distribution file, and the path of the excited state charge distribution file." << endl;
		exit(1);
	}

	if (argc > 5) {
		std::cout << "Too many arguments." << endl;
		exit(1);
	}

	std::string fitGridPath = argv[1];
	std::string gsChargePath = argv[2];
	std::string exChargePath = argv[3];
	
	if (!ifPathExist(fitGridPath)) {
		std::cout << "The grid file for fitting does not exist." << endl;
		exit(1);
	}
	if (!ifPathExist(gsChargePath)) {
		std::cout << "The ground state charge distribution file does not exist." << endl;
		exit(1);
	}
	if (!ifPathExist(exChargePath)) {
		std::cout << "The excited state charge distribution file does not exist." << endl;
		exit(1);
	}
	if (argc == 5){
		std::string emdOutputPath = argv[4];
		if (emdOutputPath.find_last_of("/") != std::string::npos && !ifPathExist(emdOutputPath.substr(0,emdOutputPath.find_last_of("/")))) {
			std::cout << "The path for storing output file does not exist." << endl;
			exit(1);
		}
	}

	ifstream inFile;
	inFile.open(fitGridPath,ios::in);
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

	inFile.open(gsChargePath,ios::in);
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

	inFile.open(exChargePath,ios::in);
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

	if (argc == 5) {
		std::string emdOutputPath = argv[4];
		ofstream outFile;
		outFile.open(emdOutputPath, ios::out);

		if (!outFile) {
			std::cout << "cannot open out file" << endl;
			exit(1);
		}

		outFile << "dEMD" << " " << "qCT" << " " << "muEMD" << endl;

		outFile << emdResult << " " << chargeTransferTotal << " " << emdResult*chargeTransferTotal << endl;
		outFile.close();
	}
	else {
		std::cout << "dEMD = " << emdResult << ", qCT = " << chargeTransferTotal << ", muEMD = " << emdResult*chargeTransferTotal << endl;
	}

	delete[]featureP;
	delete[]featureQ;
	delete[]weightP;
	delete[]weightQ;
}
