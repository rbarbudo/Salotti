#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <string>
#include <cstring>
#include "operacionescadenas.hpp"


//Esta funcion quita la extension a una cadena (Normalmente .txt).
void quitarExtensionCadena(const char *cadenaOriginal, char *cadenaNueva)
{
	int k = 0;

	while(cadenaOriginal[k] != '.') //Le quitamos la extensión ".txt"
	{
		cadenaNueva[k] = cadenaOriginal[k];
		k++;
	}
	cadenaNueva[k] = '\0';
}


void crearFichero_pt(const char *ficheroOriginal, char *ficheroFinal)
{
	char linea[7][100];  // Se declaran las 7 lineas del fichero
	char ficheropt[50];  // Nombre del fichero pt
	char ficheroeps[50]; //Nombre del fichero eps
	int i;
	FILE *pF;

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
	pF = fopen(ficheropt,"w");

	for(i = 0; i < 7; i++)
		fprintf(pF,"%s\n", linea[i]);

	fclose(pF);
}
