//
//  TSP.cpp
//  TSP_Genetic_Source_Code
//
//  Created by Alper Aytun on 2/26/18.
//  Copyright © 2018 Alper Aytun. All rights reserved.
//

#include "TSP.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <assert.h>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <iterator>
using namespace std;

TSP::TSP(double crossoverProbability, double mutationProbability, int** dMat) : crossoverProbability(crossoverProbability), mutationProbability(mutationProbability), solutions(new int[chromosones][cities]), newPopulation(new int[chromosones][cities])
{
    /* Seed the random number generator */
    srand((unsigned int)time(NULL));
    /* Use the same number to generate a specific sequence */
    //srand(0);
   
    /* Generate random population */
    randomPopulation();
    
    /* Read and get Distance Matrix */
    // distMat = readDistanceMatrix(problemName);
    distMat = dMat;
}

TSP::~TSP()
{
    delete [] distMat;
}

void TSP::randomPopulation()
{
    /* Iterate throught each chromosone... */
    for(size_t chromosoneIndex = 0; chromosoneIndex < chromosones; ++chromosoneIndex)
    {
        /* ... and give it a random path */
        setRandomPath(solutions[chromosoneIndex]);
    }
}

double TSP::getBestFitness() const
{
    return evaluateFitness(bestChromosone);
}

double TSP::getAverageDistance() const
{
    double distance = 0;
    for(size_t chromosoneIndex = 0; chromosoneIndex < chromosones; ++chromosoneIndex)
    {
        distance += totalDistance(solutions[chromosoneIndex]);
    }
    return distance/chromosones;
}

string TSP::getBestPathString() const
{
    stringstream path;
    for(size_t gene = 0; gene < cities; ++gene)
    {
        if(gene != 0)
        {
            path << ",";
        }
        path << bestChromosone[gene];
    }
    return path.str();
}

double TSP::getLowestTotalDistance() const
{
    return totalDistance(bestChromosone);
}

void TSP::nextPopulation()
{
    double fitness[chromosones];
    /* Fill an array with a fitness score for each chromosone,
     * the index of a score corresponds with the chromosone's index in solutions[index]
     */
    for(size_t chromosoneIndex = 0; chromosoneIndex < chromosones; ++chromosoneIndex)
    {
        fitness[chromosoneIndex] = evaluateFitness(solutions[chromosoneIndex]);
    }
    int val = calculateUniqueFitnessValues(fitness);
    this->uniqueFitnessNum = val;
    
    /* Use elitism, find and copy over the two best chromosones to the new population */
    long int eliteIndex1 = 0;
    long int eliteIndex2 = 0;
    /* find the best solution */
    eliteIndex1 = min_element(fitness, fitness + chromosones) - fitness;
    this->bestChromosone = solutions[eliteIndex1];
    
    double highestFitness = 1e12;
    /* Find the second best solution */
    for(size_t chromosoneIndex = 0; chromosoneIndex < chromosones; ++chromosoneIndex)
    {
        if(chromosoneIndex != eliteIndex1 && fitness[chromosoneIndex] < highestFitness)
        {
            highestFitness = fitness[chromosoneIndex];
            eliteIndex2 = chromosoneIndex;
        }
    }
    
    /* Keep track of how many chromosones exists in the new population */
    size_t offspringCount = 0;
    /* Copy over the two best solutions to the new population */
    copyToNewPopulation(solutions[eliteIndex1], offspringCount);
    ++offspringCount;
    copyToNewPopulation(solutions[eliteIndex2], offspringCount);
    ++offspringCount;
    
    /* Create the rest of the new population, break this loop when the new population is complete */
    while(true)
    {
        int * parentA;
        int * parentB;
        parentA = tournamentSelection(fitness);
        parentB = tournamentSelection(fitness);
        while (parentB == parentA)
        {
            parentB = tournamentSelection(fitness);

        }
        
        int offspringA[cities];
        int offspringB[cities];
        orderCx(parentA, parentB, offspringA, offspringB);
        mutate(offspringA);
        mutate(offspringB);
        
        /* Add to new population if an equal chromosone doesn't exist already */
        if(!hasDuplicate(offspringA, offspringCount))
        {
            copyToNewPopulation(offspringA, offspringCount);
            ++offspringCount;
        }
        /* We need to check if the new population is filled */
        if(offspringCount == chromosones)
        {
            break;
        }
        if(!hasDuplicate(offspringB, offspringCount))
        {
            copyToNewPopulation(offspringB, offspringCount);
            ++offspringCount;
        }
        /* Check again so that we don't accidentaly write all over the heap and have to spend an evening wondering why the heap is corrupt... :) */
        if(offspringCount == chromosones)
        {
            break;
        }
    }
    
    /*
     * We now have a new population,
     * now it needs to replace the current population
     * so that we don't go through the same population every time we run this function
     */
    for(size_t chromosoneIndex = 0; chromosoneIndex < chromosones; ++chromosoneIndex)
    {
        memcpy(solutions[chromosoneIndex], newPopulation[chromosoneIndex], sizeof(int) * cities);
    }
}

bool TSP::hasDuplicate(const int * const chromosone, size_t populationCount)
{
    /* Iterate throught each chromosone in newPopulation and compare them gene by gene */
    for(size_t chromosoneIndex = 0; chromosoneIndex < populationCount; ++chromosoneIndex)
    {
        int genesCompared = 0;
        for(size_t gene = 0; gene < cities; ++gene)
        {
            if(chromosone[gene] != newPopulation[chromosoneIndex][gene])
            {
                /* These chromosones are not equal! */
                break;
            }
            ++genesCompared;
        }
        
        if(genesCompared == cities)
        {
            return true;
        }
    }
    
    return false;
}

void TSP::mutate(int * const chromosone)
{
    /* 0.0 <= random <= 1 */
    {
        double random = randomInclusive(1);
        /* Nope, didn't happen */
        if(random > mutationProbability)
        {
            return;
        }
    }
    
    int tmp;
    int random1 = (int)randomExclusive(cities);
    int random2 = (int)randomExclusive(cities);
    while(random1 == random2)
    {
        random2 = (int)randomExclusive(cities);
    }
    
    tmp = chromosone[random1];
    chromosone[random1] = chromosone[random2];
    chromosone[random2] = tmp;
    
}

void TSP::orderCx(int const * const parentA, const int * const parentB, int * offspringA, int * offspringB)
{
    
    /* Perform multi-point crossover to generate offspring */
    
    /* 0 <= cuttOffIndex <= cities */
    int cuttOffIndex1 = (int)randomInclusive(cities);
    int cuttOffIndex2 = (int)randomInclusive(cities);
    while(cuttOffIndex2 == cuttOffIndex1)
    {
        cuttOffIndex2 = (int)randomExclusive(cities);
    }
    
    unsigned int start;
    unsigned int end;
    if(cuttOffIndex1 < cuttOffIndex2)
    {
        start = cuttOffIndex1;
        end = cuttOffIndex2;
    }
    else
    {
        start = cuttOffIndex2;
        end = cuttOffIndex1;
    }
    
    int *chunkA;
    int *chunkB;
    int cLength(end - start);
    chunkA = new int[cLength];
    chunkB = new int[cLength];
    
    for (int i = start; i < end; i++)
    {
        chunkA[i -start] = parentB[i]; // kesip aldigimiz kismi bir arraya koydum
        chunkB[i-start] = parentA[i]; // kesip aldigimiz kismi bir arraya koydum
        offspringA[i] = parentB[i]; // kesilen kismi iki parentta replace et
        offspringB[i] = parentA[i];
    }
    orderResorter(chunkA, parentA, offspringA, cLength, start, end);
    orderResorter(chunkB, parentB, offspringB, cLength, start, end);
    delete [] chunkA;
    delete [] chunkB;
    chunkA = 0;
    chunkB = 0;
    
}

void TSP::orderResorter(int* chunk, const int* parent, int* offspring, int cLength, int start, int end)
{
    vector<int> checkList;
    int flag;
    
    // tailpart (from index end to index cities)
    for (int i = end; i < cities; i++)
    {
        flag = 0;
        for(int j = 0; j < cLength; j++)
        {
            
            if (parent[i] == chunk[j])
            {
                flag = 1;
            }
        }
        if (flag == 0)
            checkList.push_back(parent[i]);
    }
    
    // head part (from index 0 to index start)
    for (int i = 0; i < end; i++)
    {
        flag = 0;
        for(int j = 0; j < cLength; j++)
        {
            if (parent[i] == chunk[j])
            {
                flag = 1;
            }
        }
        if (flag == 0)
            checkList.push_back(parent[i]);
    }
    
//    cout << "checkList is : " << endl;
//    
//    for (int i = 0; i < checkList.size(); i++ )
//        cout << checkList.at(i) << " ";
//    
//    cout << endl << endl;
    
    int counter = 0;
    // fill the tail part of offspring
    for(int i = end; i < cities; i++ )
    {
        offspring[i] = checkList.at(counter);
        counter++;
    }
    
    // fill the head part of the offspring
    for(int i = 0; i < start; i++ )
    {
        offspring[i] = checkList.at(counter);
        counter++;
    }
}
void TSP::crossover(int const * const parentA, const int * const parentB, int * offspringA, int * offspringB)
{
    {
        /* There is a chance we don't perform a crossover,
         * in that case the offspring is a copy of the parents
         */
        /* 0.0 <= random <= 1 */
        double random = randomInclusive(1);
        /* The offspring is a copy of their parents */
        if(random > crossoverProbability)
        {
            memcpy(offspringA, parentA, sizeof(int) * cities);
            memcpy(offspringB, parentB, sizeof(int) * cities);
            return;
        }
    }
    /* Perform multi-point crossover to generate offspring */
    
    /* 0 <= cuttOffIndex <= cities */
    int cuttOffIndex1 = (int)randomInclusive(cities);
    int cuttOffIndex2 = (int)randomInclusive(cities);
    while(cuttOffIndex2 == cuttOffIndex1)
    {
        cuttOffIndex2 = (int)randomExclusive(cities);
    }
    
    unsigned int start;
    unsigned int end;
    if(cuttOffIndex1 < cuttOffIndex2)
    {
        start = cuttOffIndex1;
        end = cuttOffIndex2;
    }
    else
    {
        start = cuttOffIndex2;
        end = cuttOffIndex1;
    }
    /* Offspring A is initially copy of parent A */
    memcpy(offspringA, parentA, sizeof(int) * cities);
    /* Offspring B is initially copy of parent B */
    memcpy(offspringB, parentB, sizeof(int) * cities);
    
    /* Put a sequence of parent B in offspring A */
    memcpy(offspringA + start, parentB + start, sizeof(int) * (end - start));
    /* Put a sequence of parent A in offspring B */
    memcpy(offspringB + start, parentA + start, sizeof(int) * (end - start));
    
    /* Mark collisions in offspring with -1*/
    for(size_t cityIndex = 0; cityIndex  < cities; ++cityIndex)
    {
        /* Index is part of the parent sequence */
        if((cityIndex  >= start && cityIndex  < end)) {
            /* Do nothing, we want to keep this sequence intact */
        }
        else
        {
            /* Check if the item at cityIndex also occurs somewhere in the copied substring */
            for(size_t substringIndex = start; substringIndex < end; ++substringIndex)
            {
                /* A duplicate, mark it */
                if(offspringA[cityIndex] == offspringA[substringIndex])
                {
                    offspringA[cityIndex] = -1;
                }
                if(offspringB[cityIndex] == offspringB[substringIndex])
                {
                    offspringB[cityIndex] = -1;
                }
            }
        }
        
    }
    
    /*
     * Go through the offspring,
     * if an element is marked we fill the hole with an element from the other offspring
     */
    for(size_t offspringIndex = 0; offspringIndex < cities; ++offspringIndex)
    {
        /* There is a hole here */
        if(offspringA[offspringIndex] == -1)
        {
            repairOffspring(offspringA, offspringIndex, offspringB);
        }
        if(offspringB[offspringIndex] == -1)
        {
            repairOffspring(offspringB, offspringIndex, offspringA);
        }
    }
}

void TSP::repairOffspring(int * const offspringToRepair, int missingIndex, const int * const other)
{
    /* Iterate through the other offspring until we find an element which doesn't exist in the offspring we are repairing */
    for(size_t patchIndex = 0; patchIndex < cities; ++patchIndex)
    {
        /* Look for other[patchIndex] in offspringToRepair */
        int *missing = find(offspringToRepair, offspringToRepair + cities, other[patchIndex]);
        
        /* The element at other[patchIndex] is missing from offspringToRepair */
        if(missing == (offspringToRepair + cities))
        {
            //cout << "1:" << offspringToRepair[missingIndex] << endl;
            offspringToRepair[missingIndex] = other[patchIndex];
            //cout << "2:" << offspringToRepair[missingIndex] << endl;
            break;
        }
    }
}

void TSP::copyToNewPopulation(int const * const chromosone, size_t index)
{
    assert(index < chromosones && "Index out of bounds");
    for(size_t i = 0; i < cities; ++i)
    {
        newPopulation[index][i] = chromosone[i];
    }
    
}

int * TSP::tournamentSelection(double const * const fitness) const
{
//   srand((unsigned int) time(0));
    
    int num1, num2, num3;
    double fitnesses[3];
    int indices[3];
    
    num1 = rand() % chromosones;
    indices[0] = num1;
    fitnesses[0] = fitness[num1];
    
    num2 = rand() % chromosones;
    indices[1] = num2;
    fitnesses[1] = fitness[num2];
    
    num3 = rand() % chromosones;
    indices[2] = num3;
    fitnesses[2] = fitness[num3];
    
    //std::cout << num1 << "\t" << num2 << "\t" << num3 << std::endl;
    int count, lowest, min_idx;
    
    lowest = fitnesses[0];
    min_idx = indices[0];
    for (count = 0; count < 3; count++)
    {
        if (fitnesses[count] < lowest)
        {
            lowest = fitnesses[count];
            min_idx = indices[count];
        }
        
    }
    return solutions[min_idx];
}

int * TSP::rouletteSelection(double const * const fitness) const
{
    double sum = 0;
    /* Calculate sum of all chromosome fitnesses in population */
    for(size_t i = 0; i < chromosones; ++i)
    {
        sum += fitness[i];
    }
    
    /* 0.0 <= random <= sum */
    double random = randomInclusive(sum);
    
    sum = 0;
    /* Go through the population and sum fitnesses from 0 to sum s. When the sum s is greater or equal to r; stop and return the chromosome where you are */
    for(size_t i = 0; i < chromosones; ++i)
    {
        sum += fitness[i];
        if(sum >= random)
        {
            return solutions[i];
        }
    }
    assert(false && "A chromosone should have been picked by now");
    return(NULL);
}

void TSP::setRandomPath(int * chromosone)
{
    for(int i = 0; i < cities; ++i)
    {
        chromosone[i] = i;
    }
    
    /*
     * Shuffle the chromosone using the Fisher–Yates shuffle.
     */
    for(int i = cities-1; i > 0; --i)
    {
        /* 0 <= random <= i */
        int random = (int)randomInclusive(i);
        int temp = chromosone[i];
        chromosone[i] = chromosone[random];
        chromosone[random] = temp;
    }
}

double TSP::evaluateFitness(int const * const chromosone) const
{
    return totalDistance(chromosone);
}

double TSP::totalDistance(int const * const chromosone) const
{
    double distance = 0;  
    /* Calculate the total distance between all cities */
    distance += distMat[chromosone[cities-1]][chromosone[0]];
    
    for(size_t i = 0; i < cities-1; ++i)
    {
        distance += distMat[chromosone[i]][chromosone[i+1]];
    }
    
    return distance;
}

double TSP::randomInclusive(double max)
{
    /* Generate random number r, 0.0 <= r <= max */
    //return ((double)rand() / (double)RAND_MAX * max);
    return ((double)rand() * max) / (double)RAND_MAX;
}

double TSP::randomExclusive(double max)
{
    /* Generate random number r, 0.0 <= r < max */
    //return ((double)rand() / ((double)RAND_MAX + 1) * max);
    return ((double)rand() * max) / ((double)RAND_MAX + 1);
}

long int TSP::calculateUniqueFitnessValues(double* fitness)
{
    std::set<int> uniqueFitnesses(fitness,fitness + chromosones);
    return uniqueFitnesses.size();
}

int TSP::getUniqueFitnessNum()
{
    return this->uniqueFitnessNum;
}

void TSP::setUniqueFitnessValue(int val)
{
    this->uniqueFitnessNum = val;
}