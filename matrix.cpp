#include "matrix.h"
#include <string.h>
#include <cmath>
#include <stdlib.h>

double Matrix::activeInstances=0;
int Matrix::getActiveInstances()
{
	return activeInstances;
}

Matrix::~Matrix()
{
	// free memory
	if(data)
	{
		delete [] data[0];
	}
	delete [] data;
	data = NULL;
	activeInstances-=1;
}

Matrix::Matrix()
{
	_rows=0;
	_columns=0;
	activeInstances++;
}

Matrix::Matrix(int rows, int columns)
{
	this->_rows = rows;
	this->_columns = columns;
	data = new double*[rows];
	double *temp = new double[rows*columns];
	for(int i=0; i<rows; i++){
		data[i]=temp+i*columns;
	}
	activeInstances++;
}


// create linear vector
// TODO: check for increasing from start to end
// check for non-zero inc
Matrix::Matrix(double start, double inc, double end)
{
	int n = (end-start)/inc + 1;
	data = new double*;
	data[0]=new double[n];

	_rows = 1;
	_columns = n;

	double a=start;
	for(int i=0; i<n; i++){
		data[0][i]=a;
		a += inc;
	}
	activeInstances++;
}

 // deep copy contructor
Matrix::Matrix(const Matrix& source)
	:_rows(source._rows), _columns(source._columns)
{
	data = new double*[_rows];
	double *temp = new double[_rows*_columns];
	for(int i=0; i<_rows; i++){
		data[i]=temp+i*_columns;
	}
	std::copy(source.data[0], &(source.data[0][_rows*_columns]), data[0]); // deep copy
	activeInstances++;
}

// copy assign operator, with deep copy
Matrix Matrix::operator= (const Matrix source)
{
	if(this != &source){ // check if it is the same object
		_rows = source._rows;
		_columns = source._columns;
		data = new double*[_rows];
		double *temp = new double[_rows*_columns];
		for(int i=0; i<_rows; i++){
			data[i]=temp+i*_columns;
		}
		std::copy(source.data[0], &(source.data[0][_rows*_columns]), data[0]); // deep copy
	}
	activeInstances++;
	return *this;
}

// the stream operator must be declared global!
// this method does not belong to the Matrix class
// it is just a friend of it.
std::ostream& operator <<(std::ostream &os,const Matrix &M)
{
	for(int i=0; i<M._rows; i++){
		for(int j=0; j<M._columns; j++){
			os << M.data[i][j] << "\t";
		}
		os << std::endl;
	}
	return os;
}

double& Matrix::operator () (int i, int j)
{
	if(i<_rows && j<_columns){
		double& r = data[i][j];
		return r;
	}
	else{
		// ERROR: out of bounds
		return *(new double);
	}
}

double Matrix::operator () (int i, int j) const
{
	return data[i][j];
}

double& Matrix::operator () (int m)
{
	int i,j;
	j=m/_rows;
	i=m%_rows;
	if(i<_rows && j<_columns){
		double& r = data[i][j];
		return r;
	}
	else{
		// ERROR: out of bounds
		return *(new double);
	}
}

double Matrix::operator () (int m) const
{
	int i,j;
	j=m/_rows;
	i=m%_rows;
	return data[i][j];
}


Matrix Matrix::T() // transpose
{
	Matrix dest(_columns, _rows); // new matrix with transposed dimensions

	// copy data
	for(int i=0; i<_rows; i++){
		for(int j=0; j<_columns; j++){
			dest.data[j][i]=data[i][j];
		}
	}
	return dest;
}


// Addition of two matrices to a new one
Matrix operator+(const Matrix M1, const Matrix M2)
{
	if(M1._columns == M2._columns && M1._rows == M2._rows){
		Matrix temp(M1._rows, M1._columns);
		for(int i=0; i<M1._rows; i++){
			for(int j=0; j<M1._columns; j++){
				temp(i,j) = M1(i,j)+M2(i,j);
			}
		}
		return temp;
	}
	else{
		// ERROR: for addition matrix dimension must agree
		return Matrix(); // return empty matrix
	}
}
Matrix operator+(const Matrix M1, const double a)
{
	Matrix ret(M1._rows, M1._columns);
	for(int i=0; i<M1._rows; i++){
		for(int j=0; j<M1._columns; j++){
			ret(i,j) = M1(i,j)+a;
		}
	}
	return ret;
}
Matrix operator+(const double a, const Matrix M2)
{
	return M2+a;
}


// Subtraction of two matrices to a new one
Matrix operator-(const Matrix M1, const Matrix M2)
{
	if(M1._columns == M2._columns && M1._rows == M2._rows){
		Matrix temp(M1._rows, M1._columns);
		for(int i=0; i<M1._rows; i++){
			for(int j=0; j<M1._columns; j++){
				temp(i,j) = M1(i,j)-M2(i,j);
			}
		}
		return temp;
	}
	else{
		// ERROR: for subratrion matrix dimension must agree
		return *(new Matrix()); // return empty matrix
	}
}
Matrix operator-(const Matrix M1, const double a)
{
	Matrix ret(M1._rows, M1._columns);
	for(int i=0; i<M1._rows; i++){
		for(int j=0; j<M1._columns; j++){
			ret(i,j) = M1(i,j)-a;
		}
	}
	return ret;
}
Matrix operator-(const double a, const Matrix M2)
{
	Matrix ret(M2._rows, M2._columns);
	for(int i=0; i<M2._rows; i++){
		for(int j=0; j<M2._columns; j++){
			ret(i,j) = a-M2(i,j);
		}
	}
	return ret;
}

// Multiplication of two matrices to a new one
Matrix operator*(const Matrix M1, const Matrix M2)
{
	if(M1._columns == M2._rows){
		Matrix temp(M1._rows, M2._columns);
		for(int i=0; i<M1._rows; i++){
			for(int j=0; j<M2._columns; j++){
				double sum = 0;
				for(int n=0; n<M1._columns; n++){
					sum = sum + M1(i,n)*M2(n,j);
				}
				temp(i,j) = sum;
			}
		}
		return temp;
	}
	else{
		// ERROR: for multiplication matrix dimension must agree
		return Matrix(); // return empty matrix
	}
}
Matrix operator*(const Matrix M1, const double a)
{
	Matrix ret(M1._rows, M1._columns);
	for(int i=0; i<M1._rows; i++){
		for(int j=0; j<M1._columns; j++){
			ret(i,j) = M1(i,j)*a;
		}
	}
	return ret;
}
Matrix operator*(const double a, const Matrix M2)
{
	return M2*a;
}
Matrix Matrix::dotMul(const Matrix M2)
{
	return dotMul(*this, M2);
}

/*
  element wise multiplication
  also a row-matrix can be multiplied by a column-matrix,
	the dimension of the returned matrix is that of the first one.
*/
Matrix Matrix::dotMul(const Matrix M1, const Matrix M2)
{
	if((M1._rows == M2._rows && M1._columns == M2._columns) ||
			(M1._rows == M2._columns && M1._columns == M2._rows)){
		Matrix ret(M1._rows, M1._columns);
		for(int i=0; i<M1._rows; i++){
			for(int j=0; j<M1._columns; j++){
				ret(i,j) = M1(i,j)*M2(i,j);
			}
		}
		return ret;
	}else{ // dimension error
		return Matrix();
	}
}

// Division
Matrix operator/(const Matrix M1, const double a)
{
	Matrix ret(M1._rows, M1._columns);
	for(int i=0; i<M1._rows; i++){
		for(int j=0; j<M1._columns; j++){
			ret(i,j) = M1(i,j)/a;
		}
	}
	return ret;
}
Matrix operator/(const double a, const Matrix M2)
{
	Matrix ret(M2._rows, M2._columns);
	for(int i=0; i<M2._rows; i++){
		for(int j=0; j<M2._columns; j++){
			ret(i,j) = a/M2(i,j);
		}
	}
	return ret;
}




Matrix Matrix::setTo(double v) // set all elements to v
{
	for(int i=0; i<_rows; i++){
		for(int j=0; j<_columns; j++){
			data[i][j]=v;
		}
	}
	return *this;
}


// TODO: check for determinant and quadratic dimension
Matrix Matrix::inv()
{
	Matrix inv(_rows, _columns);

	// TODO: überprüfungen auf NULL
	// überprüfen auf quadatisch
	// zusätzlichen speicher reservieren für die einheitsmatrix rechts.

	double **a; // speicher für die beiden matrizzen (originale und inverse)
	int n = _rows; //  dimension der Matrix

	int   i, j;                    // Zeile, Spalte
	int   s;                       // Elimininationsschritt
	int   pzeile;                  // Pivotzeile
	int   fehler = 0;              // Fehlerflag
	double f;                      // Multiplikationsfaktor
	const double Epsilon = 1e-8;   // Genauigkeit
	double Maximum;                // Zeilenpivotisierung
	int pivot = 1;


	// Speicher anlgen, der für beide Matrizen Plaz hat.
	a = new double*[n];
	double * mem = new double[2*n*n];
	for(i=0; i<n; i++){
		a[i]= &mem[i*n*2];
		memcpy(a[i],data[i], n*sizeof(double)); // originale Matrix in diesen Speicher kopieren
	}

	// ergänze die Matrix a um eine Einheitsmatrix (rechts anhängen)
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++)
		{
			a[i][n+j] = 0.0;
			if (i == j)
				a[i][n+j] = 1.0;
		}
	}

	// die einzelnen Eliminationsschritte
	s = 0;
	do {
		// Pivotisierung vermeidet unnötigen Abbruch bei einer Null in der Diagnonalen und
		// erhöht die Rechengenauigkeit
		Maximum = fabs(a[s][s]);
		if (pivot)
		{
			pzeile = s ;
			for (i = s+1; i < n; i++)
				if (fabs(a[i][s]) > Maximum) {
					Maximum = fabs(a[i][s]) ;
					pzeile = i;
				}
		}
		fehler = (Maximum < Epsilon);

		if (fehler) break;           // nicht lösbar

		if (pivot)
		{
			if (pzeile != s)  // falls erforderlich, Zeilen tauschen
			{
				double h;
				for (j = s ; j < 2*n; j++) {
					h = a[s][j];
					a[s][j] = a[pzeile][j];
					a[pzeile][j]= h;
				}
			}
		}

		// Eliminationszeile durch Pivot-Koeffizienten f = a[s][s] dividieren
		f = a[s][s];
		for (j = s; j < 2*n; j++)
		a[s][j] = a[s][j] / f;

		// Elimination --> Nullen in Spalte s oberhalb und unterhalb der Diagonalen
		// durch Addition der mit f multiplizierten Zeile s zur jeweiligen Zeile i
		for (i = 0; i < n; i++ ) {
			if (i != s)
			{
				f = -a[i][s];                 // Multiplikationsfaktor
				for (j = s; j < 2*n ; j++)    // die einzelnen Spalten
				a[i][j] += f*a[s][j];       // Addition der Zeilen i, s
			}
		}

		s++;
	} while ( s < n ) ;

	if (fehler)
	{
		std::cout << ("Inverse: Matrix ist fast singulär oder singulär\n");
		return Matrix();
	}
	// Die angehängte Einheitsmatrix Matrix hat sich jetzt in die inverse Matrix umgewandelt
	// Umkopieren auf die Zielmatrix
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			inv.data[i][j] = a[i][n+j];
		}
	}
	delete [] a[0];
	delete [] a;
	return inv;
}



// toString for Qt
QString Matrix::toString()
{
	QString str;

	for(int i=0; i<_rows; i++){
		for(int j=0; j<_columns; j++){
			str.append(QString::number(data[i][j],'f',8) + "\t");
		}
		str.append('\n');
	}
	return str;
}

Matrix Matrix::ident(int n)
{
	Matrix M = Matrix(n,n);
	for(int i=0; i<n; i++){
		M.data[i][i]=1;
	}
	return M;
}

Matrix Matrix::init(int numberOfElements, ...)
{
	va_list elements;
	va_start(elements, numberOfElements);
	for(int i=0; i<numberOfElements; i++){
		(*this)(i) = va_arg(elements, double);
	}
	va_end(elements);

	return *this;
}


Matrix Matrix::toColumnVector()
{
	Matrix ret = (*this).T(); // transpose
	ret = ret.toRowVector(); // convert
	ret = ret.T(); // transpose again
	return ret;
}

Matrix Matrix::toRowVector()
{
	Matrix ret = *this;
	ret._columns = ret._rows*ret._columns;
	ret._rows = 1;
	double* start = ret.data[0];
	delete [] ret.data;
	ret.data = new double*[1];
	ret.data[0]=start;

	return ret;
}

// todo:
//Matrix toMatrix(int rows)
//{
//	return Matrix();
//}
