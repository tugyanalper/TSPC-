//
//  TSP.hpp
//  TSP_Genetic_Source_Code
//
//  Created by Alper Aytun on 2/26/18.
//  Copyright © 2018 Alper Aytun. All rights reserved.
//

#ifndef TSP_hpp
#define TSP_hpp

#include <stdio.h>
#pragma once
#include <string>

class TSP

{
public:
    //constructor for TSP Class
    TSP(const double crossoverProbability, const double mutationProbability, int** distMat);
    
    // Destructor for TSP Class
    ~TSP();
    
    /* The constants used in this project */
    static const unsigned int chromosones = 80, cities = 280;
    
    /* Generate a random population of chromosones */
    void randomPopulation();
    
    /* Create a new population using crossover and mutation */
    void nextPopulation();
    
    /* Returns the fitness of the best chromosone */
    double getBestFitness() const;
    
    /* Returns a string representation of the best path */
    std::string getBestPathString() const;
    
    /* Returns the total distance of the best chromosone path */
    double getLowestTotalDistance() const;
    
    /* Returns the populations average length */
    double getAverageDistance() const;
    
    int getUniqueFitnessNum();
    
private:
    const double crossoverProbability, mutationProbability;
    int ** distMat;
    int uniqueFitnessNum;
    
    /* Gets the total distance of the supplied path */
    double totalDistance(int const * const chromosone) const;
    
    /* The chromosone containing the shortest path */
    int *bestChromosone;
    
    /* Contains the current population of chromosones */
    int (*solutions)[cities];  // solutions point to an array that holds number cities integer
    /* The two chromosones with the best fitness functions */
    //bestChromosone1[cities], bestChromosone2[cities],
    /* Used to store the new chromosones when creating a new population */
    int (*newPopulation)[cities];
    
    /* Returns a random double r, 0 <= r <= max */
    static double randomInclusive(const double max);
    
    /* Returns a random double r, 0 <= r < max */
    static double randomExclusive(const double max);
    
    /* True if the two chromosones represent the same path */
    static bool areChromosonesEqual(int const * const chromosoneA, int const * const chromosoneB);
    
    /* Evaluate the fitness the supplied chromosone */
    double evaluateFitness(const int * const chromosone) const;
    
    /* Selects a chromosone from the current population using Roulette Wheel Selection.
     * Using the algorithm described in http://www.obitko.com/tutorials/genetic-algorithms/selection.php.
     */
    int * rouletteSelection(double const * const fitness) const;
    
    /* Tournament Selection uses 3 individual tournament */
    int * tournamentSelection(double const * const fitness) const;
    
    /* Replace the element at offspringIndex with the first element found in other that does not exist in offspringToRepair */
    void repairOffspring(int * const offspringToRepair, int missingIndex, const int * const other);
    
    /* Might swap one gene with another, depending on the mutation probability */
    void mutate(int * const chromosone);
    
    /* Cross over the parents to form new offspring using Multi-Point Crossover, collisions are handled as shown in lecture 5.
     * The chromosones might be a copy of their parents, depending on the crossover probability.
     */
    void crossover(const int * const parentA, const int * const parentB, int * const offspringA, int * const offspringB);
    
    // Order Crossover Implementation
    void orderCx(int const * const parentA, const int * const parentB, int * offspringA, int * offspringB);
    
    void orderResorter(int* chunk, const int* parent, int* offspring, int cLength, int start, int end);
    
    /* Checks if the supplied chromosone is in newPopulation */
    bool hasDuplicate(const int * const chromosone, size_t populationCount);
    
    /* Copies the supplied chromosone to the new population */
    void copyToNewPopulation(const int * const chromosone, size_t index);
    
    /* Make the chromosone represent a path, which is chosen by random */
    static void setRandomPath(int * const chromosone);

    long int calculateUniqueFitnessValues(double*);
    
    void setUniqueFitnessValue(int);
};

#endif /* TSP_hpp */
