//
//  main.cpp
//  TSP_Genetic_Source_Code
//
//  Created by Alper Aytun on 2/26/18.
//  Copyright © 2018 Alper Aytun. All rights reserved.
//

#include <stdio.h>
#include "TSP.hpp"
#include <iostream>
#include <fstream>
#include <limits>
#include <assert.h>
#include <vector>
#include <cmath>
#include "ProblemReader.hpp"
using namespace std;

int main(int argc, const char *argv[])
{
    /* 90% mutation probability, 2% mutation probability */
//    int dimension = getDimension("bays29.txt");
    int** dMat = readDistanceMatrix("a280.tsp");
    TSP *tsp = new TSP(0.9, 0.02, dMat);
    size_t generations = 0, generationsWithoutImprovement = 0;
    double bestFitness = 1e12;
    double initialAverage = tsp->getAverageDistance();

    ofstream myfile;
    myfile.open("results.txt");
    /* We'll stop when we've gone 10k generations without improvement */
    while(generationsWithoutImprovement < 5000)
    {
        tsp->nextPopulation();
        ++generations;
        double newFitness = tsp->getBestFitness();
        bool fImproved = false;
        /* The new fitness is higher, the chromosone is better */
        if(newFitness < bestFitness)
        {
            bestFitness = newFitness;
            generationsWithoutImprovement = 0;
            cout << "Best fitness: " << bestFitness <<  "\tNumber of Unique Fitness Values: " << tsp->getUniqueFitnessNum() << endl;
            fImproved = true;
        }
        else
        {
            ++generationsWithoutImprovement;
        }
        myfile << generations << "," << bestFitness << "," << fImproved << "," << tsp->getAverageDistance() << "," << tsp->getUniqueFitnessNum() << endl;
    }
    myfile.close();
    cout << "DONE!" << endl;
    cout << "Number of generations: " << generations << endl;
    cout << "Best chromosone info: " << endl;
    cout << "\t-Path: " << tsp->getBestPathString() << endl;
    //cout << "\t-Goal function: " << tsp->getBestFitness() << endl;
    cout << "\t-Distance: " << tsp->getLowestTotalDistance() << endl;
    cout << "Average distance: " << tsp->getAverageDistance() << endl;
    cout << "Initial average: " << initialAverage << endl;

    delete tsp;

    return 0;
}