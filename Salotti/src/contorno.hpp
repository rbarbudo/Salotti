#ifndef CONTORNO_H
#define CONTORNO_H

#include "punto.hpp"
#include "matriz.hpp"
#include <vector>    // necesario para el contenedor Vector2D
#include <algorithm>

using namespace std; //Para usar Vector de la STL

/* Estructura del contorno, que contendran puntos */
class Contorno
{
public:
	Contorno(); //Es necesario en algunos casos.
	Contorno(const int &n);
	Contorno(char *nombreFichero);
	Contorno(const Contorno &contorno); //Constructor de copia
	~Contorno();
	Contorno & operator=(const Contorno &contorno); //Sobrecarga de la asignacion
	inline int numeroPuntosContorno()const //Observador
	{
		return _numeroPuntos;
	}
	inline Punto centroContorno()const //Observador
	{
		return _centro;
	}

	inline void centroContorno(Punto p)  //Modificador
	{
		_centro = p;
	}

	inline void numeroPuntosContorno(const int &n) //Modificador
	{
		_numeroPuntos = n;
	}

	inline Punto puntoContorno(const int &posicion)const //Observador
	{
		return _puntoContorno[posicion - 1];
	}

	inline void puntoContorno(const Punto &p, const int &posicion) //Modificador
	{
		_puntoContorno[posicion - 1] = p;
	}

	inline double tPunto(const int &posicion)const  //Observador
	{
		return _t[posicion - 1];
	}

	inline void tPunto(const int &posicion, const double &t) //Modificador
	{
		_t[posicion - 1] = t;
	}

	inline int codigoCadenaPunto(const int &posicion)const //Observador
	{
		return _codigoCadena[posicion - 1];
	}

	inline void codigoCadenaPunto(const int &posicion, const int &codigo)  //Modificador
	{
		_codigoCadena[posicion - 1] = codigo;
	}

	inline bool dominantePunto(const int &posicion)const //Observador
	{
		return _dominante[posicion - 1];
	}

	inline void dominantePunto(const int &posicion, const bool &dominante)	//Modificador
	{
		_dominante[posicion - 1] = dominante;
	}

	inline int regionSoporteAnteriorPunto(const int &posicion)const	//Observador
	{
		return _regionSoporteAnterior[posicion - 1];
	}

	inline void regionSoporteAnteriorPunto(const int &posicion, const int &region)	//Modificador
	{
		_regionSoporteAnterior[posicion - 1] = region;
	}

	inline int regionSoportePosteriorPunto(const int &posicion)const	//Observador
	{
		return _regionSoportePosterior[posicion - 1];
	}

	inline void regionSoportePosteriorPunto(const int &posicion, const int &region)	//Modificador
	{
		_regionSoportePosterior[posicion - 1] = region;
	}

	inline double parametroCurvaturaPunto(const int &posicion)const	//Observador
	{
		return _parametroCurvatura[posicion - 1];
	}

	inline void parametroCurvaturaPunto(const int &posicion, const double &curvatura) //Modificador
	{
		_parametroCurvatura[posicion -1] = curvatura;
	}

	void reservarPuntosContorno();
	void reservarCodigoCadenaContorno();
	void reservarParametroCurvaturaContorno();
	void reservarRegionSoporteContorno();
	bool estaReservadaRegionSoporte()const;
	bool estaReservadoParametroCurvatura()const;
	bool estaReservadoDominante()const;
	void reservarPuntoDominanteContorno();
	double longitudContorno()const;
	double longitudContornoEntrePuntosDominantes()const;
	int contarRegistrosFicheroContorno(char *nombreFichero)const;
	void inicializarContorno();
	void leerPuntosFichero(char *nombreFichero);
	void calcularCentroContorno();
	void guardarContorno(char *nombre)const;
	void guardarContornoDibujo(char *nombre)const;
	void crearFichero_pt(char *ficheroOriginal, char *ficheroFinal)const;
	void cargarContorno(char *nombre);
	void crearContorno();
	void liberarContorno();
	void coordenadasPolaresCentro();
	void codigoCadenaContorno();
	void coordenadasCartesianas();
	void coordenadasCartesianasCodigoCadena();
	void marcarTodosPuntosDominantes();
	void marcarTodosPuntosNoDominantes();
	void marcarPuntosDominantesDesdeContornoAuxiliar(Contorno &auxiliar);
	int contarPuntosDominantes()const;
	void diferenciaContornoDominantesReales(double &sumaErrorCuadrado, double &errorMaximo)const;
	double calcularIseEntreDospuntos(int inicial, int final)const;
	void  calcularEmaxEntreDosPuntos(int inicial, int final, double &errorMaximo, int &posicion)const;
	void copiarPuntosDominantes(const Contorno &contorno);
	int iesimoUltimoDominante(const int &iteracion);
	void mostrarPuntosDominantes();
	void guardarPuntosDominantes(Matriz <int> &matrizPuntosDominantes, const int &f)const;
	void guardarPuntosDominantes(vector <int> &vectorPuntosDominantes)const;
	void frecuenciaPuntosDominantes(vector <int> &frecuencia)const;
	void cambiarPuntoInicial(Contorno &antiguo, const int inicial);
	int contarBreakPoints();
	void copiarPuntosDominantesDesdeMatriz(const Contorno &contorno,  const Matriz <int> &matrizContornosOptimos, int numeroPuntosAproximacion);
	void copiarPuntosDominantesDesdeVector(const Contorno &contorno,  const vector <int> &vectorContornosOptimos, int numeroPuntosAproximacion);


private:
	int _numeroPuntos; /* Numero depuntos del contorno */
	Punto _centro; /* Centro de gravedad */
	Punto *_puntoContorno; /* Vector para los puntos del contorno */
	double *_t; /* Almacena coordenada t parametrica del punto */
	int *_codigoCadena; /* Almacena codigo de cadena del punto */
	bool *_dominante; /* Valor booleano que indica si el punto es dominante (1) o no (0) */
	int *_regionSoporteAnterior; /* Almacena region de soporte izquierda
		del punto*/
	int *_regionSoportePosterior; /* Almacena region de soporte derecha
		del punto */
	double *_parametroCurvatura; /* almacena el parametro de curvatura del punto*/

};

#endif
