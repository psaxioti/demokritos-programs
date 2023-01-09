#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>

using namespace std;
int main ()
{	
	string csfile;
	string ratesfile; 
	cout<<"input cross section file (MeV-lab,mbarn)"<<endl;
	getline (cin, csfile);
	cout<<"output filename"<<endl;
	getline (cin, ratesfile);
	double ap,at;
	cout<<"A_(in amu) of projectile"<<endl;
	cin>>ap;
	cout<<"A_(in amu) of target"<<endl;
	cin>>at;
	
	ifstream infile;
	ofstream outfile;
	infile.open (csfile.c_str());
	outfile.open (ratesfile.c_str());

	double a,b,fact,integr;
	double ared,acomp,acomp1;
	double ener[1000];
	double xs[1000];
	int sum;
	sum=0;
	acomp=ap+at;
	acomp1=ap*at;
	ared=acomp1/acomp;
	double t9[58] =  {0.10,0.15,0.20,0.25,0.30,0.40,0.50,0.60,0.70,0.80,0.90,1.00,1.10,1.20,1.30,1.40,1.50,1.60,1.70,1.80,1.90,2.00,2.10,2.20,2.30,2.40,2.50,2.60,2.70,2.80,2.90,3.00,3.10,3.20,3.30,3.40,3.50,3.60,3.70,3.80,3.90,4.00,4.10,4.20,4.30,4.40,4.50,4.60,4.70,4.80,4.90,5.00,5.50,6.00,7.00,8.00,9.00,10.0};

	
        while(!infile.eof()) 
		{
		infile>>a>>b;
			ener[sum]=(at/(ap+at))*a;
			xs[sum]=b/1000;
			sum++;
		}
	int summ;
	summ=sum-1;		
	double y[summ];

	infile.close();
		for(int k=0;k<58;k++){
		integr=0;
		fact=3.7335e+10/(sqrt(t9[k]*t9[k]*t9[k]*ared));				
		for(int i=0;i<summ;i++){
		y[i]=xs[i]*ener[i]*exp(-11.6050*ener[i]/t9[k]);
   	        }
		for(int i=1;i<summ;i++){
		integr=(y[i]+y[i-1])*0.50*(ener[i]-ener[i-1])+integr;
		}
		outfile<<t9[k]<<"	"<<integr*fact<<endl;
		}

 outfile.close();
}
