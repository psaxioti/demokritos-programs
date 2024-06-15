#include <cmath>
#include <fstream>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_sf_erf.h>
#include <gsl/gsl_spline.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

void exitCode(const int ErrorCode);
void printUsage();

int main(const int argc, const char *argsv[]) {
   if (argc != 3 && argc != 5)
      exitCode(1);

   ifstream TalysCSfile(argsv[1]);
   if (!TalysCSfile.good())
      exitCode(2);

   ifstream ExperimentalCSfile(argsv[2]);
   if (!ExperimentalCSfile.good())
      exitCode(3);

   bool cmFlag = false;
   double TargetMass = 0;
   double ProjectileMass = 0;
   if (argc == 5) {
      TargetMass = atof(argsv[3]);
      switch (*argsv[4]) {
         case 'p':
            ProjectileMass = 1.007825;
            break;
         case 'n':
            ProjectileMass = 1.008665;
            break;
         case 'a':
            ProjectileMass = 4.002603;
            break;
         default:
            exitCode(4);
            break;
      }
      cmFlag = true;
   }

   vector<double> TalysEn, TalysCS;
   TalysEn.clear();
   TalysCS.clear();

   while (TalysCSfile.peek() == '#')
      TalysCSfile.ignore(1000, '\n');
   vector<vector<double>> Talysdata;
   string line;
   while (getline(TalysCSfile, line)) {
      vector<double> data;
      double value;
      istringstream iss(line);
      while (iss >> value) {
         data.push_back(value);
      }
      Talysdata.push_back(data);
   }
   double Factor = 1.;
   if (cmFlag)
      Factor = TargetMass / (TargetMass + ProjectileMass);
   for (vector<vector<double>>::size_type i = 0, size = Talysdata.size(); i < size; ++i) {
      TalysEn.push_back(Factor * Talysdata[i][0]);
      TalysCS.push_back(Talysdata[i][1]);
   }
   Talysdata.clear();
   TalysCSfile.close();

   vector<double> ExperimentalEn, ExperimentalCS, ExperimentalCSError;
   bool ExperimentaPointOutsideTalysFlag = false;
   ExperimentalEn.clear();
   ExperimentalCS.clear();
   ExperimentalCSError.clear();

   while (ExperimentalCSfile.peek() == '#')
      ExperimentalCSfile.ignore(1000, '\n');
   vector<vector<double>> expdata;
   while (getline(ExperimentalCSfile, line)) {
      vector<double> data;
      double value;
      istringstream iss(line);
      while (iss >> value) {
         data.push_back(value);
      }
      expdata.push_back(data);
   }
   for (vector<vector<double>>::size_type i = 0, size = expdata.size(); i < size; ++i) {
      if (expdata[i][0] >= TalysEn[0] && expdata[i][0] <= TalysEn[TalysEn.size() - 1]) {
         ExperimentalEn.push_back(expdata[i][0]);
         ExperimentalCS.push_back(expdata[i][1]);
         if (expdata[i].size() >= 3 && expdata[i][2] > 0)
            ExperimentalCSError.push_back(expdata[i][2]);
         else
            ExperimentalCSError.push_back(expdata[i][1]);

      } else
         ExperimentaPointOutsideTalysFlag = true;
   }
   expdata.clear();
   ExperimentalCSfile.clear();

   gsl_interp_accel *acc = gsl_interp_accel_alloc();
   gsl_spline *spline = gsl_spline_alloc(gsl_interp_linear, TalysEn.size());
   gsl_spline_init(spline, &TalysEn[0], &TalysCS[0], TalysEn.size());

   double ChiSquare = 0.;
   double FrmsExponential = 0.;
   for (size_t i = 0; i < ExperimentalEn.size(); ++i) {
      double CurrentEn = ExperimentalEn[i];
      double CurrentExperimentalCS = ExperimentalCS[i];
      double CurrentExperimentalCSError = ExperimentalCSError[i];
      double TalysCSInterpolated = gsl_spline_eval(spline, CurrentEn, acc);

      double Distance = TalysCSInterpolated - CurrentExperimentalCS;
      double NormalizedDistance = Distance / CurrentExperimentalCSError;
      double CoverE = TalysCSInterpolated / CurrentExperimentalCS;

      double r = CoverE;
      if (CurrentExperimentalCSError != CurrentExperimentalCS) {
         double x = abs(NormalizedDistance) / sqrt(2);
         r = 1 + ((CoverE - 1) * gsl_sf_erf(x));
      }

      ChiSquare += pow(NormalizedDistance, 2);

      FrmsExponential += pow(log(r), 2);
   }
   ChiSquare /= ExperimentalEn.size();
   FrmsExponential = exp(sqrt(FrmsExponential / ExperimentalEn.size()));

   cout << setw(12) << left << setprecision(8) << ChiSquare;
   cout << setw(12) << left << setprecision(8) << FrmsExponential;
   if (ExperimentaPointOutsideTalysFlag)
      cout << " Caution!!!!! not all experimental data points were processed!!!!!!!!";
   cout << " " << argsv[1] << endl;

   gsl_spline_free(spline);
   gsl_interp_accel_free(acc);

   return 0;
}

void exitCode(const int ErrorCode) {
   switch (ErrorCode) {
      case 1:
         printUsage();
         break;
      case 2:
         cerr << "Talys cs file cannot be opened!" << endl;
         cerr << "Check file provided and try again!" << endl;
         break;
      case 3:
         cerr << "Experimental cs file cannot be opened!" << endl;
         cerr << "Check file provided and try again!" << endl;
         break;
      case 4:
         cerr << "Unknown projectile symbol! Has to be one of p, a or n" << endl;
         break;
   }
   exit(ErrorCode);
}

void printUsage() {
   cerr << "Something wrong with command arguments!!!" << endl;
   cerr << "Exiting!!!!!!!!!" << endl;
}
