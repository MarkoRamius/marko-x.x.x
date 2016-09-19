/*---------------------------------------------------------------------------*\
    marko-x.x.x - my personal OpenFOAM shenanigans

    Copyright (C) 2016- Gerhard Holzinger
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Application
    randomParallelCommOpenFOAM_v01

Description
    Demonstrate random number generation and inter-process communication
    
    For some applications, it might be important for all parallel processes
    to use the same set of random numbers. Then the master process generates 
    the random numbers and communicates them to its slave processes.
    
    Run this application in parallel with or without the -comm option

\*---------------------------------------------------------------------------*/


#include "Istream.H"
#include "Random.H"
#include "OSspecific.H"
#include "clock.H"
#include "OStringStream.H"
#include "parRun.H"
#include "Time.H"
#include "argList.H"


using namespace Foam;

bool comm(false);

int main(int argc, char *argv[])
{
	// make communication between parallel processes optional
	argList::addBoolOption
	(
		"comm",
		"communicate random numbers among parallel processes"
	);
	
	
	#include "setRootCase.H"
    #include "createTime.H"
	
	
	Info << "Hello parallel communicating, random OpenFOAM!" << nl << endl;
	
	
	
    
    if (args.optionFound("comm"))
    {
    	comm = true;
    }

	
	// random stuff
	Random ranGen(clock::getTime()+pid());
	
	const label randListSize = 10;
	List<label> randList(randListSize);
	
	
	
	// ersatz run-time loop
	for (int i = 0; i < 10; i++)
	{
	    // master-only; generate new random numbers
	    if (not comm or Pstream::master())
        {
            for (int i = 0; i < randListSize; i++)
            {
                randList[i] = ranGen.integer(0, 10);
            }
        }
        
        // distribute random numbers
        if (comm)
        {
            Pstream::scatter(randList, Pstream::blocking);
        }
	    
	    
	    // print random numbers
	    OStringStream oStrStream;
		oStrStream << randList;
	    
	    std::cout << "randList (" << pid() << ") = " << oStrStream.str() << std::endl;
	    
	    
	    // waste some time for synchronisation purposes
	    system("sleep 1");
	    
	    
	    // print the current "time"
	    Info << nl << "Pseudo Time = " << i << nl << endl;
	}
	
	Info << endl;
	
	return 0;
}
