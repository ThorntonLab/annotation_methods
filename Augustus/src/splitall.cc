#include <Sequence/Fasta.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <sstream>

using namespace std;
using namespace Sequence;

int main(int argc, char **argv)
{
  int argn=1;

  Fasta f;
  unsigned i=1;
  ostringstream rest;
  while(!cin.eof())
    {
      cin>>f>>ws;
      if ( f.length() > 1000000 )
	{
	  ostringstream os;
	  os << "chrom" << i++ << ".fasta";
	  ofstream o(os.str().c_str());
	  o << f << '\n';
	  o.close();
	}
      else
	{
	  rest << f << '\n';
	}
    }
  if( ! rest.str().empty() )
    {
      ofstream o("therest.fasta");
      o << rest.str();
      o.close();
    }
}
