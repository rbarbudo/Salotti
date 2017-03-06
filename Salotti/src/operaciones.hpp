#ifndef OPERACIONES_H
#define OPERACIONES_H
#include "contorno.hpp"
#include "recta.hpp"
//#include "heapvectorMasood.hpp"

void operacionesContornos();
void rotarContorno(Contorno &contorno, const double &angulo);
void escalarContorno(Contorno &contorno, const double &escala);
void escalarDiscretaContorno(Contorno &contorno, const double &escala);
void discretizarContorno(Contorno &contorno);
void conectividad4(const Contorno &contorno, Contorno &contornoNuevo);
void escalarEnteraContorno(Contorno &contornoNuevo, const Contorno &contorno, const int &escala);
int calcularPuntoInicialContorno(const Contorno &contorno);
void cascoPoligonalConvexo(const Contorno &contorno, Contorno &casco);
int estimarPuntoInicialCascoPoligonal(const Contorno &contorno);
bool comprobarPuntos(const Contorno &contorno, const Recta &r);
void filtroGausiano(const Contorno &contornoOriginal, Contorno &contornoResultado, const double &sigma);
void filtroGausianoPrimeraDerivada(const Contorno &contornoOriginal, Contorno &contornoResultado, const double &sigma);
void filtroGausianoSegundaDerivada(const Contorno &contornoOriginal, Contorno &contornoResultado, const double &sigma);
void generarGausiana(double *gausiana, const int &anchoGausiana, const double &sigma, double &sumaGausiana);
void generarGausianaPrimeraDerivada(double *gausiana, const int &anchoGausiana, const double &sigma, double &sumaGausiana);
void generarGausianaSegundaDerivada(double *gausiana, const int &anchoGausiana, const double &sigma, double &sumaGausiana);
void convolucionarGausiana(const Contorno &contornoOriginal, Contorno &contornoResultado, double *gausiana,
const int &anchoGausiana, const double &sumaGausiana, const int &numeroPuntosContorno);
void ruidoAleatorioContorno(const Contorno &contornoOriginal, Contorno &contornoResultado, const double &sigma);
void filtroMediana(const Contorno &contornoOriginal, Contorno &contornoResultado, const int &longitud);
void filtroMedia(const Contorno &contornoOriginal, Contorno &contornoResultado, const int &longitud);
void obtenerVectoresPuntosVecinos(const Contorno &contorno, double *xVector, double *yVector, const int &i, const int &longitud);
void ordenarVector(double *vector, const int &numero);
double mediaVector(double *vector, const int &longitud);
void eliminarAlineados(Contorno &contorno, int &numeroPuntosDominantes, const int &inicial);
void eliminarCasiAlineados(Contorno &contorno, int &numeroPuntosDominantes, const double &distancia, const int &inicial);
int eliminarCasiAlineadoMasood(Contorno &contorno, vector <double> &aev, vector <int> &ci, int numeroPuntosDominantes);
//int eliminarCasiAlineadoMasoodOld(Contorno &contorno, HeapVectorMasood &heap, int * & posicionNodo, vector <double> &aev, vector <int> &ci, int numeroPuntosDominantes)
;
int eliminarCasiAlineadoMasoodOptimizado(Contorno &contorno, vector <double> &aev, vector <int> &ci, vector <int> &of, int numeroPuntosDominantes);
void duplicarContorno(const Contorno &original, Contorno &duplicado);
void obtenerContornoBreakpoints(Contorno contorno, char *fichero);
bool comprobarContornoCuadrilatero(Contorno &contornoNuevo, const double &umbral);

#endif
