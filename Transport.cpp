/*
 Author: Blake
 Date: 11/21/17
 Main File
 */


#include "Transport.h"
using std::make_shared;

//constructor
Transport::Transport(Geom_ptr geoin, Cons_ptr consti, Mesh_ptr meshin , Time_ptr timein): geometry(geoin) , constants(consti), mesh(meshin) , timer(timein) {}

/*
void Transport::setup()
 {
 
 //read input file if it existed
 //plane at x = 0 and at x = 10 -> cell1
 //material
 vector<double> sigt;
 vector<double> siga;
 vector<double> sigsi;
 vector<vector<double>> sigso;
 sigt.push_back(1.0);
 siga.push_back(0.5);
 sigsi.push_back(0.5);
 sigso.push_back(sigsi);
 Material mat1 = Material(1, sigt, siga, sigso);
 
 
 //bounds
 Surf_ptr plane1 = plane1("plane1", 0.0, 0.0, 1.0, 0.0);
 Surf_ptr plane2 = make_shared<plane>("plane2", 0.0, 0.0, 1.0, 5.0);
 //vector<bool> inside1;
 //inside1.push_back(false); //false -> outside, true -> inside
 //inside1.push_back(true);
 //vector<Surf_ptr> cellSurfaces1;
 vector<pair<Surf_ptr, bool>> cellSurfaces1;
 cellSurfaces1.push_back(plane1);
 cellSurfaces1.push_back(plane2);
 
 //create cell 
 Cell_ptr cell1 = make_shared<Cell>(mat1, cellSurfaces1, inside1);
 
 //add to the geometry
 geometry.addMaterial(mat1);
 surfaces.push_back(plane1);
 surfaces.push_back(plane2);
 cells.push_back(cell1);
 }
*/
 
void Transport::runTransport()
{
    numHis = constants->getNumHis();
    double tally = 0;
    for(int i = 0; i < numHis ; i++)
    {
        //start a timer
        timer->startHist();

        //sample src 
        Part_ptr p_new = geometry->sampleSource();
        //Part_ptr p_new = make_shared<Particle>(point(0,0,0), point(0,0,1), 1);
        Cell_ptr startingCell = geometry->whereAmI(p_new->getPos());
        p_new->setCell(startingCell);
        pstack.push(p_new);
          
        //run history
        while(!pstack.empty())
        {
           Part_ptr p = pstack.top();
            while(p->isAlive())
            {
            //p->printState();
                Cell_ptr current_Cell = p->getCell();

                double d2s = current_Cell->distToSurface(p);
                double d2c = current_Cell->distToCollision(p);
            //cout << "d2s: " << d2s << "  d2c: " << d2c << endl;
                
                if(d2s > d2c) //collision!
                {
                    // score collision tally in current cell
                    timer->startTimer("scoring collision tally");
                    current_Cell->scoreTally(p , current_Cell->getMat()->getTotalXS( p->getGroup() ) ); 
                    tally++;
                    timer->endTimer("scoring collision tally");

                    timer->startTimer("scoring mesh tally");
                    //std::cout << "About to score mesh tally " << std::endl;
                    // score mesh tally
                    mesh->scoreTally( p , current_Cell->getMat()->getTotalXS( p->getGroup() ) );
                    //std::cout << "We scored that mesh tally! " << std::endl;
                    timer->endTimer("scoring mesh tally");

                    p->move(d2c);
                    current_Cell->processRxn(p, pstack);
                    p->kill(); //TODO: make this not awful
                }
                else //hit surface
                {

                    p->move(d2s + 0.00000001);
                    Cell_ptr newCell = geometry->whereAmI(p->getPos());
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
        //tell all estimators that the history has ended
         for( auto cell : geometry->getCells() ) {
        cell->endTallyHist();
         }

           // end histories in the mesh
           mesh->endTallyHist();

        // end the history timer
        timer->endHist();
    }

    cout << std::endl << "Transport finished!" << std::endl;
    cout << std::endl << "************************************************************************" << std::endl;
    cout << "************************************************************************" << std::endl;
    tally /= numHis;
    //cout << "tally " << tally << endl;
}

void Transport::output() {
    cout << std::endl << "Total Number of Histories: " << numHis << endl;

    int i = 0;
    for( Cell_ptr cell : geometry->getCells() ) {
        ++i;
        std::cout << "Collision tally in cell " << i << std::endl;
        for( int j = 1; j <= constants->getNumGroups(); ++j) {
            std::cout << " group: " << j << ", tally = " << cell->getSingleGroupTally(j, numHis).first 
                      << ", stddev = " << cell->getSingleGroupTally(j, numHis).second << std::endl;
        }
        cout << std::endl;
    }

    // print timing information
    timer->printAvgResults();

    // print mesh estimators to file
    mesh->printMeshTallies();
    if ( constants->getAllTets() ) {
        mesh->writeToVTK();
    }
}
