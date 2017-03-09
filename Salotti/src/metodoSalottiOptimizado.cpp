#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <math.h>
#include <sys/time.h> //para la función gettimeofday
#include <vector>    // necesario para el contenedor vector
#include <list>
#include <queue>
#include <algorithm>
#include "algoritmo.hpp"
#include "metodoSalottiOptimizado.hpp"
#include "nodo.hpp"
#include "matriz.hpp"
#include "operaciones.hpp"
#include "punto.hpp"
#include "recta.hpp"
#include "operacionescadenas.hpp"
#include "heapvector.hpp"
#include "operadornodoheap.hpp"

using namespace std;

/* FUNCIONES DE INTERFAZ PARA EL TIPO MetodoSalottiOptimizado */

//Constructor.

MetodoSalottiOptimizado::MetodoSalottiOptimizado(char *nombreFicheroContorno, int numeroPuntosInicial, int numeroPuntosFinal, 
		double limitePoda)
{	
	//Creamos un contorno temporal donde se carga el contorno
	Contorno c(nombreFicheroContorno);

	//Lo copiamos en el contorno privado.
	contorno(c);

	//Introducimos el nombre del fichero en la variable 
	strcpy(nombreFichero, nombreFicheroContorno);
	_inicial = numeroPuntosInicial;
	_final = numeroPuntosFinal;

	//Inicializamos los limites de poda a infinito
	for(int i = _inicial; i <= _final; i++)
		_limitePoda.push_back(limitePoda);
}

MetodoSalottiOptimizado::MetodoSalottiOptimizado(const Contorno &c, int numeroPuntosInicial, int numeroPuntosFinal, 
		double limitePoda)
{
	//Lo copiamos en el contorno privado.
	contorno(c);
	_inicial = numeroPuntosInicial;
	_final = numeroPuntosFinal;

	//Inicializamos los limites de poda a infinito
	for(int i = _inicial; i <= _final; i++)
		_limitePoda.push_back(limitePoda);
}

//Destructor
MetodoSalottiOptimizado::~MetodoSalottiOptimizado()
{
}

void MetodoSalottiOptimizado::aplicar()
{
	/* Variables para el numero de puntos del contorno y el numero a optimizar*/
	int numeroPuntosContorno = contorno().numeroPuntosContorno();	// incluye el primero dos veces
	Contorno contornoOptimo, contornoPuntosDominantes;

	/* Matrices de los sumatorios */
	Matriz<double> suma_x(numeroPuntosContorno , numeroPuntosContorno );
	Matriz<double> suma_y(numeroPuntosContorno , numeroPuntosContorno );
	Matriz<double> suma_xx(numeroPuntosContorno , numeroPuntosContorno );
	Matriz<double> suma_yy(numeroPuntosContorno , numeroPuntosContorno );
	Matriz<double> suma_xy(numeroPuntosContorno , numeroPuntosContorno );

	/* Ahora vamos a calcular los sumatorios */
	calcularSumatorios(suma_x, suma_y, suma_xx, suma_yy, suma_xy);

	//Estas matrices que se declaran a continuación facilitarán las búsquedas, aunque no son estrictamente necesarias.
	double costeActual[numeroPuntosContorno + 1][_final + 1]; //matriz para guardar costes actuales
	bool abierta[numeroPuntosContorno + 1][_final + 1]; //Controla en que lista está el nodo junto con su rango
	bool cerrada[numeroPuntosContorno + 1][_final + 1]; //Controla en que lista está el nodo junto con su rango

	//Matriz para guardar el padre del nodo seleccionado y reconstruir aproximación
	int **padreActual = NULL;

	//Matriz para saber la posición de cada nodo con su rango en el heap
	int **posicionNodo = NULL;

	padreActual = new int * [numeroPuntosContorno + 1];
	for(int i = 0; i <= numeroPuntosContorno; i++)
		padreActual[i] = new int [_final + 1];

	posicionNodo = new int * [numeroPuntosContorno + 1];
	for(int i = 0; i <= numeroPuntosContorno; i++)
		posicionNodo[i] = new int [_final + 1];

	//Iteraciones para ir obteniendo los contornos optimos para los distintos números de puntos
	//Se obtiene el óptimo de k puntos.
	for(int k = _inicial; k <= _final; k++)	// Este bucle solo se ejecuta una vez ya que _inicial es igual a _final
	{
		//Estructuras necesarias para el A*
		list <Nodo> listaCerrada; //Lista que albergará los nodos ya seleccionados
		HeapVector listaAbierta; //heap que albergará los nodos candidatos

		//Todas las iteraciones utilizan la misma matriz, pero en cada iteración el trozo de matriz usado aumenta en una columna
		//Inicialización de los trozos de matriz que se usan en cada iteración.
		costeActual[1][0] = 0.0;
		for(int i = 0; i <= numeroPuntosContorno; i++) {	// creo que hay uno mas (<=) porque el 0 es el nodo padre del primero (ficticio)
			for(int j = 0; j <= k; j++)	// es el rango pero duda: no se porque hay un elementos mas
			{
				costeActual[i][j] = 0.0;
				padreActual[i][j] = -1;
				cerrada[i][j] = false;
				abierta[i][j] = false;
				posicionNodo[i][j] = 0;	// duda: ¿es necesario que lo actualice manualmente?
			}
		}

		//COMIENZA EL ALGORITMO A*

		// inicializamos la matriz donde se almacenaran los puntos de la aproximación
		_contornosOptimos = Matriz<int>(_final - _inicial + 1, k+1); // duda: no se si los tamaños puestos pueden dar error
		// obtenemos el contorno
		Contorno contorno = this->contorno();

		/*
		 * 1) Put the start node s corresponding to the "first point
		 * of the curve in the Open list and set g(s)=0.
		 */
		Punto p = contorno.puntoContorno(1);
		Nodo inicial(p,1,0,0,0.0);
		listaAbierta.insertar(inicial, posicionNodo);
		abierta[1][0] = true;
		costeActual[1][0] = 0.0;
		padreActual[1][0] = 0;

		while(true) {
			/**
			 * 2) Get the "First node n of the Open list whose total
			 * estimate f(n)=g(n)+h(n) is smallest and put it in the
			 * Closed list.
			 */
			Nodo minimo = listaAbierta.minimo();
			listaCerrada.push_back(minimo);
			listaAbierta.borrarMinimo(posicionNodo);
			abierta[minimo.getIndice()][minimo.getRango()] = false;
			cerrada[minimo.getIndice()][minimo.getRango()] = true;
			padreActual[minimo.getIndice()][minimo.getRango()] = minimo.getPadre();
			costeActual[minimo.getIndice()][minimo.getRango()] = minimo.getCoste();
			cout << "añado a cerrada: " <<  minimo.getIndice() << ", " << minimo.getCoste() << ", " << minimo.getRango() << ", " << minimo.getPadre() << endl;

			/*
			 * 3) If n is the last point of the curve and the global error
			 * is below the value of MaxGlobalError, exit with the
			 * solution of the polygonal approximation obtained by
			 * tracing back through the pointers.
			 */

			if((minimo.getRango() == k-1) && (minimo.getIndice()==numeroPuntosContorno)) { // el indice del ultimo elemento de la lista es el ultimo punto del contorno que es el primero	// duda: no entiendo lo del maxglobal error  // duda: numero de puntos de contorno -1
				_contornosOptimos.elemento(1,k,minimo.getIndice());
				for(int i=k-1; i>=1; i--)
					_contornosOptimos.elemento(1, i, padreActual[_contornosOptimos.elemento(1,i+1)][i]); // i es el rango (no se si seria "i" en padre actual)
				_error.push_back(minimo.getCoste());
				cout << "coste final: " << minimo.getCoste() << endl;
				return;
			}

			/*
			 * 4) Expand node n, generating all of its successors. Then,
			 * for each successor n'
			 */
			//cout << "padre: " << minimo.getIndice() << ", " << minimo.getRango() << ", " << minimo.getCoste() << endl;
			for(int i=minimo.getIndice()+1; i<=numeroPuntosContorno-(k-1-(minimo.getRango()+1)); i++) {

				if((minimo.getRango()+1 == k-1) && i<numeroPuntosContorno ) {
					cout << "";
				}
				else {

					double costeAdicional = sumaErrores(minimo.getIndice(), i, suma_x, suma_y, suma_xx, suma_yy, suma_xy);
					Nodo sucesor(contorno.puntoContorno(i), i, minimo.getRango()+1, minimo.getIndice(), minimo.getCoste()+costeAdicional);
					cout << "sucesor: " <<  sucesor.getIndice() << ", " << sucesor.getCoste() << ", " << sucesor.getRango() << ", " << sucesor.getPadre() << endl;
					/**
					 * 5) If it is not already in the Open list, set g(n')...,
					 * set f(n')..., insert the new node in the Open list at the right
					 * place (according to the value of f (n')), and direct a
					 * pointer from it back to n.
					 */
					if (!abierta[sucesor.getIndice()][sucesor.getRango()]) {
						listaAbierta.insertar(sucesor, posicionNodo);
						abierta[sucesor.getIndice()][sucesor.getRango()] = true;
						costeActual[sucesor.getIndice()][sucesor.getRango()] = sucesor.getCoste();
						padreActual[sucesor.getIndice()][sucesor.getRango()] = sucesor.getPadre();
					}
					/*
					 * 6) Else the successor n was already in the Open list,
					 * update its value, g(n')... and if necessary (if g(n') is modified),
					 * redirect to n the pointer to the father, update f(n') and update
					 * the place of the node in the Open list.
					 */
					else {
						// miramos si el coste es menor ahora y segun ello actualizamos
						if(sucesor.getCoste() < costeActual[sucesor.getIndice()][sucesor.getRango()]) {
							listaAbierta.actualizar(sucesor,sucesor,posicionNodo);
							costeActual[sucesor.getIndice()][sucesor.getRango()] = sucesor.getCoste();
							padreActual[sucesor.getIndice()][sucesor.getRango()] = sucesor.getPadre();
						}
					}
				}

			}

			cout << "Lista abierta:" << endl;
			listaAbierta.imprimir();



			/*
			 * Go to 2)
			 */
		}
	}

}

void MetodoSalottiOptimizado::calcularSumatorios(Matriz <double> &suma_x, Matriz <double> &suma_y, Matriz <double> &suma_xx, Matriz <double> &suma_yy, Matriz <double> &suma_xy)
{
	int numeroPuntosContorno;
	double x, y; //Coordenadas de un punto

	numeroPuntosContorno = contorno().numeroPuntosContorno();

	//Primero se calculan los sumatorios de la primera fila.
	//Cada elemento j, se obtiene a partir del j - 1, sumándole el sumando correspondiente al punto j - 1

	for(int j = 3; j <= numeroPuntosContorno; j++)
	{
		x = contorno().puntoContorno(j - 1).x(); //cordenada x del punto j - 1
		y = contorno().puntoContorno(j - 1).y(); //cordenada y del punto j - 1
		suma_x.elemento(1, j, suma_x.elemento(1, j - 1) + x);
		suma_y.elemento(1, j, suma_y.elemento(1, j - 1) + y);
		suma_xx.elemento(1, j, suma_xx.elemento(1, j - 1) + x * x);
		suma_yy.elemento(1, j, suma_yy.elemento(1, j - 1) + y * y);
		suma_xy.elemento(1, j, suma_xy.elemento(1, j - 1) + x * y);
	}

	//A partir de la segunda fila, los sumatorios se calculan a partir de la fila anterior.
	//Cada elemento i,j se obtiene restándole al elemento (i-1, j) el sumando del punto i
	for(int i = 2; i <= numeroPuntosContorno - 2; i++)
	{
		x = contorno().puntoContorno(i).x(); //cordenada x del punto i
		y = contorno().puntoContorno(i).y(); //cordenada y del punto i
		for(int j = i + 2; j <= numeroPuntosContorno; j++)
		{
			suma_x.elemento(i, j, suma_x.elemento(i - 1, j) - x);
			suma_y.elemento(i, j, suma_y.elemento(i - 1, j) - y);
			suma_xx.elemento(i, j, suma_xx.elemento(i - 1, j) - x * x);
			suma_yy.elemento(i, j, suma_yy.elemento(i - 1, j) - y * y);
			suma_xy.elemento(i, j, suma_xy.elemento(i - 1, j) - x * y);
		}
	}

	//Ahora se calculan los de la primera columna.
	//Cada elemento (i,1) se obtiene sumándole al elemento (i, n) el sumando del punto n.
	/* for(int i = 2; i <= numeroPuntosContorno; i++)
  {
   	x = contorno().puntoContorno(numeroPuntosContorno).x(); //cordenada x del punto i
    y = contorno().puntoContorno(numeroPuntosContorno).y(); //cordenada y del punto i
    suma_x.elemento(i, 1, suma_x.elemento(i, numeroPuntosContorno) + x);
    suma_y.elemento(i, 1, suma_y.elemento(i, numeroPuntosContorno) + y);
    suma_xx.elemento(i, 1, suma_xx.elemento(i, numeroPuntosContorno) + x * x);
    suma_yy.elemento(i, 1, suma_yy.elemento(i, numeroPuntosContorno) + y * y);
		suma_xy.elemento(i, 1, suma_xy.elemento(i, numeroPuntosContorno) + x * y);
  }*/
}
double MetodoSalottiOptimizado::sumaErroresMinima(const int &inicio, const int &fin, const Matriz <double> &suma_x, 
		const Matriz <double> &suma_y, const Matriz <double> &suma_xx, const Matriz <double> &suma_yy,
		const Matriz <double> &suma_xy)
{
	//Esta funcion calcula el eje de minima inercia entre inicio y fin para calcular la aproximacion que produciría
	// minimo error (ojo, no pasa por inicio y fin, y se calcula a efectos de podar a partir de un limite inferior del error)

	if (inicio == fin)
		return 0.0;
	if (inicio == fin - 1)
		return 0.0;

	double xMedia, yMedia, xVarianza, yVarianza, xyCovarianza;
	int numeroPuntos = fin - inicio - 1;
	double ux, uy; /* Coordenadas del vector propio correspondiente al maximo valor propio */
	double sumaErroresCuadrado;

	xMedia = suma_x.elemento(inicio, fin) / numeroPuntos;
	yMedia = suma_y.elemento(inicio, fin) / numeroPuntos;
	xVarianza = suma_xx.elemento(inicio, fin) / numeroPuntos -
			(suma_x.elemento(inicio, fin) * suma_x.elemento(inicio, fin)) / (numeroPuntos * numeroPuntos);
	yVarianza = suma_yy.elemento(inicio, fin) / numeroPuntos -
			(suma_y.elemento(inicio, fin) * suma_y.elemento(inicio, fin)) / (numeroPuntos * numeroPuntos);
	xyCovarianza = suma_xy.elemento(inicio, fin) / numeroPuntos - 
			(suma_x.elemento(inicio, fin) * suma_y.elemento(inicio, fin)) / (numeroPuntos * numeroPuntos);

	/** Calculo de valores propios de la matriz de covarianzas */
	double a1, b1, c1, l1;
	a1 = 1.0;
	b1 = -(xVarianza +yVarianza);
	c1 = xVarianza * yVarianza -xyCovarianza * xyCovarianza ;

	/* Maximo valor propio */
	l1=(-b1+sqrt(b1*b1-4.0*a1*c1))/(2.0*a1);


	if (fabs(xyCovarianza) < 0.00001)
	{
		if (fabs(l1-yVarianza) < 0.00001)
		{
			ux=0.0;
			uy=1.0;
		}
		else  /*(fabs(l1 - xVarianza) < 0.00001)*/
		{
			ux=1.0;
			uy=0.0;
		}
	} 
	else
	{
		ux=1.0;
		uy=(l1 -xVarianza)/xyCovarianza;
	}

	/* La recta tiene de vector (ux, uy) y pasa por (xMedia, yMedia) */
	/* y = a + bx*/
	/*En implicitas la recta es Ax + By + C = 0, donde A = -b, B = 1, C = -a */
	double a, b;
	double A, C;
	if (fabs(ux) > 0.00000001)
	{
		b = uy / ux;
		a = yMedia - b * xMedia;
		A = -b;
		C = -a;
		sumaErroresCuadrado = ( numeroPuntos * a * a + 2.0 * a * b * suma_x.elemento(inicio, fin) + 
				b * b * suma_xx.elemento(inicio, fin) - 2.0 * a * suma_y.elemento(inicio, fin) -
				2.0 * b * suma_xy.elemento(inicio, fin) + suma_yy.elemento(inicio, fin)) / (b * b + 1.0);
	}
	else //Recta paralela al eje y -> x = xMedia ya que pasa por el punto (xMedia, yMedia)
	{
		A = 1.0;
		C = -xMedia;
		sumaErroresCuadrado = suma_xx.elemento(inicio, fin) + (2.0 * (C / A)) * suma_x.elemento(inicio, fin) + 
				numeroPuntos * (C/A) * (C/A);
	}
	return sumaErroresCuadrado;
}

double MetodoSalottiOptimizado::sumaErrores(const int &inicio, const int &fin, const Matriz <double> &suma_x, 
		const Matriz <double> &suma_y, const Matriz <double> &suma_xx, const Matriz <double> &suma_yy,
		const Matriz <double> &suma_xy)
{
	//Esta funcion calcula los errores entre inicio y fin al aproximar el contorno en ese trozo
	//por el segmento que une los puntos inicio y fin, por eso el numero de puntos es (fin - inicio - 1)

	double sumaErroresCuadrado = 0.0;
	double A, B, C; //Coeficientes de la recta en forma general. Ax + By + C = 0
	double a, b; //Coeficientes de la recta en forma explícita. y = a + bx

	if (inicio == fin)
		return 0.0;
	if (inicio == fin - 1)
		return 0.0;
	//printf("Inicio = %d, Fin = %d", inicio, fin);
	//Los dos puntos son iguales. Los errores se calculan calculando las distancias de los intermedios a ese punto
	if (contorno().puntoContorno(inicio)== contorno().puntoContorno(fin))
	{
		for(int i = inicio + 1; i < fin -1; i++)
			sumaErroresCuadrado += contorno().puntoContorno(inicio).distanciaPuntos(contorno().puntoContorno(fin));

		return sumaErroresCuadrado;
	}

	Recta recta(contorno().puntoContorno(inicio), contorno().puntoContorno(fin));
	//printf("Punto Inicio %lf %lf \n", contorno().puntoContorno(inicio).x(), contorno().puntoContorno(inicio).y());
	//printf("Punto Fin %lf %lf \n", contorno().puntoContorno(fin).x(), contorno().puntoContorno(fin).y());


	//Como la recta se calcula en forma general, hay que calcular los coeficientes en forma explicita (a y b)
	A = recta.a();
	B = recta.b();
	C = recta.c();


	//printf("A = %lf B = %lf C = %lf \n", A, B, C);

	if (fabs(B) > 0.0000001) //Recta no paralela al eje y
	{
		a = - C / B;
		b = - A / B;
		sumaErroresCuadrado = ( (fin - inicio - 1.0) * a * a + 2.0 * a * b * suma_x.elemento(inicio, fin) +
				b * b * suma_xx.elemento(inicio, fin) - 2.0 * a * suma_y.elemento(inicio, fin) -
				2.0 * b * suma_xy.elemento(inicio, fin) + suma_yy.elemento(inicio, fin)) / (b * b + 1.0);

	}
	else //Recta paralela al eje y. En este caso b sería infinito y se hace el cálculo de otra forma.
	{
		sumaErroresCuadrado = suma_xx.elemento(inicio, fin) + (2.0 * (C / A)) * suma_x.elemento(inicio, fin) +
				(fin - inicio -1) * (C/A) * (C/A);
	}

	if (isnan(sumaErroresCuadrado))
	{
		cout << "\ninicio = "<< inicio << endl;
		cout << "fin = " << fin << endl;
		cout << " A = " << A << endl;
		cout << "B = " << B << endl;
		cout << "C = " << C << endl;
	}
	return sumaErroresCuadrado;
}



void MetodoSalottiOptimizado::obtenerPuntosContornoOptimo(Contorno &contorno, int ** padre, const int & numeroPuntosProbados)
{
	int i, j, contador = numeroPuntosProbados;
	i = contorno.numeroPuntosContorno();
	j = numeroPuntosProbados;

	//El primer punto dominante que se asigna es el punto inicial del contorno. Esto viene impuesto por el método 	de Pérez y Vidal.
	contorno.dominantePunto(1, true);
	do
	{
		contorno.dominantePunto(padre[i][j], true);
		//printf("Punto dominante asignado = %d\n", padre.elemento(i, j));
		//getchar();

		contador--;
		i = padre[i][j];
		--j;
	}while(padre[i][j] != 1 /*&& contador > 1*/);
	//!!Ojo, quitar la segunda parte del predicado. Se ha puesto para que funcione con el martillo !!!!
}
