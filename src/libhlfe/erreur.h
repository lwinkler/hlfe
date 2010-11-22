// +--------------------------------------------------------------------------+
// | Fichier   : erreur.h                                                     |
// | Utilite   : déclaration de la Erreur.                                    |
// | Auteurs   : Nicolas AUNAI                                                |
// | Creation  : 16.01.2003                                                   |
// | Remarques : aucune.                                                      |
// +--------------------------------------------------------------------------+

#ifndef _ERREUR_H_
#define _ERREUR_H_
using namespace std;
#include <string>

class Erreur
{
private:
	int c;
	string exp;

public:
	static const int ncarre = 0,
							  multiplication = 1,
											   notinmat = 2,
														  dbzero = 3,
																   setval = 4,
																			getval = 5,
																					 nvect = 6;

	Erreur();
	Erreur(Erreur &s);
	Erreur(int a);
	int cause() const;
	string explic() const ;
};
#endif
