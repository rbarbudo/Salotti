#ifndef __NODO_HPP__
#define __NODO_HPP__
#include "punto.hpp"

class Nodo
{
private:
	Punto _punto;
	int _indice;
	int _rango;
	int _padre;
	double _coste;

public:
	Nodo()
{
		Punto p(0,0);
		setPunto(p);
		setIndice(0);
		setRango(0);
		setPadre(0);
		setCoste(0.0);
}

	Nodo(const Punto punto, const int indice, const int rango, const int padre, const double coste)
	{
		setPunto(punto);
		setIndice(indice);
		setRango(rango);
		setPadre(padre);
		setCoste(coste);
	}

	~Nodo()
	{
	}

	Nodo & operator=(const Nodo &n)
	{
		if (this != &n)
		{
			setPunto(n.getPunto());
			setIndice(n.getIndice());
			setRango(n.getRango());
			setPadre(n.getPadre());
			setCoste(n.getCoste());
		}
		return *this;
	}

	//Dos nodos serán iguales cuando se correspondan con el mismo punto y tengan el mismo rango
	bool operator==(const Nodo &n) const
	{
		return (getIndice() == n.getIndice() && getRango() == n.getRango());
	}

	//Sobrecarga del operador < para la ordenación.
	bool operator<(const Nodo &n) const
	{
		double aux = getCoste() - n.getCoste();
		return (/*aux < 0.0*/aux < 0.000000001);
	}

	//Sobrecarga del operador < para la ordenación.
	bool operator>=(const Nodo &n) const
						{
		double aux = getCoste() - n.getCoste();
		return not (/*aux < 0.0*/aux < 0.000000001);

						}

	const Punto &getPunto() const
	{
		return _punto;
	}

	const int &getIndice() const
	{
		return _indice;
	}

	const int &getRango() const
	{
		return _rango;
	}

	const int &getPadre() const
	{
		return _padre;
	}

	const double &getCoste() const
	{
		return _coste;
	}

	void setPunto(const Punto punto)
	{
		_punto = punto;
	}

	void setIndice(const int indice)
	{
		_indice = indice;
	}

	void setRango(const int rango)
	{
		_rango = rango;
	}

	void setPadre(const int padre)
	{
		_padre = padre;
	}

	void setCoste(const double coste)
	{
		_coste = coste;
	}
};

#endif
