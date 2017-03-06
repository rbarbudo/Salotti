#ifndef SALOTTIOPTIMIZADO_H
#define SALOTTIOPTIMIZADO_H
#include <limits> //http://www.cplusplus.com/reference/limits/numeric_limits/
#include "algoritmo.hpp"


class MetodoSalottiOptimizado:public Algoritmo
{
	public:
		MetodoSalottiOptimizado(char * nombreFicheroContorno, int numeroPuntosInicial, int numeroPuntosFinal, 
														double limitePoda = numeric_limits<double>::max());
		MetodoSalottiOptimizado(const Contorno &c, int numeroPuntosInicial, int numeroPuntosFinal, 
														double limitePoda = numeric_limits<double>::max());
		
		~MetodoSalottiOptimizado();

		void aplicar(); //Método heredado que hay que implementar en esta clase
		char nombreFichero[100];
		
		vector <double> & vectorErrores()
		{
			return _error;
		};
		
		void vectorErrores(vector <double> & v)
		{
			_error = v;
		};
		
		vector <double> & vectorTiempos()
		{
			return _tiempo;
		};
		
		void vectorTiempos(vector <double> &v)
		{
			_tiempo = v;
		};
		
		vector <int> & vectorNumeroPuntosAproximacion()
		{
			return _numeroPuntosAproximacion;
		}
		
		void vectorNumeroPuntosAproximacion(vector <int> & v)
		{
			_numeroPuntosAproximacion = v;
		}
		
		vector <double> & vectorErroresMaximos()
		{
			return _errorMaximo;
		}
		
		void vectorErroresMaximos(vector <double> & v)
		{
			_errorMaximo = v;
		}
		
		vector <int> & vectorFrecuenciaDominantes()
		{
			return _frecuenciaDominantes;
		}
		
		void vectorFrecuenciaDominantes(vector <int> &v)
		{
			_frecuenciaDominantes = v;
		}
		
		Matriz <int> & matrizContornosOptimos()
		{
			return _contornosOptimos;
		}
		
		void matrizContornosOptimos(Matriz <int> & m)
		{
			_contornosOptimos = m;
		}
		
		int & numeroPuntosInicial()
		{
			return _inicial;
		}
		
		void numeroPuntosInicial(int & inicial)
		{
			_inicial = inicial;
		}
		
		int & numeroPuntosFinal()
		{
			return _final;
		}

		void numeroPuntosFinal(int & fin)
		{
			_final = fin;
		}
		vector <double> & limitePoda()
		{
			return _limitePoda;
		}
		
		void limitePoda(vector <double> &limitePoda)
		{
			_limitePoda = limitePoda;
		}
		
	private:
		int _inicial; //Numero de puntos de la aproximación de menos puntos a calcular
		int _final; //Numero de puntos de la aproximacion de mas puntos a calcular.
		vector <double> _error; //Vector que almacenará el error óptimo para cada aproximacion poligonal
		vector <double> _limitePoda; //Vector que guarda el limite de poda para un numero de puntos determinado.
		vector <double> _tiempo;
		vector <int> _numeroPuntosAproximacion; //Vector que guarda el numero de puntos de cada aproximación.
		vector <double> _errorMaximo; //Vector que guarda el error maximo de cada aproximación.
		vector <int> _frecuenciaDominantes; //Vector que contabiliza el número de veces que un punto es dominante.
		Matriz <int> _contornosOptimos; //Matriz que almacenará el contorno óptimo para cada aproximación poligonal
		void calcularSumatorios(Matriz <double> &suma_x, Matriz <double> &suma_y, Matriz <double> &suma_xx, Matriz <double> &suma_yy, Matriz <double> &suma_xy);
		double sumaErrores(const int &inicio, const int &fin, const Matriz <double> &suma_x, 
const Matriz <double> &suma_y, const Matriz <double> &suma_xx, const Matriz <double> &suma_yy, 
const Matriz <double> &suma_xy); //Errores al aproximar por segmento inicio fin
		double sumaErroresMinima(const int &inicio, const int &fin, const Matriz <double> &suma_x, 
const Matriz <double> &suma_y, const Matriz <double> &suma_xx, const Matriz <double> &suma_yy, 
const Matriz <double> &suma_xy); //Errores al ajustar por el eje de minima inercia.
		double sumaErrores1(const int &inicio, const int &fin, const Matriz <double> &suma_x, 
const Matriz <double> &suma_y, const Matriz <double> &suma_xx, const Matriz <double> &suma_yy, 
const Matriz <double> &suma_xy); //Errores en la regresion de y sobre x
		double sumaErrores2(const int &inicio, const int &fin, const Matriz <double> &suma_x, 
const Matriz <double> &suma_y, const Matriz <double> &suma_xx, const Matriz <double> &suma_yy, 
const Matriz <double> &suma_xy); //Errores en la regresion de x sobre y
		void obtenerPuntosContornoOptimo(Contorno &contorno, int ** padre, const int &numeroPuntosProbados);
		

};
		

#endif
