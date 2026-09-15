#include <iostream>
#include <iomanip> // para usa setprecision
using namespace std;

int main(){


double salario;
cout << "Inserte salario (usa punto, ej. 1175.25): ";
cin >> salario;

double A = salario * 0.50;
double B = salario * 0.30;
double C = salario * 0.20;

cout << fixed << setprecision(2); // para que los tres valores salgan siempre con 4 decimales
cout << "Así quedaría la regla: " 
     << "Necesidades (50%): " << A << endl 
     << "Ocio (30%): " << B << endl 
     << "Ahorro e invesión (20%): " << C << endl;

}