// +--------------------------------------------------------------------------+
// | Fichier   : erreur.cpp                                                   |
// | Utilite   : définition de la Erreur.                                     |
// | Auteurs   : Nicolas AUNAI                                                |
// | Creation  : 16.01.2003                                                   |
// | Remarques : aucune.                                                      |
// +--------------------------------------------------------------------------+

#include "erreur.h"

Erreur::Erreur()
		:c(-1)
{
}

Erreur::Erreur(Erreur &source)
		:c(source.c)
{
	exp = source.exp;
}

Erreur::Erreur(int a)
		:c(a)
{
	switch (a)
	{
	case ncarre:
		exp = "matrice non carre, operation impossible";
		break;
	case nvect:
		exp = "n'est pas un vecteur, operation impossible";
		break;
	case multiplication:
		exp = "erreur de dimension, operation impossible";
		break;
	case notinmat:
		exp = "erreur de taille, operation impossible";
		break;
	case dbzero:
		exp = "division par 0, operation impossible";
		break;
	case getval:
		exp = "get_val, out of range";
		break;
	case setval:
		exp = "set_val, out of range";
		break;
	default:
		exp= "erreur inconnue";
	}
}

int
Erreur::cause() const
{
	return c;
}

string
Erreur::explic() const
{
	return exp;
}
