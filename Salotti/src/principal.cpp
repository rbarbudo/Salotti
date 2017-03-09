#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <limits>
#include <sys/time.h>

#include "algoritmo.hpp"
#include "metodoSalottiOptimizado.hpp"
//#include "operaciones.hpp"
#include "contorno.hpp"
#include "matriz.hpp"
#include "operacionescadenas.hpp"

void metodoSalottiOptimizado();

int main(int argc, char *argv[])
{
	metodoSalottiOptimizado();
	cout << "termine metodo SalottiOptimizado" << endl;
	return 0;
}

void metodoSalottiOptimizado()
{
	// Carga de contorno

	//char nombreFicheroContorno[100];
	char nombreFicheroContorno[100] = "contornos/prueba.txt";
	//cout << "Introduzca el fichero de contorno:";
	//cin >> nombreFicheroContorno;

	Contorno original(nombreFicheroContorno);

	// Informacion del contorno
	int numeroPuntosContorno = original.numeroPuntosContorno();
	cout << "Numero de puntos del contorno : " << numeroPuntosContorno << endl;
	int numeroBreakPoints = original.contarBreakPoints();
	cout << "Numero de breakpoints = " << numeroBreakPoints << endl;

	// Numero de puntos de la aproximacion

	//int numPuntosAprox;
	int numPuntosAprox = 4;
	//cout << "Número de puntos de la aproximacion contando dos veces el inicial:";
	//cin >> numPuntosAprox;

	// Datos usados para controlar la ejecucion
	vector <double> limitePoda;
	vector <int> inicialesOptimos;

	//Primera pasada usando como inicial el punto 1, para inicializar los óptimos
	MetodoSalottiOptimizado *p;
	Contorno actual(original);
	p = new MetodoSalottiOptimizado(actual, numPuntosAprox, numPuntosAprox);
	p->aplicar();

	//Vector para guardar el error optimo del contorno óptimo definitivo
	//se inicializa con los errores obtenidos al tomar el punto 1 como inicial
	vector <double> erroresOptimos = p->vectorErrores();
	//Matriz para guardar los contornos optimos definitivos
	Matriz <int> contornosOptimos = p->matrizContornosOptimos();


	// Mostramos los puntos de la solucion
	cout << "Solucion: ";
	for(int i=1; i<=numPuntosAprox; i++) {
		cout << contornosOptimos.elemento(1,i) << " ";
	}
	cout << endl;

	//Vector para guardar el numero de puntos de cada aproximación.
	vector <int> numeroPuntosAproximacion = p->vectorNumeroPuntosAproximacion();

	//Inicializamos el vector de iniciales optimos con el punto 1
	for(unsigned int j = 0; j < erroresOptimos.size(); j++)
		inicialesOptimos.push_back(1);

	//Liberamos el contorno actual
	actual.liberarContorno();

	//Definimos el vector de errores parcial para cada punto de comienzo
	vector <double> errores = p->vectorErrores();

	//Guardamos los limites de poda, que servirán para el siguiente punto de comienzo
	limitePoda = p->vectorErrores();

	//Definimos la matriz de contornos optimos para cada punto de comienzo
	Matriz <int> contornos = p->matrizContornosOptimos();

	//Vector para ver la frecuencia con que se repite la solucion optima
	//El indice coincide con el punto considerado. por ejemplo frecuenciaOptimo[7] = 5 indica que la solución optima para 
	//7 puntos se repite 5 veces (hay cinco posibles puntos de comienzo con error optimo).

	//int frecuenciaOptimo[numPuntosAprox +1];

	//Comprobaciones
	cout << "Punto inicial =  1 ->";
	for (int i = numPuntosAprox; i <= numPuntosAprox; i++)
		cout << " error = " << errores[i - numPuntosAprox] << " ";
	cout << "\n";
/*
	//Inicializamos a 0 la frecuencia con la que aparece cada solución óptima
	for (int i = 0; i <= numPuntosAprox; i++)
		frecuenciaOptimo[i] = 0;

	//Realizamos las siguientes pasadas cambiando el punto inicial
	for(int i = 2; i < numeroPuntosContorno ; i++) //Considera todos los puntos como iniciales.
	{
		cout << "Punto inicial =  " << i << " -> " << "Limite de poda = "<< limitePoda[0] << " ";
		//Creamos el puntero a clase base Algoritmo
		MetodoSalottiOptimizado *a;

		//Definimos el contorno a usar
		Contorno actual(original);

		//Cambiamos el punto inicial
		actual.cambiarPuntoInicial(original, i);

		//Aplicamos método
		a = new MetodoSalottiOptimizado(actual, numPuntosAprox, numPuntosAprox);

		//Introducimos los nuevos límites de poda
		a->limitePoda(limitePoda);

		//Aplicamos el método
		a->aplicar();

		//Guardamos el vector de errores
		//contiene los errores optimos de todas las aproximaciones desde minimo a maximo que comienzan en el punto i
		errores = a->vectorErrores();

		//Comprobaciones

		for (int k = numPuntosAprox; k <= numPuntosAprox; k++)
			cout <<" Error = " << errores[k - numPuntosAprox] << " ";

		cout << "\n";

		//Guardamos la matriz de contornos optimos

		//Matriz para guardar los contornos optimos definitivos
		Matriz <int> contornosOptimos = a->matrizContornosOptimos();

		//OJO hay que tener en cuenta que el punto inicial ya no es el 1 y por tanto en la matriz de contornos hay un desplazamiento,
		//debido al punto inicial


		//Actualización de los óptimos.
		for(unsigned int j = 0; j < errores.size(); j++) 
		{
			if (errores[j] < erroresOptimos[j]) 
				//El optimo anterior se ha mejorado
			{
				//Actualizamos el error optimo
				erroresOptimos[j] = errores[j];

				//Actualizamos la frecuencia
				frecuenciaOptimo[j + numPuntosAprox] = 1;

				//Actualizamos el nuevo punto inicial optimo
				inicialesOptimos[j] = i;


				//Actualizamos el nuevo contorno optimo teniendo en cuenta que se ha desplazado el punto inicial
				//actualizarContornoOptimoPerezVidalOptimizado(contornos, contornosOptimos, numeroPuntosContorno, i, numeroPuntosAproximacion[j] );
			}

			if (fabs(errores[j] - erroresOptimos[j]) <= 0.01) //Cuenta los puntos de comienzo que tienen el mismo error
				frecuenciaOptimo[j + numPuntosAprox]++;
		}
		//Actualizamos los nuevos limites de poda, por si han cambiado, para el siguiente punto de comienzo.
		limitePoda = a->limitePoda();
		delete a;
		actual.liberarContorno();
	}


	for(unsigned int i = 0; i < numeroPuntosAproximacion.size(); i++) 
	{
		cout << "La solucion optima para " << numeroPuntosAproximacion[i] << " puntos se repite " << frecuenciaOptimo[i+numPuntosAprox];
		cout << " veces " << "p. inicial óptimo es " << inicialesOptimos[i] << ". Error optimo = " << erroresOptimos[i] << endl;
	}
	*/
}
