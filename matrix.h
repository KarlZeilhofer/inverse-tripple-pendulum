/*
  Einfache Matrizzen Klasse
  Für Berechnungen in der linearen Algebra
  Karl Zeilhofer

	18.6.2011:
		1. Alpha-Version
			Alle grundrechnungsarten implementiert
			Matrix-Invertierung funktioniert bereits auch
			Transponierung geht auch

	19.6.2011
		Copy Construktor hinzugefügt, für tiefe speicher kopie
		Copy Assignment Operator implementiert

	20.6.2011
		Um dem Speicherleck auf die Spur zu kommen, wurden alle
			Methoden auf Pass und Return by Value geändert. Brachte leider keine Verbesserung.
		Zugriff durch nur einen Index für den ()-Operator
			zB A(1)=5;
			bei Vektoren kann somit ein falsches Indizieren vermieden werden.
			Eine Matrix wird mit nur einem Index Spaltenweise durchwandert

	25.6.2011
		Implementierung folgender Methoden:
			int rows() const {return _rows;}
			int columns() const {return _columns;}
			static Matrix ident(int n);
			Matrix toColumnVector();
			Matrix toRowVector();
			Matrix init(int numberOfElements, ...);


   TODOs:
		Geschickte initialisierung für Vektoren und Matrizzen
			evt. mit variabler Argumentenliste
		Punktweises Multiplizieren und Dividieren muss noch implementiert werden.
		Fehlerbehandlung
		Implementierung der Grundfunktionen
			det, rank, eigenvalues, eigenvectors
			rand
			floor, ceil, round
			min, max
			sin, cos, tan, exp, pow, sum, mean, std,
			size, length, selection like in matlab
			datei ein und ausgabe
		Dynamisches vergrößern der matrix

		Speicherleck stopfen!
		Exceptions einführen
			Evt. eine eigene Log-Datei anlegen.
		Verbesserte Ausgabe
*/

#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <QString>

class Matrix
{
private:
	double ** data;
	int _rows;
	int _columns;

	static double activeInstances;

public:
	int getActiveInstances();
    Matrix();
	Matrix(int _rows, int _columns);
	Matrix(double start, double inc, double end); // generate linear vector

	Matrix(const Matrix& copy); // copy contructor for deep copy
	~Matrix();

	Matrix operator= (const Matrix source); // copy assignment operator, for deep copy!

	friend std::ostream& operator <<(std::ostream &os,const Matrix &obj); // printing a matrix
	//void print();

	double& operator() (int i, int j); // write access to an element
	double operator() (int i, int j) const; // read access to an element
	double& operator() (int i); // write access to an element
	double operator() (int i) const; // read access to an element

	friend Matrix operator+(const Matrix M1, const Matrix M2);
	friend Matrix operator+(const Matrix M1, const double a);
	friend Matrix operator+(const double a, const Matrix M2);
	friend Matrix operator-(const Matrix M1, const Matrix M2);
	friend Matrix operator-(const Matrix M1, const double a);
	friend Matrix operator-(const double a, const Matrix M2);
	friend Matrix operator*(const Matrix M1, const Matrix M2);
	friend Matrix operator*(const Matrix M1, const double a);
	friend Matrix operator*(const double a, const Matrix M2);
	Matrix dotMul(const Matrix M2);
	static Matrix dotMul(const Matrix M1, const Matrix M2);
	friend Matrix operator/(const Matrix M1, const double a);
	friend Matrix operator/(const double a, const Matrix M2);

	Matrix T(); // return a new transposed matrix
	Matrix inv(); // return a new inverted matrix
	Matrix setTo(double v); // set all elements to v
	int rows() const {return _rows;}
	int columns() const {return _columns;}
	static Matrix ident(int n);
	Matrix toColumnVector();
	Matrix toRowVector();
	//Matrix toMatrix(int rows);
	Matrix init(int numberOfElements, ...);

	QString toString();
};

#endif // MATRIX_H
