#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <vector>


using namespace std;

   int main (int argc, char *argsv[]){
   string metafile=argsv[1], totalfile=argsv[2];
   double c, d, e;
//   double logos;
   vector<double> ener1, cs1, ener2, cs2;
   string line;
   double value;

    
   ifstream mfile;
   mfile.open (metafile.c_str());
   while(mfile.peek()=='#') mfile.ignore(1000,'\n');
   while(getline(mfile, line)){
      istringstream iss(line);
      for (int i=0 ; i<100 ; i++){
         if(iss>>value){
            if(i==0) ener1.push_back(value);
            if(i==1) cs1.push_back(value);
         }
//         else break;
      }
   }
//   while(mfile>>c>>d>>e){
//      ener1.push_back(c);
//      cs1.push_back(d);
//   }
   mfile.close();

   ifstream tfile;
   tfile.open (totalfile.c_str());
   while(tfile.peek()=='#') tfile.ignore(1000,'\n');
   while(getline(tfile, line)){
      istringstream iss(line);
      for (int i=0 ; i<100 ; i++){
         if(iss>>value){
            if(i==0) ener2.push_back(value);
            if(i==1) cs2.push_back(value);
         }
//         else break;
      }
   }
//   while(tfile>>c>>d>>e){
//      ener2.push_back(c);
//      cs2.push_back(d);
//   }
   tfile.close();	

//cout << ener1.size() <<endl;
   for (int lala=0; lala < ener1.size(); lala++){
//	 logos=enerexp[lala]/csexp[lala];
 cout << " " << ener1[lala] << " " << cs1[lala]/cs2[lala] << endl;
// cout << "eimai edw !!!!" <<endl;
   }
// cout << logos <<endl;
   return 0;
}
