#include <vector>    // necesario para el contenedor Vector2D
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <math.h>
#include <cassert>
#include "contorno.hpp"
#include "punto.hpp"
#include "recta.hpp"
#include "vector.hpp"
#include "matriz.hpp"
#include "operaciones.hpp"


using namespace std;

/* FUNCIONES DE INTERFAZ PARA EL TIPO CONTORNO */
//Constructor vacío
Contorno::Contorno()	
{
	numeroPuntosContorno(0);
	_puntoContorno = NULL; 
	_t = NULL; 
	_codigoCadena = NULL; 
	_dominante = NULL; 
	_regionSoporteAnterior = NULL; 
	_regionSoportePosterior = NULL; 
	_parametroCurvatura = NULL;

}

//Constructor. Crea un contorno vacío con n puntos
Contorno::Contorno(const int &n)	
{
	numeroPuntosContorno(n);
	//Se reserva memoria para los puntos del contorno 
	reservarPuntosContorno();
	reservarCodigoCadenaContorno();
	inicializarContorno();
}

//Constructor. Crea un contorno a partir de un fichero
Contorno::Contorno(char *nombreFichero)	
{
	cargarContorno(nombreFichero);
}

//Destructor
Contorno::~Contorno() 
{
	liberarContorno();
}

//Constructor de copia.

Contorno::Contorno(const Contorno &contorno)
{
	Punto centro;

	//Asignamos numero de puntos
	numeroPuntosContorno(contorno.numeroPuntosContorno());

	//Se reserva memoria para los puntos del contorno
	reservarPuntosContorno();
	reservarCodigoCadenaContorno();

	//Inicializar a 0 todos los puntos y parametros del contorno
	inicializarContorno();

	//asignamos el punto central
	centroContorno(contorno.centroContorno());

	// Asignamos todos los puntos del contorno
	for (int i = 1; i <= numeroPuntosContorno(); i++)
	{
		puntoContorno(contorno.puntoContorno(i), i);
		codigoCadenaPunto(i, contorno.codigoCadenaPunto(i));
	}

	//Si ya se tienen regiones de soporte, se asignan
	if (contorno.estaReservadaRegionSoporte())
	{
		reservarRegionSoporteContorno();
		for (int i = 1; i <= numeroPuntosContorno(); i++)
		{
			regionSoporteAnteriorPunto(i, contorno.regionSoporteAnteriorPunto(i));
			regionSoportePosteriorPunto(i, contorno.regionSoportePosteriorPunto(i));
		}
	}

	/* Si ya se ha calculado el parámetro de curvatura de los puntos, se asigna*/
	if (contorno.estaReservadoParametroCurvatura())
	{
		reservarParametroCurvaturaContorno();  
		for (int i = 1; i <= numeroPuntosContorno(); i++)
		{
			parametroCurvaturaPunto(i, contorno.parametroCurvaturaPunto(i));
		}
	}

	/* Si ya se han calculado los puntos dominantes se asignan*/

	if (contorno.estaReservadoDominante())
	{
		reservarPuntoDominanteContorno();  
		for (int i = 1; i <= numeroPuntosContorno(); i++)
		{
			dominantePunto(i, contorno.dominantePunto(i));
		}
	}
}

//Sobrecarga de la asignacion.
Contorno & Contorno::operator=(const Contorno &contorno)
{
	Punto centro;

	//Asignamos numero de puntos
	numeroPuntosContorno(contorno.numeroPuntosContorno());

	//Se reserva memoria para los puntos del contorno
	reservarPuntosContorno();
	reservarCodigoCadenaContorno();

	//Inicializar a 0 todos los puntos y parametros del contorno
	inicializarContorno();

	//asignamos el punto central
	centroContorno(contorno.centroContorno());

	// Asignamos todos los puntos del contorno
	for (int i = 1; i <= numeroPuntosContorno(); i++)
	{
		puntoContorno(contorno.puntoContorno(i), i);
		codigoCadenaPunto(i, contorno.codigoCadenaPunto(i));
	}

	//Si ya se tienen regiones de soporte, se asignan
	if (contorno.estaReservadaRegionSoporte())
	{
		reservarRegionSoporteContorno();
		for (int i = 1; i <= numeroPuntosContorno(); i++)
		{
			regionSoporteAnteriorPunto(i, contorno.regionSoporteAnteriorPunto(i));
			regionSoportePosteriorPunto(i, contorno.regionSoportePosteriorPunto(i));
		}
	}

	/* Si ya se ha calculado el parámetro de curvatura de los puntos, se asigna*/
	if (contorno.estaReservadoParametroCurvatura())
	{
		reservarParametroCurvaturaContorno();  
		for (int i = 1; i <= numeroPuntosContorno(); i++)
		{
			parametroCurvaturaPunto(i, contorno.parametroCurvaturaPunto(i));
		}
	}

	/* Si ya se han calculado los puntos dominantes se asignan*/

	if (contorno.estaReservadoDominante())
	{
		reservarPuntoDominanteContorno();  
		for (int i = 1; i <= numeroPuntosContorno(); i++)
		{
			dominantePunto(i, contorno.dominantePunto(i));
		}
	}

	return *this;
}

void Contorno::inicializarContorno()
{
	Punto punto;

	/* Se inicializa a cero un punto de contorno auxiliar */
	punto.x(0.0);
	punto.y(0.0);
	punto.radio(0.0);
	punto.angulo(0.0);

	/* Se inicializa a cero el centro del contorno */
	centroContorno(punto);

	/* Se inicializan todos los puntos del contorno con el punto de contorno auxiliar */
	for (int i = 1; i <= numeroPuntosContorno(); i++)
	{
		puntoContorno(punto, i);
		tPunto(i, (double) (i - 1));
		codigoCadenaPunto(i, 0);
	}

	_parametroCurvatura = NULL;
	_regionSoporteAnterior = NULL;
	_regionSoportePosterior = NULL;
	_dominante = NULL;

}

void Contorno::reservarPuntosContorno()
{
	_puntoContorno = new Punto [numeroPuntosContorno()];	//Reserva de puntos
	if (_puntoContorno == NULL)
	{
		cout << " FALLO EN LA RESERVA DE MEMORIA \n";
		exit(0);
	}

	_t = new double [numeroPuntosContorno()];	//Reserva para el parámetro t
	if (_t == NULL)
	{
		cout << " FALLO EN LA RESERVA DE MEMORIA \n";
		exit(0);
	}

}

void Contorno::reservarCodigoCadenaContorno()
{
	_codigoCadena = new int [numeroPuntosContorno()];
	if (_codigoCadena == NULL)
	{
		cout << " FALLO EN LA RESERVA DE MEMORIA \n";
		exit(0);
	}

}

void Contorno::reservarParametroCurvaturaContorno()
{
	_parametroCurvatura = new double [numeroPuntosContorno()];
	if (_parametroCurvatura == NULL)
	{
		cout << " FALLO EN LA RESERVA DE MEMORIA \n";
		exit(0);
	}
}

void Contorno::reservarRegionSoporteContorno()
{
	_regionSoporteAnterior = new int [numeroPuntosContorno()];
	if (_regionSoporteAnterior == NULL)
	{
		cout << " FALLO EN LA RESERVA DE MEMORIA \n";
		exit(0);
	}

	_regionSoportePosterior = new int [numeroPuntosContorno()];
	if (_regionSoportePosterior == NULL)
	{
		cout << " FALLO EN LA RESERVA DE MEMORIA \n";
		exit(0);
	}
}

bool Contorno::estaReservadaRegionSoporte()const
{
	if (_regionSoporteAnterior != NULL)
		return true;
	else
		return false;
}

bool Contorno::estaReservadoParametroCurvatura()const
{
	if (_parametroCurvatura != NULL)
		return true;
	else
		return false;
}

bool Contorno::estaReservadoDominante()const
{
	if (_dominante != NULL)
		return true;
	else
		return false;
}

void Contorno::reservarPuntoDominanteContorno()
{
	_dominante = new bool [numeroPuntosContorno()];
	if (_dominante == NULL)
	{
		cout << " FALLO EN LA RESERVA DE MEMORIA \n";
		exit(0);
	}
}

double Contorno::longitudContorno()const
{
	double longitud = 0.0;
	Punto punto1, punto2;


	for(int i = 1; i < numeroPuntosContorno(); i++)
	{
		punto1 = puntoContorno(i);
		punto2 = puntoContorno(i + 1);
		Vector2D Vector2D(punto1, punto2);
		longitud += Vector2D.moduloVector2D();
	}

	/* Se calcula tambien la distancia del último al primero */
	punto1 = puntoContorno(numeroPuntosContorno());
	punto2 = puntoContorno(1);
	Vector2D Vector2D(punto1, punto2);
	longitud += Vector2D.moduloVector2D();

	return longitud;
}

double Contorno::longitudContornoEntrePuntosDominantes()const
{
	int inicial, i;
	double longitud = 0.0;
	Punto punto1, punto2;

	/*Primero localizo el punto dominante inicial */
	i = 1;

	while(!dominantePunto(i))
		i++;

	inicial = i;

	/*Voy localizando los puntos dominantes siguientes hasta llegar al último */
	punto1 = puntoContorno(inicial);

	do
	{
		/* Pasamos al siguiente punto */
		i++;
		if (i > numeroPuntosContorno())
			i = i - numeroPuntosContorno();

		/*Localizamos el siguiente dominante*/
		while(!dominantePunto(i))
		{
			i++;
			if (i > numeroPuntosContorno())
				i = i - numeroPuntosContorno();
		}

		punto2 = puntoContorno(i);
		Vector2D vector(punto1, punto2);
		longitud += vector.moduloVector2D();
		/*Ahora punto2 pasa a ser punto1*/
		punto1 = punto2;

	}while (i != inicial);

	return longitud;
}


int Contorno::contarRegistrosFicheroContorno(char *nombreFichero)const
{
	int numeroRegistros = 0;
	double x, y;

	ifstream pf(nombreFichero);
	if (pf==NULL)
	{
		cout << "\n Fichero inexistente";
		exit(0);
	}

	while(pf >> x >> y)
		numeroRegistros++;

	pf.close();
	return numeroRegistros;
}

/**************************************************************/
/**************************************************************/
/***** FUNCION: cargarContorno                            *****/
/***** Parametros de entrada: Nombre del fichero.					*****/
/***** Parametros de salida:                              *****/
/***** Objetivo: cargar en memoria un contorno guardado en*****/
/*****            un fichero, e inicializarlo.            *****/
/***** Fecha: 17-4-00                                     *****/
/***** Fecha ultima modificación: 30-12-2010              *****/
/**************************************************************/
/**************************************************************/
/**************************************************************/
/***** Observaciones: Solo se reserva memoria para        *****/
/***** aquellos datos del contorno imprescindibles, es    *****/
/***** decir, coordenadas de puntos y el codigo de cadena *****/
/***** Los demás parámetros se reservarán cuando se vayan *****/
/***** a usar, dependiendo del modelo a aplicar           *****/
/**************************************************************/
/**************************************************************/

void Contorno::cargarContorno(char *nombreFichero)
{
	int numeroRegistros;

	/* Primero se cuentan los registros del fichero */
	numeroRegistros = contarRegistrosFicheroContorno(nombreFichero);

	/* Asigna el numero de puntos del contorno */
	numeroPuntosContorno(numeroRegistros);

	/* Se reserva memoria para los puntos del contorno */
	reservarPuntosContorno();
	reservarCodigoCadenaContorno();
	//reservarPuntoDominanteContorno();

	/* Inicializar a 0 todos los puntos y parametros del contorno */
	inicializarContorno();

	/* Se leen los puntos del fichero y se almacenan en el contorno
 	tambien asigna los valores del parametro t*/
	leerPuntosFichero(nombreFichero);

	/* Se calcula el centro del contorno */
	calcularCentroContorno();

	/* Ahora se obtienen las coordenadas polares respecto al centro */
	coordenadasPolaresCentro();

	/* Obtención del código de cadena */
	codigoCadenaContorno();

	//Se ponen a NULL el resto de punteros. 

}

void Contorno::leerPuntosFichero(char *nombreFichero)
{
	double x, y;
	Punto punto;

	ifstream pf(nombreFichero);
	if (pf==NULL)
	{
		cout << "\n Fichero inexistente";
		exit(0);
	}

	//Lee del fichero los puntos y va almacenando coordenadas cartesianas, polares y paramétricas
	for (int i = 1; i <= numeroPuntosContorno(); i++)
	{
		pf >> x >> y;
		punto.x(x);
		punto.y(y);
		punto.radio(0.0);
		punto.angulo(0.0);
		puntoContorno(punto, i);
	}

	pf.close();
}

void Contorno::calcularCentroContorno()
{
	double xMedia = 0.0, yMedia = 0.0;
	Punto punto;

	for (int i = 1; i <= numeroPuntosContorno(); i++)
	{
		punto = puntoContorno(i);
		xMedia += punto.x() / (double) numeroPuntosContorno();
		yMedia += punto.y() / (double) numeroPuntosContorno();
	}

	punto.x(xMedia);
	punto.y(yMedia);

	/* Las coordenadas polares del centro seran 0,0 ya que se refieren a sí mismo */
	punto.radio(0.0);
	punto.angulo(0.0);

	centroContorno(punto); 
}


/**************************************************************/
/**************************************************************/
/***** FUNCION: guardarContorno                           *****/
/***** Parametros de entrada: contorno y nombre de fichero*****/
/***** Parametros de salida:                              *****/
/***** Objetivo: Graba los puntos en coordenadas          *****/
/*****            cartesianas.                            *****/
/***** Funciones a las que llama                          *****/
/***** Fecha: 2-1-99                                      *****/
/***** Fecha ultima modificación: 30-12-10                *****/
/**************************************************************/
/**************************************************************/

void Contorno::guardarContorno(char *nombre)const
{
	Punto punto;
	ofstream pf(nombre);

	if (pf==NULL)
	{
		cout << "Error en la apertura del fichero \n";
		exit(0);
	}

	/* Guardamos las coordenadas cartesianas del contorno */
	for(int i = 1 ;i <= numeroPuntosContorno() ; i++)
	{
		punto = puntoContorno(i);
		pf << punto.x() << " " << punto.y() << "\n";
	}

	pf.close();
}

/*Esta funcion es igual que la anterior, pero guarda el primer punto repetido, para que el programa de dibujo
cierre el contorno*/

void Contorno::guardarContornoDibujo(char *nombre)const
{
	Punto punto;
	ofstream pf(nombre);

	if (pf==NULL)
	{
		cout << "Error en la apertura del fichero \n";
		exit(0);
	}

	/* Guardamos las coordenadas cartesianas del contorno */
	for(int i = 1 ;i <= numeroPuntosContorno() ; i++)
	{
		punto = puntoContorno(i);
		pf << punto.x() << " " << punto.y() <<"\n";
	}

	punto = puntoContorno(1);
	pf << punto.x() << " " << punto.y() <<"\n";

	pf.close();
}

/** Funcion para crear el fichero pt, que se usa con el gnuplot para generar los dibujos de los contornos */
void Contorno::crearFichero_pt(char *ficheroOriginal, char *ficheroFinal)const
{
	char linea[7][100];  // Se declaran las 7 lineas del fichero
	char ficheropt[50];  // Nombre del fichero pt
	char ficheroeps[50]; //Nombre del fichero eps
	int i;

	//Inicializamos las lineas mas faciles
	strcpy(linea[1], "set terminal postscript eps");
	strcpy(linea[2], "set noxtics");
	strcpy(linea[3], "set noytics");
	strcpy(linea[4], "set key off");
	strcpy(linea[5], "unset border");

	//Asignamos parte de las líneas 0 y 7
	strcpy(linea[0], "set output '");
	strcpy(linea[6], "plot '");

	//Obtenemos el nombre del fichero pt
	i = 0;
	while(ficheroFinal[i] != '.') //Le quitamos la extensión ".txt"
	{
		ficheropt[i] = ficheroFinal[i];
		i++;
	}
	ficheropt[i] = '\0';
	strcat(ficheropt,".pt"); //Le concatenamos la extension ".pt"

	//Obtenemos el nombre del fichero eps
	i = 0;
	while(ficheroFinal[i] != '.') //Le quitamos la extensión ".txt"
	{
		ficheroeps[i] = ficheroFinal[i];
		i++;
	}
	ficheroeps[i] = '\0';
	strcat(ficheroeps,".eps"); //Le concatenamos la extension ".eps"

	//Completamos la linea 0
	strcat(linea[0],ficheroeps);
	strcat(linea[0],"'");

	//Completamos la linea 6
	strcat(linea[6],ficheroOriginal);
	strcat(linea[6],"' w l, '");
	strcat(linea[6],ficheroFinal);
	strcat(linea[6],"' w l, '");
	strcat(linea[6],ficheroFinal);
	strcat(linea[6],"' w p pt 8 ps 2");

	//Ahora guardamos el fichero
	ofstream pf(ficheropt);

	if (pf==NULL)
	{
		cout << "Error en la apertura del fichero \n";
		exit(0);
	}

	for(i = 0; i < 7; i++)
		pf << linea[i] << "\n";

	pf.close();
}


/**************************************************************/
/**************************************************************/
/***** FUNCION: crearContorno                             *****/
/***** Parametros de entrada: puntero a contorno          *****/
/***** Parametros de salida:                              *****/
/***** Objetivo: Reserva espacio e inicializa un contorno *****/
/***** Fecha: 20-1-05                                     *****/
/***** Fecha ultima modificación: 30-12-10                *****/
/**************************************************************/
/**************************************************************/
void Contorno::crearContorno()
{ 
	/* Se reserva memoria para los puntos del contorno */
	reservarPuntosContorno();
	reservarCodigoCadenaContorno();

	/* Inicializar a 0 todos los puntos y parametros del contorno */
	inicializarContorno();
}

void Contorno::liberarContorno()
{

	if (_puntoContorno != NULL)
	{
		delete [] _puntoContorno;
		delete [] _t;

		_puntoContorno = NULL;
		_t = NULL;


		if (_codigoCadena != NULL)
		{
			delete [] _codigoCadena;
			_codigoCadena = NULL;
		}


		if (_parametroCurvatura != NULL)
		{
			delete [] _parametroCurvatura;
			_parametroCurvatura = NULL;
		}

		if (_regionSoporteAnterior != NULL)
		{
			delete [] _regionSoporteAnterior;
			_regionSoporteAnterior = NULL;
		}

		if (_regionSoportePosterior != NULL)
		{
			delete [] _regionSoportePosterior;
			_regionSoportePosterior = NULL;
		}

		if (_dominante != NULL)
		{
			delete [] _dominante;
			_dominante = NULL;
		}


		//cout << "Libero Contorno \n";

	}
}

/**************************************************************/
/**************************************************************/
/***** FUNCION: coordenadasPolaresCentro                  *****/
/***** Parametros de entrada: Contorno.                   *****/
/***** Parametros de salida:                              *****/
/***** Objetivo: Convierte de coordenadas cartesianas     *****/
/*****           a polares relativas al centro            *****/
/*****           los puntos de un contorno.               *****/
/***** Funciones a las que llama                          *****/
/***** Fecha: 2-1-99                                      *****/
/***** Fecha ultima modificación: 30-12-10                *****/
/**************************************************************/
/**************************************************************/
void Contorno::coordenadasPolaresCentro()
{
	double xPunto, yPunto, xCentro, yCentro, dx, dy, radio, angulo;
	Punto punto;
	Punto centro(centroContorno());
	xCentro = centro.x();
	yCentro = centro.y();

	for(int i = 1;i <= numeroPuntosContorno(); i++)
	{
		punto = puntoContorno(i);
		xPunto = punto.x();
		yPunto = punto.y();
		dx = xPunto - xCentro;
		dy = yPunto - yCentro;
		radio = pow(pow(dx, 2.0)+pow(dy, 2.0), 0.5);
		punto.radio(radio);
		angulo = atan2(dy, dx);

		if (angulo < 0.0)
			angulo += 2.0 * M_PI;

		punto.angulo(angulo);
		puntoContorno(punto, i);
	}
}

/**************************************************************/
/**************************************************************/
/***** FUNCION: coordenadasCartesianas                    *****/
/***** Parametros de entrada: Contorno.                   *****/
/***** Parametros de salida:                              *****/
/***** Objetivo: Convierte de coordenadas polares a       *****/
/*****           cartesianas                              *****/
/*****           los puntos de un contorno.               *****/
/***** Funciones a las que llama                          *****/
/***** Fecha: 2-1-99                                      *****/
/***** Fecha ultima modificación: 30-12-10                *****/
/**************************************************************/
/**************************************************************/
void Contorno::coordenadasCartesianas()
{
	double xCentro, yCentro, xPunto, yPunto;
	Punto punto;
	Punto centro(centroContorno());

	xCentro = centro.x();
	yCentro = centro.y();

	for(int i = 1;i <= numeroPuntosContorno(); i++)
	{
		punto = puntoContorno(i);
		xPunto = xCentro + punto.radio() * cos(punto.angulo());
		yPunto = yCentro + punto.radio() * sin(punto.angulo());
		punto.x(xPunto);
		punto.y(yPunto);
		puntoContorno(punto, i);
	}
}

/**************************************************************/
/**************************************************************/
/***** FUNCION: codigoCadenaContorno                      *****/
/***** Parametros de entrada: Contorno.                   *****/
/***** Parametros de salida:                              *****/
/***** Objetivo: Obtiene el codigo de cadena de un objeto *****/
/*****           a partir de las coordenadas cartesianas  *****/
/***** Funciones a las que llama                          *****/
/***** Fecha: 27-11-2001                                  *****/
/***** Fecha ultima modificación: 31-10-2014              *****/
/**************************************************************/
/**************************************************************/
void Contorno::codigoCadenaContorno()
{
	/*Variable para recibir el codigo de cadena de un punto */ 
	int codigoCadena;

	Punto puntoActual, puntoSiguiente;

	/* Calculo del codigo de cadena */
	for(int i = 1; i <=  numeroPuntosContorno(); i++)
	{
		puntoActual = puntoContorno(i);
		if (i < numeroPuntosContorno())
			puntoSiguiente = puntoContorno(i + 1);
		else /* Ultimo punto */
		{
			//Para contornos cerrados hay que tener cuidado, el último punto coincide con el primero.
			//En ese caso el codigo de cadena del ultimo coincide con el del primero. Modificación hecha el 31-10-2014
			if (puntoContorno(numeroPuntosContorno()) == puntoContorno(1))
				puntoSiguiente = puntoContorno(2);
			else
				puntoSiguiente = puntoContorno(1);
		}
		codigoCadena = puntoActual.codigoCadenaPunto(puntoSiguiente);
		codigoCadenaPunto(i, codigoCadena);
	}
}

void Contorno::coordenadasCartesianasCodigoCadena()
{
	/*Variable para recibir el codigo de cadena de un punto */
	int codigoCadena;
	Punto puntoActual;

	puntoContorno(puntoActual, 1);

	/* Calculo del codigo de cadena */
	for(int i = 1; i <  numeroPuntosContorno(); i++)
	{
		codigoCadena = codigoCadenaPunto(i);
		if (codigoCadena == 2)
		{
			puntoActual.x(puntoActual.x() + 1);
			puntoActual.y(puntoActual.y());
		}
		else if (codigoCadena == 3)
		{
			puntoActual.x(puntoActual.x() + 1);
			puntoActual.y(puntoActual.y() - 1);
		}
		else if (codigoCadena == 4)
		{
			puntoActual.x(puntoActual.x());
			puntoActual.y(puntoActual.y() - 1);
		}
		else if (codigoCadena == 5)
		{
			puntoActual.x(puntoActual.x() - 1);
			puntoActual.y(puntoActual.y() - 1);
		}
		else if (codigoCadena == 6)
		{
			puntoActual.x(puntoActual.x() - 1);
			puntoActual.y(puntoActual.y());
		}
		else if (codigoCadena == 7)
		{
			puntoActual.x(puntoActual.x() - 1);
			puntoActual.y(puntoActual.y() + 1);
		}
		else if (codigoCadena == 0)
		{
			puntoActual.x(puntoActual.x());
			puntoActual.y(puntoActual.y() + 1);
		}
		else
		{
			puntoActual.x(puntoActual.x() + 1);
			puntoActual.y(puntoActual.y() + 1);
		}
		puntoContorno(puntoActual, i + 1);
	}
}

void Contorno::marcarTodosPuntosDominantes()
{
	// Se marcan todos los puntos como dominantes **/
	for (int t = 1; t <= numeroPuntosContorno(); t++)
	{
		dominantePunto(t, true);
	}
}

void Contorno::marcarTodosPuntosNoDominantes()
{
	// Se marcan todos los puntos como no dominantes **/
	for (int t = 1; t <= numeroPuntosContorno(); t++)
	{
		dominantePunto(t, false);
	}
}

//Esta función marca como dominantes en el contorno, los puntos que aparecen en el auxiliar.
//En caso de que algún punto del auxiliar no esté en el contorno original saltará el aserto
void Contorno::marcarPuntosDominantesDesdeContornoAuxiliar(Contorno &auxiliar)
{
	bool encontrado = false;

	for(int i = 1 ; i <= auxiliar.numeroPuntosContorno(); i++)
	{
		encontrado = false;
		for(int j = 1 ; j <= numeroPuntosContorno() && not encontrado; j++)
		{
			//Ha encontrado el punto del contorno auxiliar en el original
			if (auxiliar.puntoContorno(i) == puntoContorno(j))
			{
				dominantePunto(j, true);
				encontrado = true;
			}
		}
		assert(encontrado);
	}
}


int Contorno::contarPuntosDominantes()const
{
	int numeroPuntosDominantes = 0;

	for (int t = 1; t <= numeroPuntosContorno(); t++)
	{
		if (dominantePunto(t))
		{
			numeroPuntosDominantes++;
		}
	}

	return numeroPuntosDominantes;
}

/* Esta funcion obtiene, mediante un Vector2D de frecuencias, que puntos del contorno son dominantes. Sera llamada multiples veces, haciendo variar el numero de puntos del contorno optimo (Perez y Vidal) desde 3 hasta el numero de break points */
void Contorno::frecuenciaPuntosDominantes(vector <int> &frecuencia)const
{
	for(int i = 1; i <= numeroPuntosContorno(); i++)
	{
		if (dominantePunto(i))
		{
			//printf("El punto %d ha aumentado su frecuencia\n", i);
			frecuencia[i]++;
		}
	}
}

/* Esta funcion guarda los puntos dominantes para un contorno optimo de i puntos en la fila f-ésima de la matriz*/
void Contorno::guardarPuntosDominantes(Matriz <int> &matrizPuntosDominantes, const int &f)const
{
	for(int i = 1; i <= numeroPuntosContorno(); i++)
	{
		if (dominantePunto(i))
			matrizPuntosDominantes.elemento(f, i, 1);
	}
}

void Contorno::guardarPuntosDominantes(vector <int> &vectorPuntosDominantes)const
{
	for(int i = 1; i <= numeroPuntosContorno(); i++)
	{
		if (dominantePunto(i))
			vectorPuntosDominantes[i] = 1;
		else
			vectorPuntosDominantes[i] = 0;
	}
}

void Contorno::diferenciaContornoDominantesReales(double &sumaErrorCuadrado, double &errorMaximo)const
{
	int i, t = 1;
	int tInicial;
	int tIzquierda, tDerecha;
	double error;

	/* Recorrido del contorno, calculando la distancia de cada punto real del contorno
    al contorno aproximado por los puntosdominantes */
	sumaErrorCuadrado = 0.0;
	errorMaximo = 0.0;

	/*Busqueda del primer punto dominante del contorno*/
	while(!dominantePunto(t))
		t++;

	tInicial = t;

	/* se asigna el extremo izquierdo del intervalo a tInicial para los primeros puntos */
	tIzquierda = t;
	do
	{
		t++;
		if (t > numeroPuntosContorno())
			t = t - numeroPuntosContorno();

		while (!dominantePunto(t)) /* Se localiza extremo derecho del intervalo */
		{
			t++;
			if (t > numeroPuntosContorno())
				t = t - numeroPuntosContorno();
		}

		tDerecha = t;

		/* Se calculan las distancias de todos los puntos comprendidos entre tIzquierda y tDerecha hasta
     el segmento que une los puntos tIzquierda y tDerecha*/

		Recta recta(puntoContorno(tIzquierda), puntoContorno(tDerecha));

		i = tIzquierda + 1;
		if (i > numeroPuntosContorno())
			i = i - numeroPuntosContorno();

		while ( i != tDerecha)
		{
			error = recta.distanciaPuntoRecta(puntoContorno(i));
			sumaErrorCuadrado += error * error;

			if (error > errorMaximo)
				errorMaximo = error;

			i++;
			if (i > numeroPuntosContorno())
				i = i - numeroPuntosContorno();
		}

		tIzquierda = tDerecha; /*Para el siguiente intervalo tDerecha sera tIzquierda */
	} while (tIzquierda != tInicial);
}

//Esta función se incorpora en julio de 2016.
//Calcula el ISE en el segmento entre inicial y final
double Contorno::calcularIseEntreDospuntos(int inicial, int final) const
{
	assert(inicial >= 1 and inicial <= numeroPuntosContorno());
	assert(final >= 1 and final <= numeroPuntosContorno());

	double ise = 0.0;
	double error = 0.0;
	int begin, end;

	if (inicial == final)
		return 0.0;

	Recta recta(puntoContorno(inicial), puntoContorno(final));

	if (inicial < final)
	{
		begin = inicial + 1;
		end = final - 1;
		for (int i = begin; i <= end; i++)
		{
			error = recta.distanciaPuntoRecta(puntoContorno(i));
			ise += error * error;
		}
	}
	else 
	{
		begin = inicial + 1;
		if (begin > numeroPuntosContorno())
			begin = 1;

		end = final - 1;
		if (end < 1)
			end = numeroPuntosContorno();
		for (int i = begin; i <= numeroPuntosContorno(); i++)
		{
			error = recta.distanciaPuntoRecta(puntoContorno(i));
			ise += error * error;
		}
		for (int i = 1; i <= end; i++)
		{
			error = recta.distanciaPuntoRecta(puntoContorno(i));
			ise += error * error;
		}
	}

	return ise;	
}


//Esta función se incorpora en julio de 2016.
//Calcula el Emax en el segmento entre inicial y final
void Contorno::calcularEmaxEntreDosPuntos(int inicial, int final, double &errorMaximo, int &posicion) const
{
	assert (inicial != final);
	assert(inicial >= 1 and inicial <= numeroPuntosContorno());
	assert(final >= 1 and final <= numeroPuntosContorno());

	errorMaximo = 0.0;
	posicion = inicial;
	double error = 0.0;
	int begin, end;

	Recta recta(puntoContorno(inicial), puntoContorno(final));

	if (inicial < final)
	{
		begin = inicial + 1;
		end = final - 1;
		for (int i = begin; i <= end; i++)
		{
			error = recta.distanciaPuntoRecta(puntoContorno(i));
			if (error > errorMaximo)
			{
				errorMaximo = error;
				posicion = i;
			}
		}
	}
	else if (inicial > final)
	{
		begin = inicial + 1;
		if (begin > numeroPuntosContorno())
			begin = 1;

		end = final - 1;
		if (end < 1)
			end = numeroPuntosContorno();
		for (int i = begin; i <= numeroPuntosContorno(); i++)
		{
			error = recta.distanciaPuntoRecta(puntoContorno(i));
			if (error > errorMaximo)
			{
				errorMaximo = error;
				posicion = i;
			}
		}
		for (int i = 1; i <= end; i++)
		{
			error = recta.distanciaPuntoRecta(puntoContorno(i));
			if (error > errorMaximo)
			{
				errorMaximo = error;
				posicion = i;
			}
		}
	}
}

void Contorno::copiarPuntosDominantes(const Contorno &contorno)
{
	int contadorDominantes = 0;

	for (int t = 1; t <= contorno.numeroPuntosContorno(); t++)
	{
		if (contorno.dominantePunto(t) == true)
			// Si el punto es dominante se traspasa al contorno de puntos dominantes
		{
			contadorDominantes++;
			puntoContorno(contorno.puntoContorno(t), contadorDominantes);
			//printf("t = %d, x = %lf, y = %lf\n", t, puntoContorno(contadorDominantes).x(), puntoContorno(contadorDominantes).y()); 
		}
	}
	printf("Habia %d puntos dominantes\n", contadorDominantes);
}

int Contorno::iesimoUltimoDominante(const int &iteracion)
{
	int ultimo;
	int contador = 0;

	ultimo = numeroPuntosContorno();

	for (int i = ultimo; i >= 1; i--)
	{
		if (dominantePunto(i))
		{
			contador++;
			if (contador == iteracion)
				return i;
		}
	}

	return ultimo;
}

void Contorno::mostrarPuntosDominantes()
{
	Punto p;

	for (int t = 1; t <= numeroPuntosContorno(); t++)
	{
		if (dominantePunto(t) == 1)
		{
			p = puntoContorno(t);
			cout << "El punto " << t << " de coordenadas x = " << p.x() << " y = " << p.y() <<" \n";
		}
	}
}

void Contorno::cambiarPuntoInicial(Contorno &antiguo, const int inicial)
{
	//El punto 1 del nuevo contorno sera el marcado como inicial del antiguo
	puntoContorno(antiguo.puntoContorno(inicial), 1);

	//El punto ultimo coincide con el inicial para cerrar el contorno
	puntoContorno(antiguo.puntoContorno(inicial), numeroPuntosContorno());

	for (int t = inicial + 1; t < numeroPuntosContorno(); t++)
		puntoContorno(antiguo.puntoContorno(t), t - inicial + 1);

	for(int t = 1; t < inicial ; t++)
		puntoContorno(antiguo.puntoContorno(t), numeroPuntosContorno() - inicial + t);

	//Codigo antiguo y erroneo
	/*
    if (t >= inicial)
			puntoContorno(antiguo.puntoContorno(t), t - inicial + 1);
		else
			puntoContorno(antiguo.puntoContorno(t), numeroPuntosContorno() - inicial + 1 + t);*/
}

int Contorno::contarBreakPoints()
{
	Contorno aux = *this;
	aux.reservarPuntoDominanteContorno();
	aux.marcarTodosPuntosDominantes();
	int numeroPuntos = aux.numeroPuntosContorno();
	eliminarAlineados(aux, numeroPuntos, 1);

	return aux.contarPuntosDominantes();
}

void Contorno::copiarPuntosDominantesDesdeMatriz(const Contorno &contorno,  const Matriz <int> &matrizContornosOptimos, 
		int numeroPuntosAproximacion)
{
	int contadorDominantes = 0;

	for (int t = 1; t <= contorno.numeroPuntosContorno(); t++)
	{
		if (matrizContornosOptimos.elemento(numeroPuntosAproximacion, t) == 1)
			// Si el punto es dominante se traspasa al contorno de puntos dominantes
		{
			contadorDominantes++;
			puntoContorno(contorno.puntoContorno(t), contadorDominantes);
		}
	}
	printf("Habia %d puntos dominantes\n", contadorDominantes);
}

void Contorno::copiarPuntosDominantesDesdeVector(const Contorno &contorno,  const vector <int> &vectorContornosOptimos, int numeroPuntosAproximacion)
{
	int contadorDominantes = 0;

	for (int t = 1; t <= contorno.numeroPuntosContorno(); t++)
	{
		if (vectorContornosOptimos[t] == 1)
			// Si el punto es dominante se traspasa al contorno de puntos dominantes
		{
			contadorDominantes++;
			puntoContorno(contorno.puntoContorno(t), contadorDominantes);
		}
	}
	printf("Habia %d puntos dominantes\n", contadorDominantes);
}
