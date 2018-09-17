/*
 Author: Blake
 Date: 11/21/17
 Main File
 */


#include "Transport.h"
using std::make_shared;

void Transport::endHist() {
  // end history for all estimators in the problem
  for (auto est : estimators) {
    est->endHist();
  }
}

//constructor
Transport::Transport(Geom_ptr geoin, Cons_ptr consti, Mesh_ptr meshin , Time_ptr timein, vector < EstCol_ptr > estimatorsin ): geometry(geoin) , constants(consti), 
  mesh(meshin) , timer(timein) ,  estimators(estimatorsin) {}
 
void Transport::runTransport()
{
    numHis = constants->getNumHis();

    // construct event handlers
    CollisionHandler       collisionHandler;
    SurfaceCrossingHandler surfaceCrossingHandler;

    for( unsigned long long i = 0; i < numHis; i++ )
    {
        //start a timer
        timer->startHist();
	      RN_init_particle(i);
        
        //sample source
        Part_ptr p_new = geometry->sampleSource();
        Cell_ptr startingCell = geometry->whereAmI(p_new->getPos());
        p_new->setCell(startingCell);
        pstack.push(p_new);
          
        //run history
        while(!pstack.empty())
        {
           Part_ptr p = pstack.top();
            while(p->isAlive())
            {
                Cell_ptr current_Cell = p->getCell();

                double d2s = current_Cell->distToSurface(p);
                double d2c = current_Cell->distToCollision( p->getGroup() );
                
                if(d2s > d2c) //collision!
                {
                    // keep track of initial position and move particle
                    point oldPos = p->getPos();
                    p->move(d2c);

                    // handle estimators 
                    timer->startTimer("handling estimators at collision");
                    collisionHandler.setCurrentCell(current_Cell);
                    collisionHandler.setCurrentTet( mesh->whereAmI( p->getPos() ) );
                    collisionHandler.score( std::make_shared<point>(oldPos) , p );  
                    timer->endTimer("handling estimators at collision"); 


                    // sample the reaction
                    React_ptr reactionToSample = current_Cell->getMat()->sampleCollision( p->getGroup() );
                    reactionToSample->sample( p, pstack );
                    p->kill(); //TODO: make this not awful
                }
                else //hit surface
                {
                    // prepare the SurfaceCrossingHandler
                    // set the surface to be crossed given particle position & direction
                    surfaceCrossingHandler.setSurfaceCrossed( current_Cell->getClosestSurface( p ) );
                    // set the cell the particle will be leaving
                    surfaceCrossingHandler.setCellLeft(current_Cell);                 
                
                    // keep track of initial position and move particle
                    point oldPos = p->getPos();
                    p->move(d2s + 0.0000001);
                    
                    // find the new cell the particle is in
                    Cell_ptr newCell = geometry->whereAmI(p->getPos());
                    
                    // score any relevant estimators at the surface crossing
                    timer->startTimer("scoring estimators at surface crossing");
                    surfaceCrossingHandler.score( std::make_shared<point>(oldPos) , p );  
                    timer->endTimer("scoring estimators at surface crossing");
                    
                if(newCell == nullptr)
                {
                    p->kill();
                }
                else
                {
                    p->setCell(newCell);
                }
                }
            }
            pstack.pop();
        }
        // end the history in all the estimators
        endHist();

        // end the history timer
        timer->endHist();
    }
}

void Transport::output() {
    cout << std::endl << "Total Number of Histories: " << numHis << endl;

    int i = 0;
    /*
    for( Cell_ptr cell : geometry->getCells() ) {
        ++i;
        std::cout << "Collision tally in cell " << i << std::endl;
        for( int j = 1; j <= constants->getNumGroups(); ++j) {
            std::cout << " group: " << j << ", tally = " << cell->getSingleGroupTally(j, numHis).first 
                      << ", stddev = " << cell->getSingleGroupTally(j, numHis).second << std::endl;
        }
        cout << std::endl;
    }
    */

    // print timing information
    timer->printAvgResults();

    // print mesh estimators to file
    mesh->printMeshTallies();
    if ( constants->getAllTets() ) {
        mesh->writeToVTK();
    }
}
