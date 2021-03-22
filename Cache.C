#include <iostream>
#include <string>
#include <assert.h>
#include <bitset>
#include "Set.h"
#include "Cache.h"

//TO DO: most of this is incomplete
//       look for TO DO throughout the file

/*
 * Cache
 * A cache is a set of Sets. Create and initialize the sets.
 * Initialize the Cache object.
 */
Cache::Cache(int32_t associativity, int32_t blockOffsetBits, int32_t setIndexBits)
{
   int32_t i;

   numSets = 1<<setIndexBits;  

   //The rest of this is complete.
   sets = new Set[numSets];
   for (i = 0; i < numSets; i++)
   {
       sets[i].initialize(associativity);
   }
   this->associativity = associativity;
   this->blockOffsetBits = blockOffsetBits;
   this->setIndexBits = setIndexBits;
}

/* 
 * isHit
 * Returns true if access the cache with the provided address results in
 * a hit. Calls isHit in the Set class.
 */
bool Cache::isHit(int64_t address)
{
	int tag = getBits(blockOffsetBits+setIndexBits, 63, address);
	int index = getBits(blockOffsetBits, blockOffsetBits+setIndexBits-1,address);

	if (sets[index].isHit(tag) == true){
		return true;
	}
	return false;

}

/*
 * isEvict
 * Returns true if accessing the cache with the provided address results in
 * an eviction because the Set is full. Calls isFull in the Set class.
 */

bool Cache::isEvict(int64_t address)
{
	int tag = getBits(blockOffsetBits+setIndexBits, 63, address);
	int index = getBits(blockOffsetBits, blockOffsetBits+setIndexBits-1,address);
	
	if (sets[index].isFull() == true){
		return true;
	}
	return false;
}

/*
 * update
 * Updates the set in which the tag of the address is to be stored
 * by calling the update method on the appropriate Set object.
 */
void Cache::update(int64_t address)
{
	int tag = getBits(blockOffsetBits+setIndexBits, 63, address);
	int index = getBits(blockOffsetBits, blockOffsetBits+setIndexBits -1,address);

	sets[index].update(tag); 
}

/*
 * getBits
 * Takes as input a source, a starting bit number, and an ending bit number
 * and returns those bits from the source.
 * For example, getBits(60, 62, 0x7000000000000000) returns 7
 */
int64_t Cache::getBits(int32_t start, int32_t end, int64_t source)
{
   return (((1 << ((end - start)+1)) -1) & (source >> start));
}

/*
 * Destruct the dynamically allocated data.
 */
Cache::~Cache()
{
   //TO DO
}

/*
 * print
 * Output the contents of the cache.  This may be useful for debugging.
 * You can add a call to this in the Simulate run method.
*/
void Cache::print()
{
   std::cout << "\nNumber of Sets: " << numSets << ", ";
   std::cout << "Associativity: " << associativity ;

   int32_t i, j;
   for (i = 0; i < numSets; i++)
   {
      sets[i].print();
   }
}

