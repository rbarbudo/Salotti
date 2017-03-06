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
	int numeroPuntosContorno = contorno().numeroPuntosContorno();
	Contorno contornoOptimo, contornoPuntosDominantes;

	/*Variables para guardar los errores */
	double error, errorMaximo, sumaErroresCuadrado;

	/* Matrices de los sumatorios */
	Matriz<double> suma_x(numeroPuntosContorno , numeroPuntosContorno );
	Matriz<double> suma_y(numeroPuntosContorno , numeroPuntosContorno );
	Matriz<double> suma_xx(numeroPuntosContorno , numeroPuntosContorno );
	Matriz<double> suma_yy(numeroPuntosContorno , numeroPuntosContorno );
	Matriz<double> suma_xy(numeroPuntosContorno , numeroPuntosContorno );

	/* Ahora vamos a calcular los sumatorios */
	calcularSumatorios(suma_x, suma_y, suma_xx, suma_yy, suma_xy);

	/*
	//Inicializa el vector de frecuencias de los dominantes.
	for(int i = 0; i <= numeroPuntosContorno; i++)
		_frecuenciaDominantes.push_back (0);
	//Matriz para ver los puntos dominantes en las sucesivas iteraciones
	Matriz <int> matrizPuntosDominantes(_final, numeroPuntosContorno);
	*/

	//Estas matrices que se declaran a continuación facilitarán las búsquedas, aunque no son estrictamente necesarias.
	//El primer índice indica número de nodo y el segundo indica rango.
	double costeActual[numeroPuntosContorno + 1][_final + 1]; //matriz para guardar costes actuales
	bool abierta[numeroPuntosContorno + 1][_final + 1]; //Controla en que lista está el nodo junto con su rango
	bool cerrada[numeroPuntosContorno + 1][_final + 1]; //Controla en que lista está el nodo junto con su rango

	//Matriz para guardar el padre del nodo seleccionado y reconstruir aproximación
	int **padreActual = NULL;

	//Matriz para saber la posición de cada nodo con su rango en el heap
	int **posicionNodo = NULL;

	//Valor para la poda.
	//Se usará una variable estática que contendrá el mínimo valor encontrado hasta ese momento.
	//static double limitePoda = 1000000000.0; //Se inicializa a valor infinito para que no pode la primera vez

	//Contador para ver las podas porque la solucion no es viable ya que no mejora a la mejor solución del momento.
	static int podadosPrematuros = 0;

	//variable para ver el punto inicial que se está tratando.
	//static int puntoInicial = 0;

	//puntoInicial++;

	//cout << "\n Punto Inicial = " << puntoInicial << endl;

	padreActual = new int * [numeroPuntosContorno + 1];
	for(int i = 0; i <= numeroPuntosContorno; i++)
		padreActual[i] = new int [_final + 1];

	posicionNodo = new int * [numeroPuntosContorno + 1];
	for(int i = 0; i <= numeroPuntosContorno; i++)
		posicionNodo[i] = new int [_final + 1];

	int contadorPodados;
	int contadorTotalPodados = 0;
	int contadorEstados = 0;
	double tiempoTotal = 0;

	//_contornosOptimos = new int [_final - _inicial + 1];

	//Iteraciones para ir obteniendo los contornos optimos para los distintos números de puntos
	//Se obtiene el óptimo de k puntos.
	for(int k = _inicial; k <= _final; k++)
	{
		//Estructuras necesarias para el A*
		list <Nodo> listaCerrada; //Lista que albergará los nodos ya seleccionados
		HeapVector listaAbierta; //heap que albergará los nodos candidatos

		contadorPodados = 0;
		//Todas las iteraciones utilizan la misma matriz, pero en cada iteración el trozo de matriz usado aumenta en una columna
		//Inicialización de los trozos de matriz que se usan en cada iteración.
		costeActual[1][0] = 0.0;
		for(int i = 0; i <= numeroPuntosContorno; i++)
			for(int j = 0; j <= k; j++)
			{
				costeActual[i][j] = 0.0;
				padreActual[i][j] = -1;
				cerrada[i][j] = false;
				abierta[i][j] = false;
				posicionNodo[i][j] = 0;
			}

		//Nodo minimo; //Nodo para almacenar el minimo en cada momento de la lista abierta

		//COMIENZA EL ALGORITMO A*
			// TODO
				// preguntar y corregir costes
				double costeFake = 0.0;
				// usar padreActual tb
				// revisar los true en los condicionales y en los bucles


		// inicializamos la matriz donde se almacenaran los puntos de la aproximación
		//_contornosOptimos[k-_inicial] = new int[k];

		// obtenemos el contorno y el primer punto
		Contorno contorno = this->contorno();
		Punto p = contorno.puntoContorno(0);

		/*
		 * 1) Put the start node s corresponding to the "first point
		 * of the curve in the Open list and set g(s)=0.
		 */
		Nodo inicial(p,1,0,0,0.0);	// el indice empieza en 1 para indicar mediante el padre 0 que es el primero
		listaAbierta.insertar(inicial, posicionNodo);
		abierta[0][0] = true;
		costeActual[0][0] = costeFake;

		while(true) {
			/**
			 * 2) Get the "First node n of the Open list whose total
			 * estimate f(n)=g(n)+h(n) is smallest and put it in the
			 * Closed list.
			 */
			// añadimos el minimo a la lista cerrada
			Nodo minimo = listaAbierta.minimo();
			listaCerrada.push_back(minimo);
			listaAbierta.borrarHeap();
			abierta[minimo.getIndice()-1][minimo.getRango()] = false;
			cerrada[minimo.getIndice()-1][minimo.getRango()] = true;
			costeActual[minimo.getIndice()-1][minimo.getRango()] = costeFake;

			// obtenemos info del ultimo añadido a la lista cerrada
			int rangoLast = listaCerrada.back().getRango();
			int indiceLast = listaCerrada.back().getIndice();
			double costeLast = listaCerrada.back().getCoste();

			/*
			 * 3) If n is the last point of the curve and the global error
			 * is below the value of MaxGlobalError, exit with the
			 * solution of the polygonal approximation obtained by
			 * tracing back through the pointers.
			 */
			if(listaCerrada.back().getIndice() < numeroPuntosContorno && true) {
				//_contornosOptimos[k-_inicial][k-1] = listaCerrada.back().getIndice();
				for(int i=k-2; i>=0; i++)
					//_contornosOptimos[k-_inicial][i] = padreActual[_contornosOptimos[k-_inicial][i+1]][i]; // i es el rango (no se si seria "i-1")
				return;
			}

			/*
			 * 4) Expand node n, generating all of its successors. Then,
			 * for each successor n'
			 */
			for(int i=indiceLast+1; i<numeroPuntosContorno; i++) {	// indiceLast+1: tomo los puntos a partir del siguiente
				Nodo sucesor(contorno.puntoContorno(i), i, rangoLast+1, indiceLast, costeFake);
				/**
				 * 5) If it is not already in the Open list, set g(n')...,
				 * set f(n')..., insert the new node in the Open list at the right
				 * place (according to the value of f (n')), and direct a
				 * pointer from it back to n.
				 */
				if (!abierta[sucesor.getIndice()][sucesor.getRango()]) {
					listaAbierta.insertar(sucesor, posicionNodo);
					abierta[sucesor.getIndice()][sucesor.getRango()] = true;
				}
				/*
				 * 6) Else the successor n was already in the Open list,
				 * update its value, g(n')... and if necessary (if g(n') is modified),
				 * redirect to n the pointer to the father, update f(n') and update
				 * the place of the node in the Open list.
				 */
				else {
					// miramos si el coste es menor ahora y segun ello actualizamos
					if(sucesor.getCoste() < costeActual[sucesor.getIndice()-1][sucesor.getRango()]) {
						//listaAbierta.actualizar( ,sucesor);
					}
				}

				// comprobamos si salimos del bucle para controlar que el numero de puntos no sea inferior a k
				if(rangoLast+numeroPuntosContorno-i < k-1) // es k-1 porque el rango empieza en 0
					break;
			}
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


double MetodoSalottiOptimizado::sumaErrores1(const int &inicio, const int &fin, const Matriz <double> &suma_x, 
		const Matriz <double> &suma_y, const Matriz <double> &suma_xx, const Matriz <double> &suma_yy,
		const Matriz <double> &suma_xy)
{
	/*Esta funcion calcula la suma de los errores de la regresión de y sobre x */

	double sumaErroresCuadrado = 0.0;
	double a, b; //Coeficientes de la recta en forma explícita. y = a + bx


	if (inicio == fin)
		return 0.0;
	if (inicio == fin - 1)
		return 0.0;
	//printf("Inicio = %d, Fin = %d", inicio, fin);

	//Calculamos los coeficientes b y a a partir de los sumatorios 
	double numeradorB, denominadorB;
	int numeroPuntos = fin - inicio - 1;

	numeradorB = suma_x.elemento(inicio, fin) * suma_y.elemento(inicio, fin) - 
			numeroPuntos * suma_xy.elemento(inicio, fin);
	denominadorB = suma_x.elemento(inicio, fin) * suma_x.elemento(inicio, fin) - 
			numeroPuntos * suma_xx.elemento(inicio, fin);

	//Si todos los valores de x son iguales b es infinito y el error es 0
	if (fabs(denominadorB) < 0.0000001)
		return 0.0;

	b = numeradorB / denominadorB;
	a = (suma_y.elemento(inicio, fin) / numeroPuntos) - (b * suma_x.elemento(inicio, fin) / numeroPuntos);

	sumaErroresCuadrado = numeroPuntos * a * a + suma_yy.elemento(inicio, fin) + b * b * suma_xx.elemento(inicio, fin) 
															- 2.0 * a * suma_y.elemento(inicio, fin) - 2.0 * b * suma_xy.elemento(inicio, fin) + 2.0 * a * b * suma_x.elemento(inicio, fin);
	/*

	//Ahora se calculan los errores punto a punto en la dirección y
	for (int i = inicio; i <= fin ; i++)
		sumaErroresCuadrado += (contorno().puntoContorno(i).y() - (a + b * contorno().puntoContorno(i).x())) * 
                           (contorno().puntoContorno(i).y() - (a + b * contorno().puntoContorno(i).x()));
	 */

	return sumaErroresCuadrado;
}

double MetodoSalottiOptimizado::sumaErrores2(const int &inicio, const int &fin, const Matriz <double> &suma_x, 
		const Matriz <double> &suma_y, const Matriz <double> &suma_xx, const Matriz <double> &suma_yy,
		const Matriz <double> &suma_xy)
{
	/*Esta funcion calcula la suma de los errores de la regresión de x sobre y */

	double sumaErroresCuadrado = 0.0;
	double a, b; //Coeficientes de la recta en forma explícita. x = a + by

	if (inicio == fin)
		return 0.0;
	if (inicio == fin - 1)
		return 0.0;
	//printf("Inicio = %d, Fin = %d", inicio, fin);

	//Calculamos los coeficientes b y a a partir de los sumatorios 
	double numeradorB, denominadorB;
	int numeroPuntos = fin - inicio - 1;

	numeradorB = suma_y.elemento(inicio, fin) * suma_x.elemento(inicio, fin) - 
			numeroPuntos * suma_xy.elemento(inicio, fin);
	denominadorB = suma_y.elemento(inicio, fin) * suma_y.elemento(inicio, fin) - 
			numeroPuntos * suma_yy.elemento(inicio, fin);

	//Si todos los valores de y son iguales b es infinito y el error es 0
	if (fabs(denominadorB) < 0.0000001)
		return 0.0;

	b = numeradorB / denominadorB;
	a = (suma_x.elemento(inicio, fin) / numeroPuntos) - (b * suma_y.elemento(inicio, fin) / numeroPuntos);

	//Ahora se calculan los errores punto a punto en la dirección y
	/*for (int i = inicio; i <= fin ; i++)
		sumaErroresCuadrado += (contorno().puntoContorno(i).x() - (a + b * contorno().puntoContorno(i).y())) * 
                           (contorno().puntoContorno(i).x() - (a + b * contorno().puntoContorno(i).y()));*/

	sumaErroresCuadrado = numeroPuntos * a * a + suma_xx.elemento(inicio, fin) + b * b * suma_yy.elemento(inicio, fin)
															- 2.0 * a * suma_x.elemento(inicio, fin) - 2.0 * b * suma_xy.elemento(inicio, fin) + 2.0 * a * b * suma_y.elemento(inicio, fin);
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
