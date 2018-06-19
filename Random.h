#ifndef _RANDOM_HEADER_
#define _RANDOM_HEADER_

#include <vector>
#include <cassert>
#include <memory>

/*
// call to return a uniform random number
void activateTesting( std::vector< double > inputVec );

// call to return a uniform random number
double Urand( void );

// call at the beginning of the problem
void RN_init_problem( unsigned long long* new_seed, int* print_info );

// call at the beginning of each history, nps = index for the history
void RN_init_particle( unsigned long long nps );
*/
#define     LONG  long long
#define    ULONG  unsigned long long
#define     REAL  double

class NumberGen {
public:
	// call to return a uniform random number
	virtual double Urand() = 0;

	virtual void RN_init_particle( unsigned long long nps ) = 0;

	virtual ULONG  RN_skip_ahead( ULONG* seed, LONG* nskip ) = 0;

	virtual bool   RN_test_basic(void) = 0;
};

class Rand : public NumberGen {

public:
	Rand();
	~Rand();

	double Urand() override;

	void RN_init_problem( unsigned long long* new_seed, int* print_info );

	void RN_init_particle( unsigned long long nps );

	ULONG  RN_skip_ahead( ULONG* seed, LONG* nskip );

	bool   RN_test_basic(void);
};

class Testing : public NumberGen {

private:

	int testIndex;
	std::vector<double> loopThrough;

public:

	Testing( std::vector<double> loopThrough_in ) : testIndex(0), loopThrough(loopThrough_in) {};
	Testing() : testIndex(0) {};
	~Testing();

	void setLoopThrough( std::vector<double> loopThrough_in ){
		loopThrough = loopThrough_in;
	}

	double Urand() override;

	void RN_init_problem( unsigned long long* new_seed, int* print_info ) {assert(false);};

	void RN_init_particle( unsigned long long nps ) {assert(false);};

	ULONG  RN_skip_ahead( ULONG* seed, LONG* nskip ) {assert(false);};

	bool   RN_test_basic(void) {assert(false);};
};

//typedef std::shared_ptr<NumberGen> gen_ptr;

static Rand randNumGen;

static NumberGen * getNum = &randNumGen;

void activateTesting( std::vector<double> loopThrough_in );

#endif
