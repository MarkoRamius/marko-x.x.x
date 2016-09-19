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
    randomOpenFOAM_v01

Description
    Demonstrate random number generation with respect to seeding values

\*---------------------------------------------------------------------------*/


#include "Istream.H"
#include "Random.H"
#include "OSspecific.H"
#include "clock.H"


using namespace Foam;
int main(int argc, char *argv[])
{
	Info << "Hello random OpenFOAM!" << nl << endl;
	
	/* Below are three variants on how to initialize the random number generator. */
	
	/*
	    bad! when using default seed value as in this case
	*/
	//Random ranGen(osRandomInteger());
	
	/*
	    better! use time as seed value, as time is always different
	*/
	//Random ranGen(clock::getTime());
	
	/*
	    excellent! this provides a unique seed value 
	        even in parallel processing, when multiple processes 
	        are started at the same time
	*/
	Random ranGen(clock::getTime()+pid());
	
	label randomNumber = ranGen.integer(0, 10);
	
	for (int i = 0; i < 10; i++)
	{
	    Info << "random number = " << randomNumber << endl;
	    
	    randomNumber = ranGen.integer(0, 10);
	}
	
	Info << endl;
	
	return 0;
}
