#include <iostream>
#include <fstream>
//#include <sstream>
#include <string>
#include <cmath>
#include <vector>
//#include <gsl/gsl_errno.h>
//#include <gsl/gsl_spline.h>

using namespace std;

double Ecm(double,double,double);
double F(int,int,double,double);

int main (int argc, char *argsv[]){
   string outfilename;
   int Z1,Z2;
   double M1,M2,theta,Emin,Emax,Estep,c;
   double sr,s;
   vector<double> ener;
   ofstream outfile;
// Energies in keV, lab   
   cout<<"Give Z1 and Z2"<<endl;
   cin>>Z1>>Z2;
   cout<<"Give M1 and M2"<<endl;
   cin>>M1>>M2;
   cout<<"Give angle"<<endl;
   cin>>theta;
   theta=theta*3.14159/180.;
   if(argc <= 1){
      cout<<"Give Emin, Emax and Estep"<<endl;
      cin>>Emin>>Emax>>Estep;
   }
   else {
      string energies=argsv[1];
      ifstream infile;
      infile.open (energies.c_str());
      while(infile>>c){
         ener.push_back(c);
      }
      infile.close();
   }
   cout<<"Give name for output file"<<endl;
   cin>>outfilename;
   
   outfile.open(outfilename.c_str());
   if(argc <= 1){
      for (double Ecur=Emin ; Ecur<=Emax ; Ecur=Ecur+Estep){
         sr=5.18275*pow(10.,6.)*pow(Z1*Z2/Ecur,2.);
         sr=sr*pow(sqrt((M2*M2)-(M1*M1*sin(theta)*sin(theta)))+M2*cos(theta),2.);
         sr=sr/(M2*pow(sin(theta),4.)*sqrt((M2*M2)-(M1*M1*sin(theta)*sin(theta))));
         s=sr*F(Z1,Z2,Ecm(Ecur,M1,M2),theta);
         outfile<<Ecur<<" "<<s<<endl;
      }
   }
   else {
      int j=0;
      for (double j=0 ; j<ener.size() ; j++){
         double Ecur=ener[j];     
         sr=5.18275*pow(10.,6.)*pow(Z1*Z2/Ecur,2.);
         sr=sr*pow(sqrt((M2*M2)-(M1*M1*sin(theta)*sin(theta)))+M2*cos(theta),2.);
         sr=sr/(M2*pow(sin(theta),4.)*sqrt((M2*M2)-(M1*M1*sin(theta)*sin(theta))));
         s=sr*F(Z1,Z2,Ecm(Ecur,M1,M2),theta);
         outfile<<Ecur<<" "<<s<<endl;
      }
   }
   outfile.close();

   return 0;
}

double Ecm(double E, double M1, double M2){
   return E*M2/(M1+M2);
}

double F(int Z1, int Z2, double E, double theta){
   double result;
   
   if(theta>90.*3.14159/180.){
      result=0.049*Z1*pow(Z2,4./3.);
      result=result/E;
      result=1.-result;
   }
   else if(theta<=90.*3.14159/180.){
      double v1;
      v1=0.04873*Z1*Z2*sqrt(pow(Z1,2./3.)+pow(Z2,2./3.));
      result=pow(1.+(0.5*v1/E),2.);
      result=result/pow(1.+(v1/E)+pow(v1/(2*E),2.),2.);        // Note there is a theta_cm missing!!!!!!!!!!!!!!!!
//      result=result/pow(1.+(v1/E)+pow(v1/(2*E*theta_cm),2.),2.)         Note there is a theta_cm missing!!!!!!!!!!!!!!!!
   }
   return result;
}
