//
//  ProblemReader.hpp
//  TSP_Genetic_Source_Code
//
//  Created by Alper Aytun on 3/4/18.
//  Copyright © 2018 Alper Aytun. All rights reserved.
//

#ifndef ProblemReader_hpp
#define ProblemReader_hpp

#include <stdio.h>
int getDimension(std::string pname);

std::string getEdgeWeightType(std::string pname);
std::string getEdgeWeightFormat(std::string pname);
std::string getName(std::string pname);
int** readExplicit(std::string pname, int dimension);
int** readEUC2D(std::string pname, int dimension);
int EUC2D_distance(double* coord1, double* coord2);

int** readDistanceMatrix(std::string pname);

#endif /* ProblemReader_hpp */
