#ifndef ALGORITMO_H
#define ALGORITMO_H

#include <vector>
#include "contorno.hpp"
#include "matriz.hpp"

class Algoritmo
{
public:
	Algoritmo(){};
	virtual ~Algoritmo()
	{
		//Liberamos el contorno privado
		contorno().liberarContorno();
	};

	inline Contorno & contorno() //Observador
	{
		return _contorno;
	};

	inline void contorno(const Contorno &contorno) //Modificador
	{
		_contorno = contorno;
	};

	//Función virtual pura que se implementará en las clases derivadas
	virtual void aplicar()=0;

	//Funciones virtuales que se implementarán en algunas clases derivadas
	vector <double> & vectorErrores();
	Matriz <int> & matrizContornosOptimos();

private:
	Contorno _contorno;
};

#endif
