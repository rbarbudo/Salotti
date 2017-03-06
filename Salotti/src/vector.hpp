#ifndef Vector2D_H
#define Vector2D_H
#include "punto.hpp"

class Vector2D
{
public:
	Vector2D();
	Vector2D(const Punto &origen, const Punto &destino);
	Vector2D(const Vector2D &v);
	inline double x()const //Observador
	{
		return _x;
	};

	inline double y()const //Observador
	{
		return _y;
	};

	inline void x(const double &valor) //Modificador
	{
		_x = valor;
	};

	inline void y(const double &valor) //Modificador
	{
		_y = valor;
	};

	Vector2D vectorUnitario()const;
	bool vectoresIguales(const Vector2D &v2);
	Vector2D & operator=(const Vector2D &v2);
	bool operator==(const Vector2D &v2);
	Vector2D operator+(const Vector2D &sumando2);
	double moduloVector2D()const;

private:
	double _x;
	double _y;
};

#endif
