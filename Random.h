#ifndef _RANDOM_HEADER_
#define _RANDOM_HEADER_

#include <vector>
#include <cassert>
#include <memory>
#include <iostream>

#define     LONG  long long
#define    ULONG  unsigned long long
#define     REAL  double

class NumGenParent {
private:

	std::string mode;

public:

	NumGenParent( std::string mode_in ) : mode(mode_in) {};
	~NumGenParent() {};

	std::string getMode() { return mode; };

	// call to return a uniform random number
	virtual double Urand() = 0;

	virtual void RN_init_particle( unsigned long long nps ) = 0;

	virtual ULONG  RN_skip_ahead( ULONG* seed, LONG* nskip ) = 0;

	virtual bool   RN_test_basic(void) = 0;
};

class Rand : public NumGenParent {

public:
	
	Rand() : NumGenParent("Random") {};
	~Rand() {};

	double Urand();

	void RN_init_particle( unsigned long long nps );

	ULONG  RN_skip_ahead( ULONG* seed, LONG* nskip );

	bool   RN_test_basic(void);
};

class Testing : public NumGenParent {

private:

	int testIndex;
	std::vector<double> loopThrough;

public:

	//default ctor and with input depending on possible changes in implementation
	Testing( std::vector<double> loopThrough_in ) : NumGenParent("Testing"), 
													testIndex(0), loopThrough(loopThrough_in) {};
	Testing() : NumGenParent("Testing"), testIndex(0) {};
	~Testing() {};

	//Set the vector the testing mode will return 
	void setLoopThrough( std::vector<double> loopThrough_in ){
		loopThrough = loopThrough_in;
		testIndex = 0;

		//For debugging - delete after ***
		std::cout << "My Loop through was set to: " << std::endl;
		for (int i = 0; i < loopThrough.size(); ++i){
			std::cout << loopThrough.at(i) << " ";
		}
		std::cout << std::endl;
	}

	//Functions the same as Rand's but iterates over a known vector
	double Urand() override;

	void RN_init_particle( unsigned long long nps ) {assert(false);};

	ULONG  RN_skip_ahead( ULONG* seed, LONG* nskip ) {assert(false);};

	bool   RN_test_basic(void) {assert(false);};
};

//Construct one instance of each derived class
static Rand randNumGen;
static Testing testNumGen;

//Make polymorphic pointer for rest of program to use
static NumGenParent * rng = &randNumGen;

//This function will set the loopThrough of testNumGen to the input and make rng point to it
void activateTesting( std::vector< double > loopThrough_in );

#endif
