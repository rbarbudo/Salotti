#ifndef __HEAP_VECTOR_HPP__
#define __HEAP_VECTOR_HPP__

#include <iostream>
#include <cassert>
#include <vector> //Para usar vector de la stl
#include "nodo.hpp"

using namespace std;

//En esta cola de prioridad el elemento mínimo es el que está en cabeza.
//En la implementación, el elemento de posición 0 estará vacío y se empezarán
// a meter elementos a partir de la posición 1.

//Está basada en la implementación que hacen en http://algorithmics.lsi.upc.edu/docs/pqueues.pdf

class HeapVector
{
public:
	HeapVector ()
{
		//El constructor crea un nodo vacio en la posicion 0
		//para empezar a usar la posicion desde 1 hasta _nElementos

		Nodo aux;
		_h.push_back(aux);
		_nElementos = 0;
}

	HeapVector (const HeapVector& a)
	{
		if (this != &a)
		{
			_nElementos = a._nElementos;
			_h = a._h;
		}
	}

	~HeapVector ()
	{
		borrarHeap();
	}

	bool insertar(const Nodo &x, int ** &posicionNodo)
	{
		++_nElementos;
		//Colocamos el nodo al final del heap
		_h.push_back(x);

		//Actualizamos el vector de posicion del nodo dentro del heap
		posicionNodo[x.getIndice()][x.getRango()] = _nElementos;

		//Flotamos el nodo nuevo para llevarlo a su posición correcta dentro del heap
		flotar(_nElementos, posicionNodo);
		return true;
	}

	bool actualizar(const Nodo &antiguo, const Nodo &nuevo, int ** &posicionNodo)
	{
		/*for(int i = 1; i <= _nElementos; i++)
				{
					if (_h[i] == antiguo) //Se ha encontrado el nodo
					{
						_h[i] = nuevo; //Se actualiza el nodo
						flotar(i); //Se actualiza el orden del heap
						return true;
					}
				}
				return false;
		 */
		//Localizamos la posicion del nodo antiguo
		int posicionActual = posicionNodo[antiguo.getIndice()][antiguo.getRango()];

		//Actualizamos el nodo antigo
		_h[posicionActual] = nuevo;

		//Se actualiza el orden del heap
		flotar(posicionActual, posicionNodo);

		return true;
	}

	bool borrarMinimo(int ** &posicionNodo)
	{
		assert(not estaVacio());
		//Intercambia el elemento de posicion 1 con el ultimo
		swap(_h[1], _h[_nElementos]);

		posicionNodo[_h[1].getIndice()][_h[1].getRango()] = 1;

		//Borra el último (esto lo he añadido yo)
		_h.erase(_h.begin() + _nElementos);

		//Disminuye el número de elementos
		--_nElementos;

		//Hunde el elemento 1
		hundir(1, posicionNodo);

		return true;
	}

	void borrarHeap()
	{
		if (_nElementos > 0)
		{
			_nElementos = 0;
			_h.clear();
		}
	}

	const Nodo & minimo()
	{
		assert(not estaVacio());
		return _h[1];
	}

	const double & costeMinimo()
	{
		assert(not estaVacio());
		return _h[1].getCoste();
	}

	bool estaVacio() const
	{
		return _nElementos == 0;
	}


private:
	vector <Nodo> _h;

	int _nElementos;
	void flotar(int j, int ** &posicionNodo)
	{
		//si j es la raiz, se ha acabado
		if (j == 1)
			return;

		int padre = j / 2;

		//Si el padre tiene mayor prioridad que j, intercambiar y seguir flotando
		if ( _h[j] < _h[padre])
		{
			//Intercambiamos posiciones en matriz de posiciones de nodo
			posicionNodo[_h[j].getIndice()][_h[j].getRango()] = padre;
			posicionNodo[_h[padre].getIndice()][_h[padre].getRango()] = j;

			//intercambiamos nodos en el heap
			swap(_h[j], _h[padre]);

			//Volvemos a flotar
			flotar(padre, posicionNodo);
		}
	}

	void hundir(int j, int ** &posicionNodo)
	{
		if (2 * j > _nElementos) return;

		int minimoHijo = 2 * j;

		//Se busca el menor de los dos hijos, que será el 2j o el 2j + 1
		if (minimoHijo < _nElementos && _h[minimoHijo + 1] < _h[minimoHijo])
			++minimoHijo;

		//si  j es mayor  que su menor hijo se intercambia y se sigue hundiendo
		if (_h[minimoHijo] < _h[j])
		{
			//Intercambiamos posiciones en matriz de posiciones de nodo
			posicionNodo[_h[j].getIndice()][_h[j].getRango()] = minimoHijo;
			posicionNodo[_h[minimoHijo].getIndice()][_h[minimoHijo].getRango()] = j;

			//Intercambiamos nodos en el heap
			swap(_h[j], _h[minimoHijo]);

			//Seguimos hundiendo
			hundir(minimoHijo, posicionNodo);
		}
	}
};

#endif
