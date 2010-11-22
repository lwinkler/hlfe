// +--------------------------------------------------------------------------+
// | Fichier   : matrix.h                                                     |
// | Utilite   : déclaration de la Matrice.                                   |
// | Auteurs   : Nicolas AUNAI                                                |
// | Creation  : 16.01.2003                                                   |
// | Remarques : aucune.                                                      |
// +--------------------------------------------------------------------------+

#ifndef _MATRIX_H_
#define _MATRIX_H_
#include <iostream>
#include "erreur.h"

using namespace std;


class Matrice
{
private:
	typedef double *ligne;
	ligne *lignes;
	unsigned int n; // Nombre de lignes (1er paramètre)
	unsigned int m; // Nombre de colonnes (2ème paramètre)
	void echange(double &a, double &b) const;
	Matrice supligne(const Matrice &mat, unsigned int lig, unsigned int col) const;
	double det(const Matrice &mat) const;
	float expo(unsigned int n) const;

public:
	//les constructeurs/destructeur
	Matrice();
	Matrice(unsigned int nl, unsigned int nc);
	Matrice(unsigned int nl, unsigned int nc, double valeur);
	Matrice(const Matrice &source);
	~Matrice(void);
	//les operateurs
	Matrice &operator=(const Matrice &mat);
	double &operator()(unsigned int i, unsigned int j);
	Matrice operator+(const Matrice &mat) const;
	Matrice operator-(const Matrice &mat) const;
	Matrice operator*(const Matrice &mat) const;
	Matrice operator*(const double nb) const;
	friend Matrice operator*(const double nb,const Matrice &mat);
	Matrice & operator*=(const Matrice &mat);
	Matrice & operator*=(const double nb);
	Matrice & operator+=(const Matrice &mat);
	Matrice & operator-=(const Matrice &mat);
	Matrice & operator/=(const double);
	Matrice  operator/(const double) const;
	//accesseur et manipulations
	Matrice t();
	double trace() const;
	double sqnorm() const;
	double norm() const;
	double det() const;
	Matrice co() const;
	Matrice inverse() const;
	int dim() const;
	unsigned int size_m() const;
	unsigned int size_n() const;
	/*Added by Laurent Winkler*/
	void set_val(int row, int col, double val);
	double get_val(int row, int col);

	friend ostream & operator<<(ostream &os,const Matrice &mat);

};
#endif
