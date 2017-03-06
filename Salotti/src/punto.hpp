#ifndef PUNTO_H
#define PUNTO_H

#include <math.h>
/* Estructura de punto en 2D con coordenadas cartesianas y polares */
class Punto
{
public:
	Punto()
{
		_x = 0.0;
		_y = 0.0;
		_radio = 0.0;
		_angulo = 0.0;
};
	Punto(double x, double y)
	{
		_x = x;
		_y = y;
		_radio = 0.0;
		_angulo = 0.0;
	};

	Punto(const Punto &p)
	{
		x(p.x());
		y(p.y());
		radio(p.radio());
		angulo(p.angulo());
	};

	Punto & operator=(const Punto &p)
	{
		x(p.x());
		y(p.y());
		radio(p.radio());
		angulo(p.angulo());

		return *this;
	};

	bool operator==(const Punto &p) const
    		{
		if (fabs(x()- p.x()) < 0.0000001 && fabs(y() - p.y()) < 0.0000001)
			return true;
		return false;
    		};

	bool operator!=(const Punto &p) const
				{
		if (fabs(x()- p.x()) < 0.0000001 && fabs(y() - p.y()) < 0.0000001)
			return false;
		return true;
				};


	inline double x()const //Observador
			{
		return _x;
			};

	inline double y()const //Observador
	{
		return _y;
	};

	inline double radio()const //Observador
	{
		return _radio;
	};

	inline double angulo()const //Observador
	{
		return _angulo;
	};

	inline void x(const double &x) //Modificador
	{
		_x = x;
	};

	inline void y(const double &y) //Modificador
	{
		_y = y;
	};

	inline void radio(const double &radio) //Modificador
	{
		_radio = radio;
	};

	inline void angulo(const double &angulo) //Modificador
	{
		_angulo = angulo;
	};

	double distanciaPuntos(const Punto &p)const;
	int codigoCadenaPunto(const Punto &p)const;

private:
	double _x; /* Coordenada cartesina x del punto */
	double _y; /* Coordenada cartesiana y del punto */
	double _radio; /* Coordenada polar radio del punto */
	double _angulo; /* Coordenada polar angulo del punto */
};


#endif
