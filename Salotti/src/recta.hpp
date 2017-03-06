#ifndef RECTA_H
#define RECTA_H
#include "punto.hpp"

/* La recta se va a representar en forma general ax + by + c = 0 */
class Recta
{
public:
	Recta(const Punto &p1, const Punto &p2);
	Recta(const Recta &r); //Constructor de copia
	Recta & operator=(const Recta &r); //Sobrecarga de la asignacion
	inline double a()const //Observador
	{
		return _a;
	};

	inline double b()const //Observador
	{
		return _b;
	};

	inline double c()const //Observador
	{
		return _c;
	};

	inline void a(const double &a) //Modificador
	{
		_a = a;
	};

	inline void b(const double &b) //Modificador
	{
		_b = b;
	};

	inline void c(const double &c) //Modificador
	{
		_c = c;
	};

	double distanciaPuntoRecta(const Punto &punto)const;
	int signoPuntoRecta(const Punto &punto)const;

private:
	double _a;
	double _b;
	double _c;
};

#endif
