#include <fstream>
#include <iostream>
#include <string>
#include <assert.h>
#include "Set.h"
#include "Cache.h"
#include "Simulate.h"


//TO DO: implement run and accessCache functions

/* Simulator constructor
 * Dynamically create a Cache object and initialize the Simulator object.
 */
Simulate::Simulate(int32_t associativity, int32_t blockOffsetBits, 
                   int32_t setIndexBits, bool verbose)
{
   hits = misses = evictions = 0;
   cacheP = new Cache(associativity, blockOffsetBits, setIndexBits);
   this->verbose = verbose;
}

/*
 * Destroy the Simulator object.
 */
Simulate::~Simulate()
{
   delete cacheP;
}

/*
 * run
 * Takes as input the name of a file of traces. Opens the
 * file and reads each address. Addresses are used
 * to perform the cache simulation.
 */
void Simulate::run(std::string filename)
{
   std::string addressStr;
   std::string type;
   std::string line;
   std::fstream fs;
   int64_t address;
   fs.open(filename, std::fstream::in);
    std::size_t pos;
	char* end;

   
   
   while(std::getline(fs, line))
	{
		if (line.length() > 1){
			if(line.at(0) == 'I' ){
				type = "I";
			}
			else if(line.at(1) == 'L' || line.at(1) == 'M' || line.at(1) == 'S' ){
				type = line.at(1);
				pos = line.find(",")-3;
				addressStr = line.substr(3,pos);
				address =  strtoull( addressStr.c_str(), NULL ,16 );

				if (verbose == true){
					std::cout << line << " ";
				}
				
				accessCache(address);
	
				if (line.at(1) == 'M') {
					accessCache(address);
				}
				
				if (verbose == true){
					std::cout << std::endl;
				}
			}
		}
	}  
}

/* accessCache
 * Takes as input a 64 bit address.
 * Uses the address to access the cache pointed to by cacheP
 * and updates hits, misses, and evictions.
 */
void Simulate::accessCache(int64_t address)
{
    //cacheP contains a pointer to the cache object
    //Call the appropriate methods in the Cache class
    //to determine whether to increment hits, misses, and/or evictions
    //If verbose is true, output the verbose output (see assignment)
	
	if (cacheP->isHit(address) == true) {
		hits++;
		if (verbose == true) {
			std::cout << "hit ";
		}
	}
	else{
		misses++;
		if (verbose == true) {
			std::cout << "miss ";
		}
		if (cacheP->isEvict(address) == true){
			evictions++;
			if (verbose == true) {
				std::cout << "eviction ";
			}
		}
	}
	cacheP->update(address);
	
}

/*
 * printSummary
 * Print the number of hits, misses, and evictions at the end of
 * the simulation.
 */
void Simulate::printSummary()
{
   std::fstream fs;
   printf("hits:%d misses:%d evictions:%d\n", hits, misses, evictions);
   fs.open(".csim_results", std::fstream::out);
   assert(fs.is_open());
   fs << hits << " " << misses << " " << evictions <<"\n";;
   fs.close();
}

