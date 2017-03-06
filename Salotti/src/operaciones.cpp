#include <iostream>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include "operaciones.hpp"
#include <sys/time.h> //para la función gettimeofday
#include <math.h>
#include "contorno.hpp"
//#include "menus.hpp"
#include "recta.hpp"
#include "vector.hpp"
//#include "heapvectorMasood.hpp"
//#include "nodoMasood.hpp"

using namespace std;

/*void operacionesContornos()
{
	int opcion;
	 Variable para almacenar el nombre del fichero del contorno
	char nombreFichero[100], ficheroContornoBreakPoints[100], ficheroContornoCuadrilatero[100];
	 Parametros de rotacion y escalado
	double factorEscala, anguloRotacion;
	 Parametro para filtros gausianos y ruido aleatorio
	int factorEscalaEntero;
	 Parametro de escala para escalar usando codigo cadena
	double sigma;
	 Parametro para filtro mediana y filtro media
	int longitud;

	cout << "Introduzca Nombre del fichero del contorno a cargar: ";
	cin >> nombreFichero;

	 Se carga el contorno del fichero
	Contorno contorno(nombreFichero);

	// Se crea el contorno nuevo a partir del antiguo.
	Contorno contornoNuevo(contorno);

	opcion = menuOperaciones();
	getchar();

	switch(opcion)
	{
	case 1:  Rotar o/y escalar usando radios vectores, sin discretizar.
	{
		cout << " Introduzca factor de Escala (tanto por 1): ";
		cin >> factorEscala;

		cout << " Introduzca angulo a rotar (grados): ";
		cin >> anguloRotacion;
		//getchar();

		 pasamos el angulo a radianes
		anguloRotacion *= M_PI / 180.0;

		rotarContorno(contornoNuevo, anguloRotacion);

		escalarContorno(contornoNuevo, factorEscala);
		break;
	}

	case 2:
	{
		cout << " Introduzca factor de Escala (tanto por 1): ";
		cin >> factorEscala;

		cout << " Introduzca angulo a rotar (grados): ";
		cin >> anguloRotacion;
		//getchar();

		 pasamos el angulo a radianes
		anguloRotacion *= M_PI / 180.0;

		rotarContorno(contornoNuevo, anguloRotacion);

		escalarDiscretaContorno(contornoNuevo, factorEscala);
		break;
	}

	case 3:  Cambio de escala  entero, usa codigo de cadena
	{
		cout << " Introduzca factor de Escala (entero en tanto por 1): ";
		cin >> factorEscalaEntero;
		//getchar();

		 Asignamos el nuevo numero de puntos al contorno nuevo
		contornoNuevo.numeroPuntosContorno(factorEscalaEntero * contorno.numeroPuntosContorno());

		 Se inicializa el contorno Nuevo para almacenar el resultado de la operacion
		contornoNuevo.crearContorno();

		escalarEnteraContorno(contornoNuevo, contorno, factorEscalaEntero);
		break;
	}

	case 4:  Filtro gausiano
	{
		cout << " Introduzca valor de sigma (>= 1): ";
		cin >> sigma;
		//getchar();

		filtroGausiano(contorno, contornoNuevo, sigma);
		break;
	}

	case 5:  Filtro gausiano, primera derivada
	{
		cout << "Introduzca valor de sigma (>= 1): ";
		cin >> sigma;
		//getchar();

		filtroGausianoPrimeraDerivada(contorno, contornoNuevo, sigma);
		break;
	}

	case 6:  Filtro gausiano, segunda derivada
	{
		cout << " Introduzca valor de sigma (>= 1): ";
		cin >> sigma;
		//getchar();

		filtroGausianoSegundaDerivada(contorno, contornoNuevo, sigma);
		break;
	}

	case 7:  Ruido aleatorio
	{
		cout << " Introduzca valor de sigma (>= 1): ";
		cin >> sigma;
		//getchar();

		ruidoAleatorioContorno(contorno, contornoNuevo, sigma);
		break;
	}

	case 8:  Filtro Mediana
	{
		do
		{
			cout << " Introduzca numero de puntos vecinos (IMPAR) ";
			cin >> longitud;
		} while (longitud % 2 == 0);
		//getchar();

		filtroMediana(contorno, contornoNuevo, longitud);
		break;
	}

	case 9:  Filtro Media
	{
		do
		{
			cout << " Introduzca numero de puntos vecinos (IMPAR) ";
			cin >> longitud;
		} while (longitud % 2 == 0);
		//getchar();

		filtroMedia(contorno, contornoNuevo, longitud);
		break;
	}

	case 10: Casco poligonal convexo
	{
		cascoPoligonalConvexo(contorno, contornoNuevo);
		break;
	}

	case 11: Discretizar contorno
	{
		discretizarContorno(contornoNuevo);
		break;
	}

	case 12:  PAsar a conectividad 4
	{
		conectividad4(contorno, contornoNuevo);
		break;
	}

	case 13:  Obtener contorno con solo breakpoints (ojo, se pierde la conectividad)
	{
		cout << "Introduzca Nombre del fichero del contorno que contendrá solo los breakpoints: ";
		cin >> ficheroContornoBreakPoints;
		//Se reserva punto dominantes
		contornoNuevo.reservarPuntoDominanteContorno();
		contornoNuevo.marcarTodosPuntosDominantes(); //Antes de eliminar dominantes hay que marcar todos.

		obtenerContornoBreakpoints(contornoNuevo, ficheroContornoBreakPoints);
		break;
	}

	case 14:  Comprobar si un contorno es un cuadrilátero
	{
		double umbral;
		bool esCuadrilatero = false;
		//variables para medir el tiempo de ejecucion.
		struct timeval tInicial, tActual;
		double tiempo;
		cout << "Introduzca umbral máximo para cuadrilátero: ";
		cin >> umbral;

		//Se reserva punto dominantes
		contornoNuevo.reservarPuntoDominanteContorno();
		contornoNuevo.marcarTodosPuntosNoDominantes(); //Antes de obtenerlos se marcan todos como no dominantes
		//Medimos tiempos al principio.
		gettimeofday(&tInicial, NULL);
		esCuadrilatero = comprobarContornoCuadrilatero(contornoNuevo, umbral);
		//Medimos tiempos despues de cada pasada.
		gettimeofday(&tActual, NULL);
		tiempo = 0.001*((tActual.tv_sec * 1000000.0 + tActual.tv_usec) -
				(tInicial.tv_sec * 1000000.0 + tInicial.tv_usec));
		cout << "Tiempo (ms) = \n" << tiempo;
		if (esCuadrilatero)
		{
			cout << "Introduzca Nombre del fichero del contorno para guardar cudrilátero: ";
			cin >> ficheroContornoCuadrilatero;
			Contorno contornoPuntosDominantes(contornoNuevo.contarPuntosDominantes());
			contornoPuntosDominantes.reservarPuntoDominanteContorno();
			contornoPuntosDominantes.marcarTodosPuntosDominantes();
			contornoPuntosDominantes.copiarPuntosDominantes(contornoNuevo);
			contornoPuntosDominantes.guardarContornoDibujo(ficheroContornoCuadrilatero);
		}
		break;
	}
	}

	 Guardamos el nuevo contorno en un fichero nuevo
	if (opcion != 13 && opcion != 14) //En la opcion 13 la funcion guarda el contorno en fichero
	{
		cout << "Introduzca Nombre del fichero para guardar nuevo contorno: ";
		cin >> nombreFichero;

		contornoNuevo.guardarContorno(nombreFichero);
	}
} */

/* Esta función recibe un contorno en coordenadas reales y las pasa a enteras, aunque luego
almacene las coordenadas en reales */
void discretizarContorno(Contorno &contorno)
{
	Punto puntoContorno;
	int xAux, yAux;

	for(int i = 1; i <= contorno.numeroPuntosContorno(); i++)
	{
		puntoContorno = contorno.puntoContorno(i);
		xAux = puntoContorno.x() + 0.5; /* Se le suma 0.5 para redondear */
		yAux = puntoContorno.y() + 0.5; /* Se le suma 0.5 para redondear */
		puntoContorno.x((double)xAux);
		puntoContorno.y((double)yAux);
		contorno.puntoContorno(puntoContorno, i);
	}
}

void rotarContorno(Contorno &contorno, const double &angulo)
{
	Punto puntoContorno, centro;
	double xNueva, yNueva, alfaNuevo;

	/* El centro sigue igual y no se toca, pero sirve para calcular las nuevas coordenadas cartesianas */
	centro = contorno.centroContorno();

	/* Calculamos las nuevas coordenadas punto a punto */
	for(int i = 1; i <= contorno.numeroPuntosContorno(); i++)
	{
		puntoContorno = contorno.puntoContorno(i);
		alfaNuevo = puntoContorno.angulo() + angulo;

		/* Corregimos el angulo si sale negativo o sobrepasa a 2PI */
		if (alfaNuevo < 0.0)
			alfaNuevo = alfaNuevo + 2.0 * M_PI;
		if (alfaNuevo > 2.0 * M_PI)
			alfaNuevo = alfaNuevo - 2.0 * M_PI;

		puntoContorno.angulo(alfaNuevo);

		xNueva = puntoContorno.radio() * cos(puntoContorno.angulo()) + centro.x();
		puntoContorno.x(xNueva);
		yNueva = puntoContorno.radio() * sin(puntoContorno.angulo()) + centro.y();
		puntoContorno.y(yNueva);

		/* Una vez calculadas las coordenadas del nuevo punto, se asigna en la posicion correspondiente */
		contorno.puntoContorno(puntoContorno, i);
	}
} 

/* Funcion para cambiar de escala el contorno, usando los radios vectores y sin discretizar */
/* El numero de puntos del contorno se mantiene constante */

void escalarContorno(Contorno &contorno, const double &escala)
{
	Punto puntoContorno, centro;
	double xNueva, yNueva, radioNuevo;

	/* El centro sigue igual y no se toca, pero sirve para calcular las nuevas coordenadas cartesianas */
	centro = contorno.centroContorno();

	/* Calculamos las nuevas coordenadas punto a punto */
	for(int i = 1; i <= contorno.numeroPuntosContorno(); i++)
	{
		puntoContorno = contorno.puntoContorno(i);
		radioNuevo = puntoContorno.radio() * escala;
		puntoContorno.radio(radioNuevo);
		xNueva = radioNuevo * cos(puntoContorno.angulo()) + centro.x();
		puntoContorno.x(xNueva);
		yNueva = radioNuevo * sin(puntoContorno.angulo()) + centro.y();
		puntoContorno.y(yNueva);

		/* Una vez calculadas las coordenadas del nuevo punto, se asigna en la posicion correspondiente */
		contorno.puntoContorno(puntoContorno, i);
	}
} 

/* Funcion para cambiar de escala el contorno, usando los radios vectores y discretizando */
/* El numero de puntos del contorno se mantiene constante */

void escalarDiscretaContorno(Contorno &contorno, const double &escala)
{
	Punto puntoContorno, centro;
	double xNueva, yNueva, radioNuevo;

	/* El centro sigue igual y no se toca, pero sirve para calcular las nuevas coordenadas cartesianas */
	centro = contorno.centroContorno();

	/* Calculamos las nuevas coordenadas punto a punto */
	for(int i = 1; i <= contorno.numeroPuntosContorno(); i++)
	{
		puntoContorno = contorno.puntoContorno(i);
		radioNuevo = puntoContorno.radio() * escala;
		puntoContorno.radio(radioNuevo);
		xNueva = radioNuevo * cos(puntoContorno.angulo()) + centro.x();

		if (xNueva > 0.0)
			puntoContorno.x((int)(xNueva + 0.5));
		else
			puntoContorno.x((int)(xNueva - 0.5));

		yNueva = radioNuevo * sin(puntoContorno.angulo()) + centro.y();

		if (yNueva > 0.0)
			puntoContorno.y((int)(yNueva + 0.5));
		else
			puntoContorno.y((int)(yNueva - 0.5));

		/* Una vez calculadas las coordenadas del nuevo punto, se asigna en la posicion correspondiente */
		contorno.puntoContorno(puntoContorno, i);
	}
}

/* Funcion para cambiar de escala el contorno, usando el codigo de cadena y discretizando */
/* El numero de puntos del contorno varia segun la escala */

void escalarEnteraContorno(Contorno &contornoNuevo, const Contorno &contorno, const int &escala)
{
	/*Obtenemos el codigo de cadena del nuevo contorno, repitiendo "escala" veces */
	/* el codigo de cadena del contorno antiguo*/
	for (int i = 1; i <= contorno.numeroPuntosContorno(); i++)
	{
		for(int j = 1; j <= escala; j++)
			contornoNuevo.codigoCadenaPunto(escala * (i - 1) + j, contorno.codigoCadenaPunto(i));
	}

	/* Se calculan coordenadas cartesianas a partir del codigo de cadena en el contorno nuevo */
	contornoNuevo.coordenadasCartesianasCodigoCadena();
}

//Esta función obtiene el contorno de conectividad 4 a partir del contorno de conectividad 8

void conectividad4(const Contorno &contorno, Contorno &contornoNuevo)
{
	/* Primero calculamos cuantos puntos nuevos tendrá el nuevo contorno.
		Los puntos nuevos se obtendrán en aquellos puntos cuyo código de cadena equivalga a los ángulos 45, 135, 225 y 315 */
	/* Ojo, 90 grados es codigo de cadena 0 y los siguientes son en sentido horario. */
	/* Por tanto, los códigos impares corresponden a las diagonales (45, 135, 225, 315) */
	int numeroPuntosNuevos = 0;

	for (int i = 1; i <= contorno.numeroPuntosContorno(); i++)
	{
		if (contorno.codigoCadenaPunto(i) % 2 == 1)
			numeroPuntosNuevos++;
	}

	/* Ahora le sumamos los que ya tenía */
	numeroPuntosNuevos += contorno.numeroPuntosContorno();

	Contorno aux(numeroPuntosNuevos); //Creamos un auxiliar con los puntos necesarios.

	/* Recorremos el contorno original y vamos rellenando el codigo de cadena del auxiliar */
	int contador = 1; //Contador para el numero de puntos del auxiliar
	for (int i = 1; i <= contorno.numeroPuntosContorno(); i++)
	{
		if (contorno.codigoCadenaPunto(i) % 2 == 0) //En este caso no se añade ningún punto nuevo
		{
			aux.codigoCadenaPunto(contador, contorno.codigoCadenaPunto(i));
			contador++;
		}
		else //En este caso hay que añadir un punto nuevo
		{
			if (contorno.codigoCadenaPunto(i) == 1) //45
			{
				aux.codigoCadenaPunto(contador, 2); /* derecha */
				contador++;				
				aux.codigoCadenaPunto(contador, 0); /* arriba */
				contador++;
			}
			else if (contorno.codigoCadenaPunto(i) == 7) //135
			{
				aux.codigoCadenaPunto(contador, 6); /* izquierda */
				contador++;				
				aux.codigoCadenaPunto(contador, 0); /* arriba */
				contador++;
			}
			else if (contorno.codigoCadenaPunto(i) == 5) //225
			{
				aux.codigoCadenaPunto(contador, 6); /* izquierda */
				contador++;				
				aux.codigoCadenaPunto(contador, 4); /* abajo */
				contador++;
			}
			else //315
			{
				aux.codigoCadenaPunto(contador, 2); /* derecha */
				contador++;				
				aux.codigoCadenaPunto(contador, 4); /* abajo */
				contador++;
			}
		}	
	}

	/* Se calculan coordenadas cartesianas a partir del codigo de cadena en el contorno nuevo */
	aux.coordenadasCartesianasCodigoCadena();
	contornoNuevo = aux; //Se copia aux en el contorno nuevo.
}

//Esta función determina el punto inicial de un contorno
//Será el breakpoint mas alejado de la recta que une el anterior breakpoint con el siguiente breakpoint
//Este punto se usará como origen en todos los recorridos para 
//los métodos de estimación de puntos dominantes.
int calcularPuntoInicialContorno(const Contorno &contorno)
{
	int inicial = 1, anterior, siguiente, codigoAnterior, codigoSiguiente;
	double distanciaMaxima = 0.0;

	for (int i = 1; i <= contorno.numeroPuntosContorno(); i++)
	{
		anterior = i -1;
		if (anterior == 0)
			anterior = contorno.numeroPuntosContorno();

		siguiente = i;

		/** Localizamos el anterior alineado mas alejado **/
		codigoAnterior = contorno.codigoCadenaPunto(anterior);

		do
		{
			anterior--;
			if (anterior == 0)
				anterior = contorno.numeroPuntosContorno();
		} while(codigoAnterior == contorno.codigoCadenaPunto(anterior));

		anterior++;

		if (anterior > contorno.numeroPuntosContorno())
			anterior = 1;

		/** Localizamos el siguiente alineado mas alejado **/
		codigoSiguiente = contorno.codigoCadenaPunto(siguiente);

		do
		{
			siguiente++;
			if (siguiente > contorno.numeroPuntosContorno())
				siguiente = 1;
		} while(codigoSiguiente == contorno.codigoCadenaPunto(siguiente));

		/*** Obtenemos la recta que une los alineados más alejados anterior y posterior */
		Recta recta(contorno.puntoContorno(anterior), contorno.puntoContorno(siguiente));

		/** Calculamos la distancia del punto actual a la recta calculada, y comprobamos si es la maxima calculada*/
		Punto puntoActual(contorno.puntoContorno(i));
		if (recta.distanciaPuntoRecta(puntoActual) > distanciaMaxima)
		{
			distanciaMaxima = recta.distanciaPuntoRecta(puntoActual);
			inicial = i;
		}
	}
	return inicial;
}


/****************************************************************************************************/
/* Esta funcion determina el casco poligonal convexo. El casco estará formado por segmentos que dejan
a un mismo lado, por encima o por debajo al resto de puntos del contorno. Primero hay que determinar 
un punto inicial, que junto con el siguiente cumpla estas condiciones */
/****************************************************************************************************/ 

void cascoPoligonalConvexo(const Contorno &contorno, Contorno &casco)
{
	int posicionPuntoInicial, posicionPuntoActual, posicionPuntoSiguiente;
	int n, indicePuntoCasco = 1;
	/*Vector2D auxiliar para guardar los puntos del casco*/
	Punto *puntosCasco;
	Punto puntoActual, puntoSiguiente, puntoAux;

	//printf("Empiezo a calcular el casco\n");
	//getchar();
	//getchar();

	/*Se reservan el máximo numero de puntos posibles en el casco */
	n = contorno.numeroPuntosContorno();
	puntosCasco = new Punto [n + 1];

	/*Inicializamos todos los puntos del casco a -9999 */
	for(int i = 1; i <= n; i++)
	{
		puntosCasco[i].x(-9999);
		puntosCasco[i].y(-9999);
	}

	//printf("Calculo del punto inicial\n");
	//getchar();
	//getchar();

	/* Determinamos posicion del punto inicial del casco */
	posicionPuntoInicial = estimarPuntoInicialCascoPoligonal(contorno);
	//printf("El punto inicial es %d \n", posicionPuntoInicial);
	//getchar();
	//getchar();

	/*Introducimos el punto inicial en el casco*/
	puntoAux = contorno.puntoContorno(posicionPuntoInicial);
	puntosCasco[indicePuntoCasco].x(puntoAux.x());
	puntosCasco[indicePuntoCasco].y(puntoAux.y());

	/*Buscamos el resto de puntos del casco*/
	posicionPuntoActual = posicionPuntoInicial;
	posicionPuntoSiguiente = posicionPuntoActual + 1;

	if (posicionPuntoSiguiente > n)
		posicionPuntoSiguiente = 1;

	while(posicionPuntoSiguiente != posicionPuntoInicial)
	{
		puntoActual = contorno.puntoContorno(posicionPuntoActual);
		puntoSiguiente = contorno.puntoContorno(posicionPuntoSiguiente);
		Recta r(puntoActual, puntoSiguiente);
		//printf("El punto actual es %d \n", posicionPuntoActual);
		//getchar();

		/*Comprobamos si todos los puntos del contorno quedan a un lado de la recta r*/
		if (comprobarPuntos(contorno, r)) /*Todos los puntos quedan a un lado de la recta. Es lado del casco */
		{
			/*Introducimos puntoSiguiente en el casco*/
			indicePuntoCasco++;
			//printf("El punto %d es del casco\n", posicionPuntoSiguiente);
			//getchar();
			puntoAux = contorno.puntoContorno(posicionPuntoSiguiente);
			puntosCasco[indicePuntoCasco].x(puntoAux.x());
			puntosCasco[indicePuntoCasco].y(puntoAux.y());

			/* Actualizamos los valores de los puntos */
			posicionPuntoActual = posicionPuntoSiguiente;
			posicionPuntoSiguiente = posicionPuntoActual + 1;

			if (posicionPuntoSiguiente > n)
				posicionPuntoSiguiente = 1;
		}
		else /*No es lado del casco, actualizamos el punto siguiente*/
		{
			posicionPuntoSiguiente++;

			if (posicionPuntoSiguiente > n)
				posicionPuntoSiguiente = 1;
		}
	}

	/*Ahora almacenamos los puntos del casco en el nuevo contorno */
	/* Asignamos este numero de puntos al contorno nuevo */
	casco.numeroPuntosContorno(indicePuntoCasco);

	/* Se inicializa el contorno Nuevo para almacenar el resultado de la operacion */
	casco.crearContorno();

	for (int i = 1; i <= indicePuntoCasco; i++)
		casco.puntoContorno(puntosCasco[i], i);

	/*Liberamos el vector de puntos */
	delete [] puntosCasco;
	puntosCasco = NULL;
}

/*Esta funcion estima el punto inicial del casco poligonal convexo */
int estimarPuntoInicialCascoPoligonal(const Contorno &contorno)
{
	int n;
	Punto pInicial, pFinal;

	/*Ahora pegamos una pasada por todas las posibilidades, comenzando por el punto 1 */
	/*para obtener el primer punto del casco */
	n = contorno.numeroPuntosContorno();
	for(int i = 1; i < n; i++)
	{
		pInicial = contorno.puntoContorno(i);
		for(int j = i + 1; j <= n; j++)
		{
			//printf("Voy a probar la recta que va del punto %d al %d\n", i, j);
			//getchar();
			pFinal = contorno.puntoContorno(j);
			Recta r(pInicial, pFinal);
			if (pInicial.x() != pFinal.x() || pInicial.y() != pFinal.y()) //Para que no tenga en cuenta el ultimo punto
				//En el caso de que se repita.
			{
				/*Comprobamos si todos los puntos del contorno quedan a un lado de la recta r*/
				if (comprobarPuntos(contorno, r))
					return i;
			}
		}
		//printf("Paso al punto %d\n", i);
	}
	return 0;
}

/* Funcion que comprueba si todos los puntos del contorno quedan a un lado de la recta r */
bool comprobarPuntos(const Contorno &contorno, const Recta &r)
{
	int signoInicial, signoActual;
	int i, n;

	n = contorno.numeroPuntosContorno();
	i = 1;

	/* Determinamos el signo del primer punto que está a un lado u otro de la recta */
	signoInicial = r.signoPuntoRecta(contorno.puntoContorno(i));
	while(signoInicial == 0 && i < n)
	{
		//printf("El signo del punto %d es %d \n", i, signoInicial);
		//getchar();
		i++;
		signoInicial = r.signoPuntoRecta(contorno.puntoContorno(i));
	}


	/*Comprobamos si el resto de puntos están en el mismo lado (tienen el mismo signo) o no  */
	for(int j = i + 1; j <= n; j++)
	{
		signoActual = r.signoPuntoRecta(contorno.puntoContorno(j));
		if (signoActual != 0 && signoActual != signoInicial)
			return 0;
	}

	return 1;
}

/**************************************************************/
/**************************************************************/
/***** FUNCION: filtroGausiano                            *****/
/***** Parametros de entrada: contorno original, puntero a*****/
/*****                      contorno resultado, parametro *****/
/*****                        de suavizado (sigma)        *****/                    
/***** Parametros de salida:                              *****/
/***** Objetivo: Suavizar un contorno con una gausiana    *****/
/***** Funciones a las que llama                          *****/
/***** Fecha: 2-1-99                                      *****/
/***** Fecha ultima modificacion: 2-02-05                 *****/
/**************************************************************/
/**************************************************************/
void filtroGausiano(const Contorno &contornoOriginal, Contorno &contornoResultado, const double &sigma)
{
	/* Array en que se genera la gausiana */
	double *gausiana = NULL;
	/* Ancho de la gaussiana */
	int anchoGausiana;
	/* Suma de los valores de la gaussiana */
	double sumaGausiana = 0.0;
	/* Numero de puntos del contorno*/
	int numeroPuntosContorno;

	/* Calculo del ancho de la gausiana */
	anchoGausiana = (int)(6 * sigma) + 1;
	/* Se establece un valor mínimo para el ancho*/
	if (anchoGausiana < 3)
		anchoGausiana = 3;
	/* Se transforma en impar el ancho de la gausiana */
	if (!(anchoGausiana % 2))
		anchoGausiana++;
	/* El ancho no puede sobrepasar el numero de puntos del contorno */
	numeroPuntosContorno = contornoOriginal.numeroPuntosContorno();

	if (anchoGausiana > numeroPuntosContorno)
		anchoGausiana = numeroPuntosContorno;

	/* Reserva de espacio para la gausiana */
	gausiana = new double [(anchoGausiana + 1)];

	if (gausiana == NULL)
	{
		cout << " FALLO EN LA RESERVA DE MEMORIA \n";
		exit(0);
	}

	/* Se genera la gausiana */
	generarGausiana(gausiana, anchoGausiana, sigma, sumaGausiana);

	/* Se convoluciona y se obtiene contorno suavizado*/
	convolucionarGausiana(contornoOriginal, contornoResultado, gausiana, anchoGausiana, sumaGausiana,  	numeroPuntosContorno);
	/* Se libera la gausiana*/
	delete [] gausiana;
}

/**************************************************************/
/**************************************************************/
/***** FUNCION: filtroGausianoPrimeraDerivada             *****/
/***** Parametros de entrada: contorno original, puntero a*****/
/*****                      contorno resultado, parametro *****/
/*****                        de suavizado (sigma)        *****/                    
/***** Parametros de salida:                              *****/
/***** Objetivo: Suavizar un contorno con la derivada 1   *****/
/*****           de la gausiana                           *****/
/***** Funciones a las que llama                          *****/
/***** Fecha: 2-1-99                                      *****/
/***** Fecha ultima modificacion: 3-02-05                 *****/
/**************************************************************/
/**************************************************************/
void filtroGausianoPrimeraDerivada(const Contorno &contornoOriginal, Contorno &contornoResultado, const double &sigma)
{
	/* Array en que se genera la gausiana */
	double *gausianaPrimeraDerivada = NULL;
	/* Ancho de la gaussiana */
	int anchoGausiana;
	/* Suma de los valores de la gaussiana */
	double sumaGausiana = 0.0;
	/* Numero de puntos del contorno*/
	int numeroPuntosContorno;

	/* Calculo del ancho de la gausiana */
	anchoGausiana = (int)(6 * sigma) + 1;
	/* Se establece un valor mínimo para el ancho*/
	if (anchoGausiana < 3)
		anchoGausiana = 3;
	/* Se transforma en impar el ancho de la gausiana */
	if (!(anchoGausiana % 2))
		anchoGausiana++;
	/* El ancho no puede sobrepasar el numero de puntos del contorno */
	numeroPuntosContorno = contornoOriginal.numeroPuntosContorno();
	if (anchoGausiana > numeroPuntosContorno)
		anchoGausiana = numeroPuntosContorno;

	/* Reserva de espacio para la gausiana */
	gausianaPrimeraDerivada = new double [(anchoGausiana + 1)];
	if (gausianaPrimeraDerivada == NULL)
	{
		cout << " FALLO EN LA RESERVA DE MEMORIA \n";
		exit(0);
	}

	/* Se genera la gausiana */
	generarGausianaPrimeraDerivada(gausianaPrimeraDerivada, anchoGausiana, sigma, sumaGausiana);

	/* Se convoluciona y se obtiene contorno suavizado*/
	convolucionarGausiana(contornoOriginal, contornoResultado, gausianaPrimeraDerivada, anchoGausiana, sumaGausiana,  numeroPuntosContorno);

	/* Se libera la gausiana*/
	delete [] gausianaPrimeraDerivada;
}

/**************************************************************/
/**************************************************************/
/***** FUNCION: filtroGausianoSegundaDerivada             *****/
/***** Parametros de entrada: contorno original, puntero a*****/
/*****                      contorno resultado, parametro *****/
/*****                        de suavizado (sigma)        *****/                    
/***** Parametros de salida:                              *****/
/***** Objetivo: Suavizar un contorno con la derivada 1   *****/
/*****           de la gausiana                           *****/
/***** Funciones a las que llama                          *****/
/***** Fecha: 2-1-99                                      *****/
/***** Fecha ultima modificacion: 3-02-05                 *****/
/**************************************************************/
/**************************************************************/
void filtroGausianoSegundaDerivada(const Contorno &contornoOriginal, Contorno &contornoResultado, const double &sigma)
{
	/* Array en que se genera la gausiana */
	double *gausianaSegundaDerivada = NULL;	
	/* Ancho de la gaussiana */
	int anchoGausiana;
	/* Suma de los valores de la gaussiana */
	double sumaGausiana = 0.0;
	/* Numero de puntos del contorno*/
	int numeroPuntosContorno;

	/* Calculo del ancho de la gausiana */
	anchoGausiana = (int)(6 * sigma) + 1;
	/* Se establece un valor mínimo para el ancho*/
	if (anchoGausiana < 3)
		anchoGausiana = 3;
	/* Se transforma en impar el ancho de la gausiana */
	if (!(anchoGausiana % 2))
		anchoGausiana++;
	/* El ancho no puede sobrepasar el numero de puntos del contorno */
	numeroPuntosContorno = contornoOriginal.numeroPuntosContorno();
	if (anchoGausiana > numeroPuntosContorno)
		anchoGausiana = numeroPuntosContorno;

	/* Reserva de espacio para la gausiana */
	gausianaSegundaDerivada = new double [anchoGausiana + 1];
	if (gausianaSegundaDerivada == NULL)
	{
		cout << " FALLO EN LA RESERVA DE MEMORIA \n";
		exit(0);
	}

	/* Se genera la gausiana */
	generarGausianaSegundaDerivada(gausianaSegundaDerivada, anchoGausiana, sigma, sumaGausiana);

	/* Se convoluciona y se obtiene contorno suavizado*/
	convolucionarGausiana(contornoOriginal, contornoResultado, gausianaSegundaDerivada, anchoGausiana, sumaGausiana,  numeroPuntosContorno);

	/* Se libera la gausiana*/
	delete [] gausianaSegundaDerivada;
}

void generarGausiana(double *gausiana, const int &anchoGausiana, const double &sigma, double &sumaGausiana)
{
	double suma = 0.0;
	double amplitudGausiana;

	amplitudGausiana = 1.0 / (sqrt(2 * M_PI) * sigma);

	for (int i = -anchoGausiana / 2; i < 1 + anchoGausiana / 2; i++)
	{
		gausiana[i + anchoGausiana / 2] = amplitudGausiana * exp(-pow(i, 2.0)  / (2.0 * sigma * sigma));
		suma += fabs(gausiana[i + anchoGausiana / 2]);
	}
	sumaGausiana = suma;
}

void generarGausianaPrimeraDerivada(double *gausiana, const int &anchoGausiana, const double &sigma, double &sumaGausiana)
{
	double suma = 0.0;
	double amplitudGausiana;

	amplitudGausiana = -1.0 / (sqrt(2 * M_PI) * sigma * sigma * sigma);

	for (int i = -anchoGausiana / 2; i < 1 + anchoGausiana / 2; i++)
	{
		gausiana[i + anchoGausiana / 2] = amplitudGausiana * i *
				exp(-pow(i, 2.0)  / (2.0* sigma * sigma));
		suma += fabs(gausiana[i + anchoGausiana / 2]);
	}
	sumaGausiana = suma;
}

void generarGausianaSegundaDerivada(double *gausiana, const int &anchoGausiana, const double &sigma, double &sumaGausiana)
{
	double suma = 0.0;
	double amplitudGausiana;

	amplitudGausiana = -1.0 / (sqrt(2 * M_PI) * sigma * sigma * sigma);

	for (int i = -anchoGausiana / 2; i < 1 + anchoGausiana / 2; i++)
	{
		gausiana[i + anchoGausiana / 2] = amplitudGausiana * (1.0 - i * i /(sigma
				* sigma)) * exp(-pow(i, 2.0) / (2.0 * sigma *sigma));
		suma += fabs(gausiana[i + anchoGausiana / 2]);
	}
	sumaGausiana = suma;
}

void convolucionarGausiana(const Contorno &contornoOriginal, Contorno &contornoResultado, double *gausiana,
		const int &anchoGausiana, const double &sumaGausiana, const int &numeroPuntosContorno)
{
	/* Indices de recorrido */
	int indice;
	/* Sumas para la convolucion */
	double sumaX, sumaY;
	/* Punto auxiliar para recorrer los puntos del contorno */
	Punto punto;

	/* Recorre todos lospuntos del contorno */
	for (int i = 1; i <= numeroPuntosContorno; i++)
	{
		sumaX = 0.0;
		sumaY = 0.0;
		/* Cada punto lo obtiene como media ponderada con gausiana de sus vecinos */
		for (int j = -anchoGausiana / 2; j < 1 + anchoGausiana / 2; j++)
		{
			if (i + j > 0 && i + j <= numeroPuntosContorno)
				indice = (i + j);
			else if (i + j > numeroPuntosContorno)
				indice = (i + j) - numeroPuntosContorno;
			else
				indice = numeroPuntosContorno + (i + j);
			punto = contornoOriginal.puntoContorno(indice);
			sumaY += punto.y() * gausiana[j + anchoGausiana / 2];
			sumaX += punto.x() * gausiana[j + anchoGausiana / 2];
		}
		punto.x(sumaX / sumaGausiana);
		punto.y(sumaY / sumaGausiana);
		contornoResultado.puntoContorno(punto, i);
	}
}

/**************************************************************/
/**************************************************************/
/***** FUNCION: ruidoAleatorioContorno                    *****/
/***** Parametros de entrada: contorno original, puntero a*****/
/*****                      contorno resultado, parametro *****/
/*****                        de suavizado (sigma)        *****/                    
/***** Parametros de salida:                              *****/
/***** Objetivo: aplicar ruido gausiano a un contorno     *****/
/***** Funciones a las que llama                          *****/
/***** Fecha: 2-1-99                                      *****/
/***** Fecha ultima modificacion: 30-12-10                *****/
/**************************************************************/
/**************************************************************/
void ruidoAleatorioContorno(const Contorno &contornoOriginal, Contorno &contornoResultado, const double &sigma)
{
	/* Variables auxiliares para generar aleatoriamente una Normal(0,sigma) */
	double al1,al2,n1,n2;

	/* Puntos auxiliares para aplicar el ruido */
	Punto punto1, punto2;

	/* Numero de puntos del contorno */
	int numeroPuntosContorno;

	numeroPuntosContorno = contornoOriginal.numeroPuntosContorno();

	srand((unsigned)time(NULL));

	for(int i = 1;i <= numeroPuntosContorno;i = i + 2)
	{
		al1 = (double)rand() / RAND_MAX;
		al2 = (double)rand() / RAND_MAX;
		n1 = sqrt(-2.0 * log(al1)) * cos(2.0 * M_PI * al2) * sigma;
		n2 = sqrt(-2.0 * log(al1)) * sin(2.0 * M_PI * al2) * sigma;
		/* Basta con modificar aleatoriamente los radios vectores */
		if (i < numeroPuntosContorno)
		{
			punto1 = contornoOriginal.puntoContorno(i);
			punto2 = contornoOriginal.puntoContorno(i + 1);
			punto1.radio(punto1.radio() + n1);
			punto2.radio(punto2.radio() + n2);
			contornoResultado.puntoContorno(punto1, i);
			contornoResultado.puntoContorno(punto2, i + 1);;

		}
		else
		{
			punto1 = contornoOriginal.puntoContorno(i);
			punto1.radio(punto1.radio() + n1);
			contornoResultado.puntoContorno(punto1, i);
		}
	}
	/* Ahora hay que calcular de nuevo las coordenadas cartesianas y el nuevo centro */
	contornoResultado.coordenadasCartesianas();
	contornoResultado.calcularCentroContorno();
}

/**************************************************************/
/**************************************************************/
/***** FUNCION: filtroMediana                             *****/
/***** Parametros de entrada: contorno original, puntero a*****/
/*****                      contorno resultado, parametro *****/
/*****                        de suavizado (numero de p.) *****/                    
/***** Parametros de salida:                              *****/
/***** Objetivo: Suavizar un contorno pasando un filtro   *****/
/*****          mediana.                                  *****/
/***** Funciones a las que llama                          *****/
/***** Fecha: 2-1-99                                      *****/
/***** Fecha ultima modificacion: 30-12-10                *****/
/**************************************************************/
/**************************************************************/
void filtroMediana(const Contorno &contornoOriginal, Contorno &contornoResultado, const int &longitud)
{
	/* Vectores auxiliares para calcular la mediana en cada punto del contorno */
	double *xVector2D,*yVector2D;
	/* Punto auxiliar */
	Punto punto;
	/* Variables auxiliares */
	double xMediana, yMediana;
	int numeroPuntosContorno;

	numeroPuntosContorno = contornoOriginal.numeroPuntosContorno();

	for(int i = 1; i < numeroPuntosContorno; i++)
	{
		/* Se reserva espacio para los vectores */
		xVector2D = new double [longitud];
		yVector2D = new double [longitud];

		/* Se obtienen los vectores a partir de los puntos vecinos */
		obtenerVectoresPuntosVecinos(contornoOriginal, xVector2D, yVector2D, i, longitud);

		/* Se ordenan los vectores */
		ordenarVector(xVector2D, longitud);
		ordenarVector(yVector2D, longitud);

		/* Se obtiene la mediana de ambos vectores y se almacena como punto i en el nuevo contorno */
		xMediana = xVector2D[(int)(longitud / 2)];
		yMediana = yVector2D[(int)(longitud / 2)];
		punto.x(xMediana);
		punto.y(yMediana);
		contornoResultado.puntoContorno(punto, i);

		/* Se liberan los vectores */
		delete [] xVector2D;
		delete [] yVector2D;
	}
}

void obtenerVectoresPuntosVecinos(const Contorno &contorno, double *xVector2D, double *yVector2D, const int &i, const int &longitud)
{
	int indice;
	int numeroPuntosContorno;
	Punto punto;

	numeroPuntosContorno = contorno.numeroPuntosContorno();

	/* Recorre los vecinos del punto i para obtener los vectores */
	for(int j = (int)(-longitud) / 2; j < (int)(longitud / 2); j++)
	{
		if (i + j > 0 && i + j <= numeroPuntosContorno)
			indice = (i + j);
		else if (i + j > numeroPuntosContorno)
			indice = (i + j) - numeroPuntosContorno;
		else
			indice = numeroPuntosContorno + (i + j);

		punto = contorno.puntoContorno(indice);
		xVector2D[j + (int)(longitud / 2)] = punto.x();
		yVector2D[j + (int)(longitud / 2)] = punto.y();
	}
}

void ordenarVector(double *vector, const int &numero)
{
	double aux;

	for(int k = 0; k < numero; k++)
	{
		for(int l = k + 1; l < numero; l++)
		{
			if (vector[k] > vector[l])
			{
				aux = vector[k];
				vector[k] = vector[l];
				vector[l] = aux;
			}
		}
	}
}

/**************************************************************/
/**************************************************************/
/***** FUNCION: filtroMedia                               *****/
/***** Parametros de entrada: contorno original, puntero a*****/
/*****                      contorno resultado, parametro *****/
/*****                        de suavizado (numero de p.) *****/                    
/*****                       Se toman num puntos a izq. y *****/
/*****                       num a derecha                *****/
/***** Parametros de salida:                              *****/
/***** Objetivo: Suavizar un contorno pasando un filtro   *****/
/*****          media.                                    *****/
/***** Funciones a las que llama                          *****/
/***** Fecha: 28-9-03                                     *****/
/***** Fecha ultima modificacion: 8-02-05                 *****/
/**************************************************************/
/**************************************************************/
void filtroMedia(const Contorno &contornoOriginal, Contorno &contornoResultado, const int &longitud)
{
	/* Vectores auxiliares para calcular la mediana en cada punto del contorno */
	double *xVector2D,*yVector2D;
	/* Punto auxiliar */
	Punto punto;
	/* Variables auxiliares */
	double xMedia, yMedia;
	int numeroPuntosContorno;

	numeroPuntosContorno = contornoOriginal.numeroPuntosContorno();

	for(int i = 1; i < numeroPuntosContorno; i++)
	{
		/* Se reserva espacio para los vectores */
		xVector2D = new double [longitud];
		yVector2D = new double [longitud];

		/* Se obtienen los vectores a partir de los puntos vecinos */
		obtenerVectoresPuntosVecinos(contornoOriginal, xVector2D, yVector2D, i, longitud);

		/* Se obtiene la media de ambos vectores y se almacena como punto i en el nuevo contorno */
		xMedia = mediaVector(xVector2D, longitud);
		yMedia = mediaVector(yVector2D, longitud);
		punto.x(xMedia);
		punto.y(yMedia);
		contornoResultado.puntoContorno(punto, i);

		/* Se liberan los vectores */
		delete [] xVector2D;
		delete [] yVector2D;
	}
}

double mediaVector(double *vector, const int &longitud)
{
	double media = 0.0;

	for(int i = 0; i < longitud; i++)
		media += vector[i] / longitud;

	return media;
}


//Esta función elimina los puntos que están alineados en un contorno, partiendo desde el punto inicial del mismo.
//Esta funcion ha sido actualizada el 31-10-2014 usando el codigo de cadena, para tener en cuenta si el contorno
//es cerrado o no. Cuando el contorno es cerrado, el codigo de cadena del ultimo punto se ha calculado
//teniendo en cuenta que su siguiente es el 2º y no el primero, por eso ya no hay que considerar
//como caso especial la posible eliminación como alineado del ultimo punto.
//El segundo parametro es para devolver cuantos puntos quedan despues de eliminar los alineados

void eliminarAlineados(Contorno &contorno, int &numeroPuntosDominantes, const int &inicial)
{
	int numeroPuntosContorno;
	int numero;
	//Vector2D v, w;

	/* se usan para indicar la posicion de tres puntos dominantes seguidos */
	int tActual, tSiguiente, tAnterior;

	/* Variable para guardar el último punto dominante */
	int tUltimoBP = inicial;

	//Variable paramétrica que cuenta los puntos
	int t;

	numeroPuntosContorno = contorno.numeroPuntosContorno();
	numero = numeroPuntosDominantes;

	/* Eliminación de puntos alineados */
	/* Primero se pega una pasada que comienza en el punto inicial */
	//tUltimaPasada = inicial;

	/* Se daran varias pasadas */
	for(int pasada = 1; pasada <= 1; pasada++)
	{
		tAnterior = -1;
		tActual = -1;
		tSiguiente = -1;
		t = inicial;
		for (int contador = 1; contador <= numeroPuntosContorno; contador++)
		{
			if (t > numeroPuntosContorno) /*Para que el recorrido sea circular */
				t = t - numeroPuntosContorno;

			if (contorno.dominantePunto(t))
			{
				if (tAnterior == -1) /* asigna a tAnterior su primer valor */
					tAnterior = t;
				else if (tActual == -1) /* asigna a tActual su primer valor */
					tActual = t;
				else /* asigna a tSiguiente su primer y tambien los siguientes valores */
					tSiguiente = t;

				if (tSiguiente != -1) /*Ya se tienen tres puntos dominantes seguidos */
				{
					//Vector2D w(contorno.puntoContorno(tAnterior), contorno.puntoContorno(tActual));
					//Vector2D v(contorno.puntoContorno(tActual), contorno.puntoContorno(tSiguiente));

					//if (w.vectorUnitario() == v.vectorUnitario()) /* Estan alineados tAnterior, tActual y tSiguiente */
					if (contorno.codigoCadenaPunto(tAnterior) == contorno.codigoCadenaPunto(tActual)) //Estan alineados
					{
						/* Elimina tActual como dominante */
						contorno.dominantePunto(tActual, false);
						numero--;

						/*Actualiza los contadores tActual y tSiguiente*/
						tActual = tSiguiente;
						tSiguiente = -1;
					}
					else /* No estan alineados, se actualizan tAnterior, tActual y tSiguiente */
					{
						tUltimoBP = tActual;
						tAnterior = tActual;
						tActual = tSiguiente;
						tSiguiente = -1;
					}
				}
			}
			t++;
		}

		//cout << "Ultimo BP = " << tUltimoBP << endl;
		//Elimino todos los puntos marcados como dominantes desde el último BP
		for(int i = tUltimoBP + 1; i <= numeroPuntosContorno ; i++)
		{
			if (contorno.dominantePunto(i) == true)
			{
				contorno.dominantePunto(i, false);
				numero--;
			}
		}

		//Ahora vemos si hay que eliminar el primer punto del contorno como BP, ya que este se supuso que era el primer BP.
		/*	if (contorno.codigoCadenaPunto(tUltimoBP) == contorno.codigoCadenaPunto(1))
		//El primero esta alineado con el ultimo BP y con el segundo, por tanto se elimina
		{
			contorno.dominantePunto(1, false);
			numero--;
			cout << "Elimino el punto 1 de la lista de BPs" << endl;
		}
  	if (contorno.puntoContorno(numeroPuntosContorno) == contorno.puntoContorno(1)) //Es cerrado
			if (contorno.dominantePunto(1) == true) //Si el primero es dominante el ultimo tambien lo será
			{
				contorno.dominantePunto(numeroPuntosContorno, true);
				numero++;
			}*/
	}
	numeroPuntosDominantes = numero;
}

//La funcion siguiente se ha actualizado el 31 de Octubre de 2014 
//para tener en cuenta si el contorno es cerrado o no y así no tener problemas si hay
//coincidencia entre el primer y el último punto del contorno
//cosa que ocurre en contornos cerrados.

void eliminarCasiAlineados(Contorno &contorno, int &numeroPuntosDominantes, const double &distancia, const int &inicial)
{
	int numeroPuntosContorno;
	int numero;
	/* se usan para indicar la posicion de tres puntos dominantes seguidos */
	int tActual, tSiguiente, tAnterior;

	int t;
	/* Variable para guardar el último punto dominante de la pasada anterior, que será el primero para la segunda 	pasada */
	int tUltimaPasada;

	numeroPuntosContorno = contorno.numeroPuntosContorno();
	numero = numeroPuntosDominantes;

	/* Eliminación de puntos casi alineados */
	tAnterior = -1;
	tActual = -1;
	tSiguiente = -1;

	/* Primero se pega una pasada que comienza en el punto inicial */
	tUltimaPasada = inicial;


	//Se darán dos pasadas
	for(int pasada = 1; pasada <= 2; pasada++)
	{
		tAnterior = -1;
		tActual = -1;
		tSiguiente = -1;
		t = tUltimaPasada;
		for (int contador = 1; contador <= numeroPuntosContorno + 1; contador++)
		{
			if (t > numeroPuntosContorno) /*Para que el recorrido sea circular */
				t = t - numeroPuntosContorno;

			if (contorno.dominantePunto(t))
			{
				if (tAnterior == -1) /* asigna a tAnterior su primer valor */
					tAnterior = t;
				else if (tActual == -1) /* asigna a tActual su primer valor */
					tActual = t;
				else /* asigna a tSiguiente su primer valor y tambien los siguientes cuando ya se tienen tAnterior y tActual*/
				{ // !!!!OJO!!!!, aquí hay que tener en cuenta si es un contorno cerrado y si tActual es el ultimo
					// y t es 1, ya que coinciden primero con ultimo, en ese caso se sigue buscando el siguiente
					if (t == 1) //Estamos en el primero
					{
						if (tActual == numeroPuntosContorno) //Estamos en el ultimo punto del contorno
						{
							if (contorno.puntoContorno(numeroPuntosContorno) == contorno.puntoContorno(1)) //Es cerrado
								tSiguiente = -1; //Seguimos buscando siguiente
							else //No es cerrado
								tSiguiente = t;
						}
						else //No estamos en el ultimo punto del contorno y su siguiente seria el primero
							//Esto puede ocurrir en la segunda pasada.
							tSiguiente = 1; //Tomamos el 1 como siguiente
					}
					else
						tSiguiente = t;
				}
				if (tSiguiente != -1) /* Ya se tienen tres puntos dominantes seguidos */
				{
					/*Ecuacion recta que une el primero y el tercero*/
					Recta recta(contorno.puntoContorno(tAnterior), contorno.puntoContorno(tSiguiente));
					if (recta.distanciaPuntoRecta(contorno.puntoContorno(tActual)) <= distancia)
						/* La distancia del intermedio a la recta es inferior al umbral */
					{
						/* Se elimina el punto intermedio como dominante porque está casi alineado */
						contorno.dominantePunto(tActual, false);
						numero--;
						tActual = tSiguiente;
						tSiguiente = -1;
					}
					else /* Los puntos no están casi alineados y se actualizan los tres puntos */
					{
						tAnterior = tActual;
						tActual = tSiguiente;
						tSiguiente = -1;
					}
					tUltimaPasada = tActual;
				}
			}
			t++;
		}
	}
	if (contorno.puntoContorno(numeroPuntosContorno) == contorno.puntoContorno(1)) //Es cerrado
		if (contorno.dominantePunto(numeroPuntosContorno) == true) //Si el primero es dominante el ultimo tambien lo será
		{
			contorno.dominantePunto(1, true);
			numero++;
		}
	numeroPuntosDominantes = numero;
}

int eliminarCasiAlineadoMasood(Contorno &contorno, vector <double> &aev, vector <int> &ci, int numeroPuntosDominantes)
{
	/*Esta funciÃ³n elimina un solo punto, usando mÃ©todo de Masood. El punto eliminado serÃ¡ aquel que produzca un error cuadrÃ¡tico mÃ­nimo al eliminarlo */
	int eliminado;
	//int *cAux, contador;
	int posicionMinimo, anterior, posterior, anterioranterior, posteriorposterior;
	double aux;

	//Buscamos el mÃ­nimo de AEV.
	posicionMinimo = 1;
	for(int i = 2; i <= numeroPuntosDominantes; i++)
	{
		//printf("aev[%d] = %lf punto %d\n", i, aev[i], ci[i]);
		if (aev[i] < aev[posicionMinimo])
			posicionMinimo = i;
	}

	//printf("El break point que se va a eliminar es %d\n", ci[posicionMinimo]);
	eliminado = ci[posicionMinimo];
	//getchar();
	//getchar();


	//El punto de posicion c[posicionMinimo] se elimina como dominante.
	contorno.dominantePunto(ci[posicionMinimo], false);

	//Ahora hay que corregir el error asociado a los dos dominantes vecinos.
	//Primero calculamos las posiciones de los vecinos
	if (posicionMinimo == 1)
	{
		anterior = numeroPuntosDominantes;
		posterior = posicionMinimo + 1;
	}
	else if (posicionMinimo == numeroPuntosDominantes)
	{
		anterior = posicionMinimo - 1;
		posterior = 1;
	}
	else
	{
		anterior = posicionMinimo - 1;
		posterior = posicionMinimo + 1;
	}

	anterioranterior = anterior - 1;
	posteriorposterior = posterior + 1;
	if (anterioranterior <= 0)
		anterioranterior = numeroPuntosDominantes;
	if (posteriorposterior > numeroPuntosDominantes)
		posteriorposterior = 1;

	//cout << "Anterior Anterior = " << anterioranterior << endl;
	//assert(anterior >= 1 && anterior <= numeroPuntosDominantes);
	//assert(posterior >= 1 && posterior <= numeroPuntosDominantes);
	//assert(anterioranterior >= 1 && anterioranterior <= numeroPuntosDominantes);
	//assert(posteriorposterior >= 1 && posteriorposterior <= numeroPuntosDominantes);


	//Corregimos los errores.
	//Error del anterior.
	Recta recta(contorno.puntoContorno(ci[anterioranterior]), contorno.puntoContorno(ci[posterior]));
	aux = recta.distanciaPuntoRecta(contorno.puntoContorno(ci[anterior]));
	aev[anterior] = aux * aux;
	//printf("He corregido error anterior (%lf)\n", aev[anterior]);


	//Error del posterior.
	Recta recta1(contorno.puntoContorno(ci[anterior]), contorno.puntoContorno(ci[posteriorposterior]));
	aux = recta1.distanciaPuntoRecta(contorno.puntoContorno(ci[posterior]));
	aev[posterior] = aux * aux;
	//printf("He corregido error posterior (%lf) \n", aev[posterior]);


	//Ahora hay que eliminar el minimo de los vectores c y aev
	for(int i = posicionMinimo; i < numeroPuntosDominantes; i++)
	{
		ci[i] = ci[i+1];
		aev[i] = aev[i + 1];
	}

	//Devolvemos la posicion del dominante eliminado.
	return eliminado;
}

/*

int eliminarCasiAlineadoMasoodNuevo(Contorno &contorno, HeapVectorMasood &heap, int * & posicionNodo, vector <double> &aev, vector <int> &ci, int numeroPuntosDominantes)
{
	Esta función elimina un solo punto, usando método de Masood.
	 .El punto eliminado será aquel que produzca un error cuadrático mí­nimo al eliminarlo
	int eliminado, posicionMinimo, anterior, posterior, anterioranterior, posteriorposterior;

	//Para extraer el nodo de menor error del heap
	NodoMasood minimo;
	double aux;

	//Buscamos el mínimo del heap.
	minimo = heap.minimo();

	//Extraemos su orden dentro de los dominantes para poder actualizar los errores de su anterior y de su posterior
	//posicionMinimo es la posicion que ocupa teniendo en cuenta solo los dominantes.
	posicionMinimo = minimo.getIndice();

	//printf("El break point que se va a eliminar es %d\n", ci[posicionMinimo]);
	//getchar();
	//getchar();
	//eliminado es la posicion que ocupa en el contorno original
	eliminado = ci[posicionMinimo];

	//borramos el mínimo del heap 
	assert(heap.borrarMinimo(posicionNodo));

	//El punto de posicion eliminado se elimina como dominante.
	contorno.dominantePunto(eliminado, false);

	//Ahora hay que corregir el error asociado a los dos dominantes vecinos.
	//Primero calculamos las posiciones de los vecinos
	if (posicionMinimo == 1)
	{
		anterior = numeroPuntosDominantes;
		posterior = posicionMinimo + 1;
	}
	else if (posicionMinimo == numeroPuntosDominantes)
	{
		anterior = posicionMinimo - 1;
		posterior = 1;
	}
	else
	{
		anterior = posicionMinimo - 1;
		posterior = posicionMinimo + 1;
	}

	anterioranterior = anterior - 1;
	posteriorposterior = posterior + 1;
	if (anterioranterior < 0)
		anterioranterior = numeroPuntosDominantes;
	if (posteriorposterior > numeroPuntosDominantes)
		posteriorposterior = 1;

	//Almacenamos los nodos anterior y posterior para actualizarlos
	NodoMasood nodoAnteriorAntiguo, nodoPosteriorAntiguo;
	nodoAnteriorAntiguo.setCoste(aev[anterior]);
	nodoAnteriorAntiguo.setIndice(anterior);
	nodoPosteriorAntiguo.setCoste(aev[posterior]);
	nodoPosteriorAntiguo.setIndice(posterior);

	//Corregimos los errores.
	//Error del anterior.
	Recta recta(contorno.puntoContorno(ci[anterioranterior]), contorno.puntoContorno(ci[posterior]));
	aux = recta.distanciaPuntoRecta(contorno.puntoContorno(ci[anterior]));
	aev[anterior] = aux * aux;
	//printf("He corregido error anterior (%lf)\n", aev[anterior]);


	//Error del posterior.
	Recta recta1(contorno.puntoContorno(ci[anterior]), contorno.puntoContorno(ci[posteriorposterior]));
	aux = recta1.distanciaPuntoRecta(contorno.puntoContorno(ci[posterior]));
	aev[posterior] = aux * aux;
	//printf("He corregido error posterior (%lf) \n", aev[posterior]);

	//Ahora hay que actualizar los dos nodos (anterior y posterior) del heap
	NodoMasood nodoAnterior, nodoPosterior;

	nodoAnterior.setCoste(aev[anterior]);
	nodoAnterior.setIndice(anterior);
	nodoPosterior.setCoste(aev[posterior]);
	nodoPosterior.setIndice(posterior);

	//Actualizamos los nodos en el heap
	heap.actualizar(nodoAnteriorAntiguo, nodoAnterior, posicionNodo);
	//cout << "He actualizado nodo anterior \n";
	//getchar();
	//getchar();

	heap.actualizar(nodoPosteriorAntiguo, nodoPosterior, posicionNodo);
	//cout << "He actualizado nodo posterior \n";
	//getchar();
	//getchar();

	//Ahora hay que eliminar el minimo de los vectores c y aev
	for(int i = posicionMinimo; i < numeroPuntosDominantes; i++)
	{
		ci[i] = ci[i+1];
		aev[i] = aev[i + 1];
	}

	//Devolvemos la posicion del dominante eliminado.
	return eliminado;
}
*/

int eliminarCasiAlineadoMasoodOptimizado(Contorno &contorno, vector <double> &aev, vector <int> &ci, vector <int> &of, int numeroPuntosDominantes)
{
	int eliminado;
	int posicionMinimo, anterior, posterior, anterioranterior, posteriorposterior;
	double aux;

	//Buscamos el mínimo de AEV.
	posicionMinimo = 1;
	for(int i = 2; i <= numeroPuntosDominantes; i++)
	{
		//printf("aev[%d] = %lf punto %d\n", i, aev[i], ci[i]);
		if (aev[i] < aev[posicionMinimo])
			posicionMinimo = i;
	}

	//printf("El break point que se va a eliminar es %d(%d)\n", posicionMinimo, ci[posicionMinimo]);
	eliminado = ci[posicionMinimo];

	if (posicionMinimo < 1 || posicionMinimo > numeroPuntosDominantes)
	{
		cout << "posicionMinimo fuera de rango = " << posicionMinimo << endl;
		getchar();
	}

	//El punto de posicion c[posicionMinimo] se elimina como dominante.
	if (ci[posicionMinimo] < 1 || ci[posicionMinimo] > contorno.numeroPuntosContorno())
	{
		cout << "posicionMinimo fuera de rango = "<< ci[posicionMinimo] << endl;
		getchar();
	}

	contorno.dominantePunto(ci[posicionMinimo], false);

	//Ahora hay que corregir el error asociado a los dos dominantes vecinos.
	//Primero calculamos las posiciones de los vecinos

	anterior = posicionMinimo - 1;
	posterior = posicionMinimo + 1;
	if (posicionMinimo == 1)
		anterior = numeroPuntosDominantes;
	if (posicionMinimo == numeroPuntosDominantes)
		posterior = 1;

	//Calculamos el anterior del anterior y el posterior del posterior.
	anterioranterior = anterior - 1;
	posteriorposterior = posterior + 1;
	if (anterioranterior < 0)
		anterioranterior = numeroPuntosDominantes;
	if (posteriorposterior > numeroPuntosDominantes)
		posteriorposterior = 1;

	//Corregimos los errores.
	//Error del anterior.
	Recta recta1(contorno.puntoContorno(ci[anterioranterior]), contorno.puntoContorno(ci[posterior]));
	aux = recta1.distanciaPuntoRecta(contorno.puntoContorno(ci[anterior]));
	aev[anterior] = aux * aux;
	//printf("He corregido error anterior (%lf)\n", aev[anterior]);


	//Error del posterior.
	Recta recta2(contorno.puntoContorno(ci[anterior]), contorno.puntoContorno(ci[posteriorposterior]));
	aux = recta2.distanciaPuntoRecta(contorno.puntoContorno(ci[posterior]));
	aev[posterior] = aux * aux;
	//printf("He corregido error posterior (%lf) \n", aev[posterior]);
	//Ahora hay que marcar los break points vecinos del eliminado, que serán anterior y posterior.
	of[anterior] = 1;
	of[posterior] = 1;

	//Ahora hay que eliminar el minimo de los vectores ci, aev y of.
	for(int i = posicionMinimo; i < numeroPuntosDominantes; i++)
	{
		ci[i] = ci[i+1];
		aev[i] = aev[i + 1];
		of[i] = of[i + 1];
	}
	//printf("He marcado los puntos %d(%d) y %d(%d)\n", anterior, ci[anterior], posicionMinimo, ci[posicionMinimo]);

	//Devolvemos la posicion del dominante eliminado.
	return eliminado;
}

/*************************************************************************/
/* Esta funcion sirve para duplicar contornos cerrados (solo cerrados) ***/
/* Hay que tener en cuenta no duplicar el primero en la segunda vuelta ***/
/*************************************************************************/

void duplicarContorno(const Contorno &original, Contorno &duplicado)
{
	/* Asignamos el nuevo numero de puntos al contorno nuevo */
	duplicado.numeroPuntosContorno(2 * original.numeroPuntosContorno() - 1);
	// Ojo, le restamos 1 para que en la segunda vuelta no aparezca dos veces el primero, ya que es un contorno cerrado

	/* Se inicializa el contorno Nuevo para almacenar el resultado de la operacion */
	duplicado.crearContorno();

	// Primero copiamos la primera vuelta
	for(int i = 1; i <= original.numeroPuntosContorno(); i++)
		duplicado.puntoContorno(original.puntoContorno(i), i);

	//Ahora copiamos la segunda vuelta, que empezará por el segundo punto, para no repetir dos veces el primero
	for(int i = 2; i <= original.numeroPuntosContorno(); i++)
		duplicado.puntoContorno(original.puntoContorno(i), original.numeroPuntosContorno() + i - 1);
}

/*** Esta funcion obtiene el contorno compuesto por los brekpoints de un contorno dado y lo guarda en un fichero*/

void obtenerContornoBreakpoints(Contorno contorno, char *fichero)
{
	int numeroBreakPoints;
	FILE *pF;
	double x, y;
	double xInicio, yInicio;

	//Elimina los alineados del contorno a partir del punto 1
	eliminarAlineados(contorno, numeroBreakPoints, 1);

	//Abre el fichero de salida
	pF = fopen(fichero, "w");

	int contador = 1;

	while (contorno.dominantePunto(contador) == false)
		contador++;

	//Guardamos las coordenadas del primer breakpoint.
	xInicio = contorno.puntoContorno(contador).x();
	yInicio = contorno.puntoContorno(contador).y();
	fprintf(pF, "%lf %lf \n", xInicio, yInicio);


	for(int i = contador + 1; i <= contorno.numeroPuntosContorno(); i++)
	{
		if (contorno.dominantePunto(i) == true)
		{
			x = contorno.puntoContorno(i).x();
			y = contorno.puntoContorno(i).y();

			fprintf(pF, "%lf %lf \n", x, y);
		}
	}

	//Ahora duplicamos el punto inicial para cerrar el contorno de brekpoints.
	fprintf(pF, "%lf %lf \n", xInicio, yInicio);

	fclose(pF);
}

//Esta función se incorpora en julio de 2016 y sirve para comprobar si un contorno tiene 4 lados

bool comprobarContornoCuadrilatero(Contorno &contornoNuevo, const double &umbral)
{	
	//primero seleccionamos dos puntos aleatorios que van a ser el inicial y el central
	int inicial, central;	
	int numeroPuntosContorno = contornoNuevo.numeroPuntosContorno();
	inicial = 1;
	central = numeroPuntosContorno / 2;

	//inicial = contornoNuevo.puntoContorno(1);
	//central = contornoNuevo.puntoContorno(numeroPuntosContorno / 2);

	//Ahora se calculan los puntos más distantes a la recta que une esos puntos
	//uno en el recorrido inicial, central y otro en el recorrido central inicial

	//se supone que si es un cuadrilátero, estos dos puntos más distantes son vértices opuestos del cuadrilátero

	double eMax1, eMax3;
	int posicionMax1, posicionMax3;
	cout <<"CALCULO P1 \n";
	contornoNuevo.calcularEmaxEntreDosPuntos(inicial, central, eMax1, posicionMax1);
	cout << "posicion max 1 = " << posicionMax1 << endl;
	cout <<"CALCULO P3 \n";
	contornoNuevo.calcularEmaxEntreDosPuntos(central, numeroPuntosContorno, eMax3, posicionMax3);
	cout << "posicion max 3 = " << posicionMax3 << endl;


	//Ahora comprobamos si es un triángulo. Para ello se comprueba el emax entre los vértices recién obtenidos
	double e1, e2; 
	int p1, p2;

	cout <<"CALCULO error entre P1 y P3 \n";
	contornoNuevo.calcularEmaxEntreDosPuntos(posicionMax1, posicionMax3, e1, p1);
	cout <<"CALCULO error entre P3 y P1 \n";
	contornoNuevo.calcularEmaxEntreDosPuntos(posicionMax3, posicionMax1, e2, p2);

	if (e1 <= umbral or e2 <= umbral)
	{
		cout << "ES UN TRIÁNGULO \n";
		return false;
	}
	else //Calculamos los dos vértices que faltan
	{
		//se marcan como dominantes los vértices recién obtenidos
		contornoNuevo.dominantePunto(posicionMax1, true);
		contornoNuevo.dominantePunto(posicionMax3, true);
		double eMax2, eMax4;
		int posicionMax2, posicionMax4;		
		contornoNuevo.calcularEmaxEntreDosPuntos(posicionMax1, posicionMax3, eMax2, posicionMax2);
		contornoNuevo.calcularEmaxEntreDosPuntos(posicionMax3, posicionMax1, eMax4, posicionMax4);
		//se marcan como dominantes los vértices recién obtenidos
		contornoNuevo.dominantePunto(posicionMax2, true);
		contornoNuevo.dominantePunto(posicionMax4, true);		
		//Ahora comprobamos el error máximo del cuadrilátero, que será la aproxmación poligonal
		double eMax, ise; 
		contornoNuevo. diferenciaContornoDominantesReales(ise, eMax);

		if (eMax < umbral)
		{
			cout << "ES UN CUADRIlÁTERO\n";
			return true;
		}
		else
		{
			cout << "NO ES UN CUADRIlÁTERO\n";
			return false;
		}		
	}
}
