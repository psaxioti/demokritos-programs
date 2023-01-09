#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <vector>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>

using namespace std;

   int main (int argc, char *argsv[]){
   string csfile=argsv[1], experfile=argsv[2], line;
   double c, d, dum,chi=0,logchi=0,logchi_sot=0,chi_sot=0;
   double mass=atof(argsv[3]);
   double proj;
   vector<double> enerexp, csexp,ener,xs;
   bool flag=false;
   
   switch(*argsv[4]){
      case 'p':
         proj=1.007825;
         break;
      case 'n':
         proj=1.008665;
         break;
      case 'a':
         proj=4.002603;
         break;
   }
      
   ifstream infile;
   infile.open (csfile.c_str());
   while(infile.peek()=='#') infile.ignore(1000,'\n');
   while(infile>>c>>d){
      ener.push_back(mass*c/(mass+proj));
      xs.push_back(d);
   }
   infile.close();

   ifstream expfile;
   expfile.open (experfile.c_str());
   
   vector < vector < double > > expdata ;
   while(getline(expfile,line)){
      vector < double > data;
      double value;
      istringstream iss(line);
      while (iss >> value)
      {
         data.push_back(value);
      }
      expdata.push_back(data);
   }
   for ( vector < vector < double > > :: size_type i = 0, size=expdata.size(); i < size; ++i){
      if(expdata[i][0]>=ener[0] && expdata[i][0]<=ener[ener.size()-1]){
         enerexp.push_back(expdata[i][0]);
         csexp.push_back(expdata[i][1]);
      }
      else flag=true;
   }
   expdata.clear();
   
/*   while(expfile>>c>>d){
      cout << c << "   " << ener[0] << "   "<< ener[ener.size()-1]<<"  "<< (c>=ener[0] && c<=ener[ener.size()-1])<<endl;
      if(c>=ener[0] && c<=ener[ener.size()-1]){
         enerexp.push_back(c);
         csexp.push_back(d);
      }
      else flag=true;
   }
*/   expfile.close();	

   gsl_interp_accel *acc = gsl_interp_accel_alloc ();
   gsl_spline *spline = gsl_spline_alloc (gsl_interp_cspline, ener.size());
   gsl_spline_init (spline, &ener[0], &xs[0], ener.size());

   for (int lala=0; lala < enerexp.size(); lala++){
         double logexp,logtal;
         logexp=abs(log10(csexp[lala]));
         logtal=abs(log10(gsl_spline_eval (spline, enerexp[lala], acc)));
	 logchi=logchi+pow(logexp-logtal,2);
	 chi=chi+pow(gsl_spline_eval (spline, enerexp[lala], acc)-csexp[lala],2);
	 logchi_sot=logchi+(pow(logexp-logtal,2)/logtal);
	 chi_sot=chi_sot+(pow(gsl_spline_eval (spline, enerexp[lala], acc)-csexp[lala],2)/gsl_spline_eval (spline, enerexp[lala], acc));
   }
   if(flag)cout << logchi/enerexp.size()<<"	"<<chi/enerexp.size()<<" "<<logchi_sot<<" "<<logchi_sot/enerexp.size()<<" "<< chi_sot<<" "<<chi_sot/enerexp.size()<<" Caution!!!!! not all experimental data points were processed!!!!!!!! " <<  argsv[1] <<endl;
   else cout << logchi/enerexp.size()<<"	"<<chi/enerexp.size()<<" "<<logchi_sot<<" "<<logchi_sot/enerexp.size()<<" "<< chi_sot<<" "<<chi_sot/enerexp.size()<< "	" <<  argsv[1]  <<endl;
		
   gsl_spline_free (spline);
   gsl_interp_accel_free (acc);

   return 0;
}
