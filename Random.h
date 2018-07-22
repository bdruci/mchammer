#ifndef _RANDOM_HEADER_
#define _RANDOM_HEADER_

#include <vector>
#include <cassert>
#include <memory>
#include <iostream>

#define     LONG  long long
#define    ULONG  unsigned long long
#define     REAL  double

//Parent class to hold a true rand num gen and a testing class
class RandomNumberGenerator {
private:

	std::string mode;

public:

	RandomNumberGenerator( std::string mode_in ) : mode(mode_in) {};
	~RandomNumberGenerator() {};

	std::string getMode() { return mode; };

	virtual double Urand() = 0;

	virtual void RN_init_particle( unsigned long long nps ) = 0;

	virtual ULONG  RN_skip_ahead( ULONG* seed, LONG* nskip ) = 0;

	virtual bool   RN_test_basic(void) = 0;
};

//True Random class
class Rand : public RandomNumberGenerator {

public:
	
	Rand() : RandomNumberGenerator("Random") {};
	~Rand() {};

	// call to return a uniform random number
	double Urand();

	void RN_init_particle( unsigned long long nps );

	ULONG  RN_skip_ahead( ULONG* seed, LONG* nskip );

	bool   RN_test_basic(void);
};

//This class is for testing purposes
class ReturnSetNums : public RandomNumberGenerator {
//Usage: construct a ReturnSetNums object in testing file and set rng equal to it by reference
private:

	int testIndex;
	std::vector<double> loopThrough;

public:

	ReturnSetNums( std::vector<double> loopThrough_in ) : RandomNumberGenerator("Testing"), 
													testIndex(0), loopThrough(loopThrough_in) {};
	ReturnSetNums() : RandomNumberGenerator("Testing"), testIndex(0) {};
	~ReturnSetNums() {};

	//Set the vector the testing mode will return 
	void setLoopThrough( std::vector<double> loopThrough_in ){
		loopThrough = loopThrough_in;
		testIndex = 0;
	}

	//Set the place in the loopThrough vector
	void setIndex( int index ){
		testIndex = index;
	}

	//Functions the same as Rand's but iterates over known vector
	double Urand() override;

	//Init in this case just resets the index
	void RN_init_particle( unsigned long long nps ) { testIndex = 0; };

	//Shouldn't be called
	ULONG  RN_skip_ahead( ULONG* seed, LONG* nskip ) {assert(false);};

	//Shouldn't be called
	bool   RN_test_basic(void) {assert(false);};
};

//Construct instance of Rand class so only one has Urand called on it
static Rand randNumGen;

//Make polymorphic pointer for rest of program to use
static RandomNumberGenerator * rng = &randNumGen;

#endif
