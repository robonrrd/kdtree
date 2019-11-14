// query_kdtree - Reads a serialized kdtree, the generating data set,
//                and a third file containing a set of query points; then, 
//                tests each kdtree query result against a brute-force
//                search (the ground truth) to determine if the kdtree is
//                operating correctly
//

#include <assert.h>
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

// Do a brute-force calculation of the closest point, as a ground
// truth for testing
int bruteForceClosest(const vector<vector<DATA_TYPE>>& data,
		      const vector<DATA_TYPE>& query);

// Read a list of well-formatted points from the input file
vector< vector<DATA_TYPE> > readPointsFromFile(const char* filename);

int
main(int argc, char *argv[])
{
   // Minimal sanity checking of user input
   if (argc != 4)
   {
      cout << "You must specify a serialized kdtree data file as "
	   << "the first argument, the original data set as the "
	   << "second argument, and a file containing query points "
	   << "as the third" << endl;
      exit(1);
   }
   
   // Deserialize the tree
   cout << "Deserizalizing " << argv[1] << endl;
   KDTree<DATA_TYPE> tree;
   ifstream infile(argv[1], ifstream::in);   
   if (infile.is_open())
   {
      infile >> tree;
      infile.close();
   }

   // Read the original point data (for use later for correctness checking)
   cout << "Reading original points from " << argv[2] << endl;
   vector< vector<DATA_TYPE> > originalPoints = readPointsFromFile(argv[2]);
   
   // Read the query data
   cout << "Reading query points from " << argv[3] << endl;
   vector< vector<DATA_TYPE> > queries = readPointsFromFile(argv[3]);

   // Create a results file
   string resultsFilename(argv[3]);
   resultsFilename += ".results";
   ofstream outfile(resultsFilename);
    
   for (const auto& query : queries)
   {
      IndexedPoint<DATA_TYPE> best = tree.nearestNeighbor(query);
      int bruteForceIndex = bruteForceClosest(originalPoints, query);

      // Check indices
      if (best.index != bruteForceIndex)
      {
	 cerr << "**ERROR** Result indices don't match" << endl;
	 outfile.close();
	 exit(1);
      }

      // ..then check the actual points, in case there was an order
      // change
      DATA_TYPE diff = 0;
      for (int ii=0; ii<query.size(); ++ii) // query.size is the
					    // dimension of all points
      {
	 diff += fabs(best.point[ii] - originalPoints[bruteForceIndex][ii]);
      }
      if (diff > 0)
      {
	 cout << "**ERROR**  Deserialized tree results don't match brute "
	      << "force results, with total L1 error " << diff << endl;
	 outfile.close();
	 exit(1);
      }
      outfile << best.index << endl;
   }
   outfile.close();
   cout << "Success!" << endl;
}

vector< vector<DATA_TYPE> > readPointsFromFile(const char* filename)
{
   ifstream datafile(filename, ifstream::in);

   // Read the first line to determine the dimensionality
   int dimension = 0;
   if (datafile.is_open())
   {
      string line;
      if (getline(datafile, line))
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
      cout << filename << " is improperly formatted or empty";
      exit(1);
   }

   // Rewind the file to the beginnning and parse
   datafile.seekg(0, datafile.beg);
   vector< vector<DATA_TYPE> > points;
   if (datafile.is_open())
   {
      string line;
      while (getline(datafile, line))
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
	 points.push_back(lineData);
      }
      datafile.close();
   }
   cout << "Read " << points.size() << " vectors of size " << dimension
	<< endl << endl;
   return points;
}

int bruteForceClosest(const vector<vector<DATA_TYPE>>& data,
		      const vector<DATA_TYPE>& query)
{
   const int dimension = query.size();
   
   vector<DATA_TYPE> best;
   int bestIndex = -1;
   DATA_TYPE bestDist = numeric_limits<DATA_TYPE>::max();
   for (int dd=0; dd<data.size(); ++dd)
   {
      DATA_TYPE dist = 0;
      for (int ii=0; ii<dimension; ++ii)
      {
	 DATA_TYPE linDiff = data[dd][ii] - query[ii];
	 dist += linDiff*linDiff;
      }

      if (dist < bestDist)
      {
	 bestDist = dist;
	 best = data[dd];
	 bestIndex = dd;
      }
   }

   return bestIndex;
}
