// build_kdtree - From a comma-separated-value text file containing a
//                list of consistently dimensioned points, build a
//                KD-tree and serialze it to disk.
//
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "kdtree.h"

using namespace std;

// Our KD tree code is templated on dimension as well as
// data-type. For the sake of this example, we are using
// double-precision floating point numbers
typedef double DATA_TYPE;

int
main(int argc, char *argv[])
{
   // Minimal sanity checking of user input
   if (argc != 2)
   {
      cout << "You must specify a data set as the first argument" << endl;
      exit(1);
   }
   else
   {
      cout << "Reading data from " << argv[1] << endl;
   }
   
   // Read the first line to determine the dimensionality
   ifstream dataFile(argv[1], ifstream::in);
   int dimension = 0;
   if (dataFile.is_open())
   {
      string line;
      if (getline(dataFile, line))
      {
	 stringstream ss(line);
	 DATA_TYPE val;
	 while (ss >> val)
	 {
	    dimension++;
	    if (ss.peek() == ',')
	       ss.ignore();
	 }
      }
   }

   if (dimension < 1)
   {
      cout << argv[1] << " is improperly formatted or empty";
      exit(1);
   }

   // Rewind the file to the beginnning and parse
   dataFile.seekg (0, dataFile.beg);
   vector< vector<DATA_TYPE> > data;
   if (dataFile.is_open())
   {
      string line;
      while (getline(dataFile, line))
      {
	 stringstream ss(line);
	 vector<DATA_TYPE> lineData;
	 DATA_TYPE val;
	 while (ss >> val)
	 {
	    lineData.push_back(val);
	    if (ss.peek() == ',')
	       ss.ignore();
	 }
	 data.push_back(lineData);
      }
      dataFile.close();
   }
   
   cout << "Read " << data.size() << " vectors of size " << dimension
	<< endl;

   KDTree<DATA_TYPE> tree;
   if (!tree.build(data))
   {
      cerr << "Failed to successfully build the KD tree" << endl;
      exit(1);
   }

   // Serialize the tree out to disk
   string serializedFilename(argv[1]);
   serializedFilename += ".kdtree";
   ofstream outfile (serializedFilename);

   cout << "Serializing KD tree to " << serializedFilename << endl;
   outfile << tree;
   outfile.close();
}
