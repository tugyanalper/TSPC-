//
//  ProblemReader.cpp
//  TSP_Genetic_Source_Code
//
//  Created by Alper Aytun on 3/4/18.
//  Copyright © 2018 Alper Aytun. All rights reserved.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include "ProblemReader.hpp"


int getDimension(std::string pname)
{
    std::ifstream myfile(pname);
    std::string line, temp;
    std::stringstream ss;
    int dimension;
    
    if (myfile.is_open())
    {
        while (getline(myfile,line))
        {
            ss.clear();
            ss.str(line);
            getline(ss, temp, ':');
            temp.erase(remove( temp.begin(), temp.end(), ' ' ), temp.end() );
            if (temp.compare("DIMENSION") == 0)
            {
                getline(ss, temp, '\n');
                temp.erase(remove( temp.begin(), temp.end(), ' ' ), temp.end() );
                dimension = stoi(temp);
                // std::cout << "Number of Cities : " << dimension << std::endl;
                myfile.close();
                break;
            }
        }
        
    }
    return dimension;
}

std::string getEdgeWeightType(std::string pname)
{
    std::ifstream myfile(pname);
    std::string line, temp;
    std::stringstream ss;
    std::string edgeType;
    
    if (myfile.is_open())
    {
        while (getline(myfile,line))
        {
            ss.clear();
            ss.str(line);
            getline(ss, temp, ':');
            temp.erase(remove( temp.begin(), temp.end(), ' ' ), temp.end() );
            if (temp.compare("EDGE_WEIGHT_TYPE") == 0)
            {
                getline(ss, temp, '\n');
                temp.erase(remove( temp.begin(), temp.end(), ' ' ), temp.end() );
                edgeType = temp;
                myfile.close();
                break;
            }
        }
        
    }
    return edgeType;
}

std::string getEdgeWeightFormat(std::string pname)
{
    std::ifstream myfile(pname);
    std::string line, temp;
    std::stringstream ss;
    std::string edgeFormat;
    
    if (myfile.is_open())
    {
        while (getline(myfile,line))
        {
            ss.clear();
            ss.str(line);
            getline(ss, temp, ':');
            temp.erase(remove( temp.begin(), temp.end(), ' ' ), temp.end() ); // remove whitespaces
            if (temp.compare("EDGE_WEIGHT_FORMAT") == 0)
            {
                getline(ss, temp, '\n');
                temp.erase(remove( temp.begin(), temp.end(), ' ' ), temp.end() );
                edgeFormat = temp;
                myfile.close();
                break;
            }
        }
        
    }
    return edgeFormat;
}

std::string getName(std::string pname)
{
    std::ifstream myfile(pname);
    std::string line, temp;
    std::stringstream ss;
    std::string name;
    
    if (myfile.is_open())
    {
        while (getline(myfile,line))
        {
            ss.clear();
            ss.str(line);
            getline(ss, temp, ':');
            temp.erase(remove( temp.begin(), temp.end(), ' ' ), temp.end() ); //remve whitespaces
            if (temp.compare("NAME") == 0)
            {
                getline(ss, temp, '\n');
                temp.erase(remove( temp.begin(), temp.end(), ' ' ), temp.end() );
                name = temp;
                myfile.close();
                break;
            }
        }
        
    }
    return name;
}

int** readExplicit(std::string pname, int dimension)
{
    std::ifstream myfile(pname);
    std::string line, temp;
    std::stringstream ss;
    if (myfile.is_open())
    {
        while (getline(myfile,line))
        {
            ss.clear();
            ss.str(line);
            getline(ss, temp, ':');
            temp.erase(remove( temp.begin(), temp.end(), ' ' ), temp.end() ); //remove whitespaces
            if (temp.compare("EDGE_WEIGHT_SECTION") == 0)// edge weight section'a kadar oku
            {
                break;
            }
            
            else continue;
        }
    }
    int** distMat = new int*[dimension]; // dynamically allocate 2d array
    for(int i = 0; i < dimension; ++i)
        distMat[i] = new int[dimension];
    
    if (myfile.is_open())
    {
        int counter(0);
        while (getline(myfile,line))
        {
            if (line.compare("DISPLAY_DATA_SECTION") != 0 ) // display data section'a kadar oku
            {
                ss.clear();
                ss.str(line);
                for (int i = 0; i < dimension; i++)
                {
                    ss >> temp;
                    distMat[counter][i] = stoi(temp);
                }
                counter += 1;
            }
            else break;
        }
        myfile.close();
    }
    
    return distMat;
}

int** readEUC2D(std::string pname, int dimension)
{
    std::ifstream myfile(pname);
    std::string line, temp;
    std::stringstream ss;
    if (myfile.is_open())
    {
        while (getline(myfile,line))
        {
            ss.clear();
            ss.str(line);
            getline(ss, temp, ':');
            temp.erase(remove( temp.begin(), temp.end(), ' ' ), temp.end() );
            if (temp.compare("NODE_COORD_SECTION") == 0)// node coord section'a kadar oku
            {
                break;
            }
            
            else continue;
        }
    }
    int** distMat = new int*[dimension]; // dynamically allocate 2d array
    for(int i = 0; i < dimension; ++i)
        distMat[i] = new int[dimension];
    
    double** coordinates = new double*[dimension];
    for(int i = 0; i < dimension; ++i)
        coordinates[i] = new double[2];
    
    if (myfile.is_open())
    {
        int counter(0);
        while (getline(myfile,line))
        {
            if (line.compare("EOF") != 0 ) // display data section'a kadar oku
            {
                ss.clear();
                ss.str(line);
                int idx = 0;
                for (int i = 0; i < 3; i++)
                {
                    ss >> temp;
                    if (i >= 1)
                    {
                        coordinates[counter][idx] = stod(temp);
                        idx += 1;
                    }
                }
                counter += 1;
            }
            else break;
        }
        myfile.close();
    }
    
    for (int i = 0; i < dimension; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            distMat[i][j] = EUC2D_distance(coordinates[i], coordinates[j]);
//            std::cout << distMat[i][j] << " ";
        }
//        std::cout << std::endl;
    }
    delete [] coordinates;
    coordinates = 0;
    return distMat;
}


int EUC2D_distance(double* coord1, double* coord2)
{
    int xdiff = coord2[0] - coord1[0];
    int ydiff = coord2[1] - coord1[1];
    
    return int(sqrt(xdiff * xdiff + ydiff * ydiff) + 0.5);
}

int** readDistanceMatrix(std::string pname)
{
    std::ifstream myfile(pname);
    std::string line, temp;
    std::stringstream ss;
    std::string edgeType;
    std::string edgeFormat;
    int** distMat;
    
    int dimension = getDimension(pname);
    edgeType= getEdgeWeightType(pname);
    if (edgeType.compare ("EXPLICIT") == 0)
    {
        edgeFormat = getEdgeWeightFormat(pname);
        distMat = readExplicit(pname, dimension);
    }
    else if (edgeType.compare ("EUC_2D") == 0)
        distMat = readEUC2D(pname, dimension);
    return distMat;
}

