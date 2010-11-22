/*filetools.c*/

#include "filetools.h"

#define MAX_LENGTH 255

/******************************************************
 * NOM:              readINI                          *
 * SPECIFICATION:                                     *
 *   retourne la valeur de la cle du                  *
 *               fichier dont la structure est celle  *
 *               d'un fichier .ini windows            *
 * ARGUMENTS:                                         *
 * >section: le nom de la section sans les crochets   *
 * >key: nom de la cle de la section                  *
 * >pStr: pointeur pour mettre le resultat            *
 *       allocation memoire effectuee                 *
 * ENVIRONNEMENT:                                     *
 * VALEUR RETOURNEE:0 si echec 1 si oui               *
 ******************************************************/
int
readINI (char *sectionName, char *pKeyName, char **ppStr, const char *fileName)
{
	INI *pIni = NULL;
	SECTION *pSection = NULL;
	KEY *pKey = NULL;
	int trouve = 0;
	char *pSectionName = NULL;
	int offset = 0;
	//pStr = resultValue;
	TRY
	{
		trouve = 0;
		pIni = _loadINI (fileName);
		/*recherche de la section */
		offset = strlen (sectionName) + 3;
		pSectionName = (char *) m_malloc (sizeof (char) * offset);
		pSectionName[0] = '[';
		strcpy (pSectionName + 1, sectionName);
		pSectionName[strlen (sectionName) + 1] = ']';
		pSectionName[offset - 1]='\0';

		pSection = _findSECTION (pIni, pSectionName);

		if (pSection != NULL)
		{
			/*recherche de la cle */
			pKey = _findKEY (pSection, pKeyName);
			if (pKey != NULL)
			{
				*ppStr = (char *) m_malloc (sizeof (char) * strlen (pKey->value) + 2);
				strcpy (*ppStr, pKey->value);
				(*ppStr)[strlen (pKey->value)] = '\0';
				trouve = 1;
			}
		}
	}
	CATCH (ERR_FOPEN)
	{
		printf ("impossible d'ouvrir le fichier %s\n", fileName);
		ppStr = NULL;
	}
	CATCH (ERR_MALLOC)
	{
		perror ("Probleme d'allocation memoire");
		exit (EXIT_FAILURE);
	}
	ENDTRY;
	return trouve;
}

/******************************************************
 * NOM:                delINI                         *
 * SPECIFICATION:                                     *
 *  seule la cle de la section est supprimee          *
 *  si la section est vide, elle est supprimee        *
 * ARGUMENTS:                                         *
 * >section: le nom de la section sans les crochets   *
 * >key: nom de la cle de la section                  *
 * >pStr: pointeur pour mettre la valeur de la cle    *
 *       avant sa suppression                         *
 *       allocation memoire effectuee                 *
 * ENVIRONNEMENT:                                     *
 * VALEUR RETOURNEE:                                  *
 * 1 si reussite de la suppression, 0 sinon           *
 ******************************************************/
int
delINI (char *sectionName, char *pKeyName, char **ppStr, char *fileName)
{
	INI *pIni = NULL;
	SECTION *pSection = NULL;
	KEY *pKey = NULL;
	int del = 0;
	char *pSectionName = NULL;
	int offset = 0;
	//pStr = resultValue;
	TRY
	{

		pIni = _loadINI (fileName);
		/*recherche de la section */
		offset = strlen (sectionName) + 3;
		pSectionName = (char *) m_malloc (sizeof (char) * offset);
		pSectionName[0] = '[';
		strcpy (pSectionName + 1, sectionName);
		pSectionName[strlen (sectionName) + 1] = ']';
		//pSectionName[offset - 1];

		pSection = _findSECTION (pIni, pSectionName);

		if (pSection != NULL)
		{
			if (pKeyName == NULL)
			{
				/*on supprime toute la section */

				pKey = _lastKEY (pSection);
				while (pKey != NULL)
				{
					_delKEY (pKey);
					pKey = _lastKEY (pSection);
				}
				_delSECTION (pSection);
				del = 1;
			}
			else
			{
				/*recherche de la cle si indiquee */
				pKey = _findKEY (pSection, pKeyName);
				if (pKey != NULL)
				{
					(*ppStr) =
						(char *) m_malloc (sizeof (char) * strlen (pKey->value) +
										   2);
					strcpy (*ppStr, pKey->value);
					(*ppStr)[strlen (pKey->value)] = '\0';
					if (_delKEY (pKey))
					{
						/*si la cle supprimee etait la derniere de la section */
						_delSECTION (pSection);
					}
					del = 1;
				}
			}
		}
		if (del)
		{
			_saveINI (pIni);
		}
	}
	CATCH (ERR_FOPEN)
	{
		printf ("impossible d'ouvrir le fichier %s\n", fileName);
		(*ppStr) = NULL;
	}
	CATCH (ERR_MALLOC)
	{
		perror ("Probleme d'allocation memoire");
		exit (EXIT_FAILURE);
	}
	ENDTRY;
	return del;
}

/******************************************************
 * NOM:                writeINI                       *
 * SPECIFICATION:                                     *
 *  si la section n'existe pas elle est cree          *
 *  si la cle n'existe pas elle est cree              *
 *  et la valeur de la cle est initialisee            *
 *  sinon la cle existante de la section existante est*
 *  mise a jour                                       *
 * ARGUMENTS:                                         *
 * ENVIRONNEMENT:                                     *
 * VALEUR RETOURNEE:                                  *
 *  NULL si echec ou keyValue si reussite             *
 ******************************************************/
char *
writeINI (char *sectionName, char *keyName, char *keyValue, char *fileName)
{
	char *pSectionName = NULL;
	char *pKeyName = NULL;
	char *pKeyValue = NULL;
	INI *pIni = NULL;
	SECTION *pSection = NULL;
	KEY *pKey = NULL;
	TRY
	{
		pKeyName = (char *) m_malloc (sizeof (char) * strlen (keyName) + 1);
		pSectionName =
			(char *) m_malloc (sizeof (char) * strlen (sectionName) + 3);
		if (keyValue != NULL)
		{
			pKeyValue = (char *) m_malloc (sizeof (char) * strlen (keyValue) + 1);
			strcpy (pKeyValue, keyValue);
			pKeyValue[strlen (keyValue)] = '\0';
		}
		else
		{
			pKeyValue = (char *) m_malloc (sizeof (char));
			pKeyValue[0] = '\0';
		}
		strcpy (pKeyName, keyName);
		pSectionName[0] = '[';
		strcpy (pSectionName + 1, sectionName);
		pSectionName[strlen (sectionName) + 1] = ']';
		pSectionName[strlen (sectionName) + 2] = '\0';
		/*chargement */
		pIni = _loadINI (fileName);

	}
	CATCH (ERR_FOPEN)
	{
		printf ("Erreur en ouvrant le fichier .ini:%s\n", fileName);
		exit (EXIT_FAILURE);
	}
	CATCH (ERR_MALLOC)
	{
		perror ("Probleme d'allocation memoire");
		exit (EXIT_FAILURE);
	}
	ENDTRY;

	/*recherche de la section */
	pSection = _findSECTION (pIni, pSectionName);
	/*recherche de la cle */
	if (pSection != NULL)
	{
		pKey = _findKEY (pSection, pKeyName);
		if (pKey != NULL)
		{
			pKey->value =
				(char *) realloc (pKey->value,
								  sizeof (char) * strlen (pKeyName) + 1);
			if (pKey->value == NULL)
			{
				perror ("probleme de reallocation memoire");
				exit (EXIT_FAILURE);
			}
			strcpy (pKey->value, pKeyValue);
		}
		else
		{
			/*cle pas trouve dans la section, on ajoute la cle a la section */
			pKey = _newKEY (pKeyName, pKeyValue);
			_addKEY (pSection, pKey);
		}
	}
	else
	{
		/*section pas trouve, on la cree avec sa cle */
		pSection = _newSECTION (pSectionName);
		_addSECTION (pIni, pSection);
		pKey = _newKEY (pKeyName, pKeyValue);
		_addKEY (pSection, pKey);

	}
	TRY
	{

		/*sauvegarde */
		_saveINI (pIni);

	}
	CATCH (ERR_FOPEN)
	{
		printf ("erreur sur le fichier %s", pIni->fileName);
	}
	ENDTRY;

	/*nettoyage */

	free (pIni);
	free (pSection);
	free (pKey);
	free (pKeyName);
	free (pSectionName);
	pIni = NULL;
	pKeyName = NULL;
	pSectionName = NULL;
	return pKeyValue;
}

/******************************************************
 * NOM:                                               *
 *                 _delKEY                            *
 * SPECIFICATION:                                     *
 * ARGUMENTS:                                         *
 * ENVIRONNEMENT:                                     *
 * VALEUR RETOURNEE:                                  *
 *  1 si derniere cellule, 0 sinon, -1 si probleme    *
 ******************************************************/
int
_delKEY (KEY * pKey)
{
	int result = 0;
	if (pKey != NULL)
	{
		/*la cle n'est pas la premiere */
		if (pKey->prev != pKey)
		{
			if (pKey->next != pKey)
			{
				/*la cle n'est pas la derniere */
				pKey->prev->next = pKey->next;
			}
			else
			{
				result = 1;
				pKey->prev->next = pKey->prev;
			}
		}
		else
		{
			if (pKey->next != pKey)
			{
				/*la cle n'est pas non plus la derniere */
				pKey->parent->fils = pKey->next;
			}
			else
			{
				result = 1;
				pKey->parent->fils = NULL;
			}
		}
		_freeKEY (pKey);
		return result;
	}
	else
	{
		result = -1;
		return result;
	}
}


/******************************************************
 * NOM:                                               *
 *                 _delSECTION                        *
 * SPECIFICATION:                                     *
 * ARGUMENTS:                                         *
 * ENVIRONNEMENT:                                     *
 * VALEUR RETOURNEE:                                  *
 *  pointeur sur la section                           *
 ******************************************************/
int
_delSECTION (SECTION * pSection)
{
	if (pSection != NULL)
	{
		/*la cle n'est pas la premiere */
		if (pSection->prev != pSection)
		{
			/*la cle n'est pas la derniere */
			if (pSection->next != pSection)
			{
				pSection->prev->next = pSection->next;
			}
			else
			{
				pSection->prev->next = pSection->prev;
			}
		}
		else
		{
			/*la cle n'estpas nonplus la derniere */
			if (pSection->next != pSection)
			{
				pSection->parent->fils = pSection->next;
				pSection->next->prev = pSection->next;
			}
			else
			{
				pSection->parent->fils = NULL;
			}
		}
		_freeSECTION (pSection);
		return 1;
	}
	else
	{
		return 0;
	}
}

/************************
 * liberation memoire   *
 ************************/
void
_freeKEY (KEY * pKey)
{
	if (pKey != NULL)
	{
		pKey->parent = NULL;
		pKey->next = NULL;
		pKey->prev = NULL;
		if (pKey->value != NULL)
		{
			free (pKey->value);
		}
		if (pKey->name != NULL)
		{
			free (pKey->name);
		}
		free (pKey);
	}
}
void
_freeSECTION (SECTION * pSection)
{
	if (pSection != NULL)
	{
		pSection->parent = NULL;
		pSection->next = NULL;
		pSection->prev = NULL;
		if (pSection->name != NULL)
		{
			free (pSection->name);
		}
		free (pSection);
	}
}

/******************************************************
 * NOM:                                               *
 *                 _newKEY                            *
 * SPECIFICATION:                                     *
 *  allocation d'une nouvelle cle                     *
 * ARGUMENTS:                                         *
 * le nom et la valeur de la cle                      *
 * ENVIRONNEMENT:                                     *
 * VALEUR RETOURNEE:                                  *
 * pointeur sur la cle, NULL sinon                    *
 ******************************************************/
KEY *
_newKEY (char *pKeyName, char *pKeyValue)
{
	KEY *pKey;
	TRY
	{
		pKey = (KEY *) m_malloc (sizeof (KEY));

		pKey->name = (char *) m_malloc (sizeof (char) * strlen (pKeyName) + 1);
		pKey->value = (char *) m_malloc (sizeof (char) * strlen (pKeyValue) + 1);
	}
	CATCH (ERR_MALLOC)
	{
		perror ("Probleme d'allocation memoire");
		exit (EXIT_FAILURE);
	}
	ENDTRY;

	strcpy (pKey->name, pKeyName);
	strcpy (pKey->value, pKeyValue);
	pKey->parent = NULL;
	pKey->prev = pKey;
	pKey->next = pKey;


	return pKey;
}

/******************************************************
 * NOM:                                               *
 *                 _newSECTION                        *
 * SPECIFICATION:                                     *
 *  allocation d'une nouvelle section                 *
 * ARGUMENTS:                                         *
 * le nom de la section                               *
 * ENVIRONNEMENT:                                     *
 * VALEUR RETOURNEE:                                  *
 * pointeur sur la section, NULL sinon                *
 ******************************************************/
SECTION *
_newSECTION (char *pSectionName)
{
	SECTION *pSection;
	TRY
	{
		pSection = (SECTION *) m_malloc (sizeof (SECTION));

		pSection->name =
			(char *) m_malloc (sizeof (char) * strlen (pSectionName) + 1);
	}
	CATCH (ERR_MALLOC)
	{
		perror ("Probleme d'allocation memoire");
		exit (EXIT_FAILURE);
	}
	ENDTRY;

	strcpy (pSection->name, pSectionName);
	pSection->parent = NULL;
	pSection->fils = NULL;
	pSection->prev = pSection;
	pSection->next = pSection;
	return pSection;
}

/******************************************************
 * NOM:                                               *
 *                 _findSECTION                       *
 * SPECIFICATION:                                     *
 *  trouver la section dans une struture INI          *
 * ARGUMENTS:                                         *
 * la structure INI, le nom de la section avec ses [ ]*
 * ENVIRONNEMENT:                                     *
 * VALEUR RETOURNEE:                                  *
 * pointeur sur la section, NULL sinon                *
 ******************************************************/
SECTION *
_findSECTION (INI * pIni, char *pSectionName)
{
	SECTION *pSection = NULL;
	pSection = pIni->fils;
	while ((pSection != NULL) && (strcmp (pSection->name, pSectionName) != 0))
	{
		if (pSection == pSection->next)
		{
			pSection = NULL;
		}
		else
		{
			pSection = pSection->next;
		}
	}
	return pSection;
}

/******************************************************
 * NOM:                                               *
 *                 _findKEY                           *
 * SPECIFICATION:                                     *
 *  trouver la cle dans une struture SECTION          *
 * ARGUMENTS:                                         *
 * la structure SECTION, le nom de la cle             *
 * ENVIRONNEMENT:                                     *
 * VALEUR RETOURNEE:                                  *
 * pointeur sur la cle, NULL sinon                    *
 ******************************************************/
KEY *
_findKEY (SECTION * pSection, char *pKeyName)
{
	KEY *pKey = NULL;
	pKey = pSection->fils;
	while ((pKey != NULL) && (strcmp (pKey->name, pKeyName) != 0))
	{
		if (pKey == pKey->next)
		{
			pKey = NULL;
		}
		else
		{
			pKey = pKey->next;
		}
	}
	return pKey;
}

/******************************************************
 * NOM:                                               *
 *                 _lastKEY                           *
 * SPECIFICATION:                                     *
 *  trouver la derniere cle dans une struture SECTION *
 * ARGUMENTS:                                         *
 * la structure SECTION                               *
 * ENVIRONNEMENT:                                     *
 * VALEUR RETOURNEE:                                  *
 * pointeur sur la cle, NULL sinon                    *
 ******************************************************/
KEY *
_lastKEY (SECTION * pSection)
{
	KEY *pKey_pred = NULL;
	KEY *pKey = NULL;
	pKey = pSection->fils;
	pKey_pred = pKey;
	while (pKey != NULL)
	{
		pKey_pred = pKey;
		if (pKey == pKey->next)
		{
			pKey = NULL;
		}
		else
		{
			pKey = pKey->next;
		}
	}
	return pKey_pred;
}

/******************************************************
 * NOM:                                               *
 *                 _lastSECTION                       *
 * SPECIFICATION:                                     *
 *  trouver la derniere section dans une INI          *
 * ARGUMENTS:                                         *
 * la structure INI                                   *
 * ENVIRONNEMENT:                                     *
 * VALEUR RETOURNEE:                                  *
 * pointeur sur la section, NULL sinon                *
 ******************************************************/
SECTION *
_lastSECTION (INI * pIni)
{
	SECTION *pSection_pred = NULL;
	SECTION *pSection = NULL;
	pSection = pIni->fils;
	pSection_pred = pSection;
	while (pSection != NULL)
	{
		pSection_pred = pSection;
		if (pSection == pSection->next)
		{
			pSection = NULL;
		}
		else
		{
			pSection = pSection->next;
		}
	}
	return pSection_pred;
}

/******************************************************
 * NOM:                                               *
 *                 _addSECTION                        *
 * SPECIFICATION:                                     *
 *  ajoute une section en derniere place dans une INI *
 *  la section ne doit pas appartenir a une INI       *
 * ARGUMENTS:                                         *
 * la structure INI, la structure SECTION             *
 * ENVIRONNEMENT:                                     *
 * VALEUR RETOURNEE:                                  *
 * pointeur sur la section, NULL sinon                *
 ******************************************************/
SECTION *
_addSECTION (INI * pIni, SECTION * pSection)
{
	SECTION *pSection_last = NULL;

	if (pSection->parent != NULL)
	{
		return NULL;
	}
	else
	{
		pSection_last = _lastSECTION (pIni);
		pSection->parent = pIni;
		if (pSection_last != NULL)
		{
			pSection_last->next = pSection;
			pSection->prev = pSection_last;
		}
		else
		{
			pIni->fils = pSection;
		}
		return pSection;
	}
}

/******************************************************
 * NOM:                                               *
 *                 _addKEY                            *
 * SPECIFICATION:                                     *
 *  ajoute une cle en derniere place dans une SECTION *
 *  la section ne doit pas appartenir a une SECTION   *
 * ARGUMENTS:                                         *
 * la structure KEY, la structure SECTION             *
 * ENVIRONNEMENT:                                     *
 * VALEUR RETOURNEE:                                  *
 * pointeur sur la cle, NULL sinon                *
 ******************************************************/
KEY *
_addKEY (SECTION * pSection, KEY * pKey)
{
	KEY *pKey_last = NULL;

	if (pKey->parent != NULL)
	{
		return NULL;
	}
	else
	{

		pKey_last = _lastKEY (pSection);
		pKey->parent = pSection;
		if (pKey_last != NULL)
		{
			pKey_last->next = pKey;
			pKey->prev = pKey_last;
		}
		else
		{
			pSection->fils = pKey;
		}
		pKey->next = pKey;
		return pKey;
	}
}

/******************************************************
 * NOM:                                               *
 *                 saveINI                            *
 * SPECIFICATION:                                     *
 *  sauvegarde de la structure INI dans son fichier   *
 *  Attention, ecrase l'ancien fichier                *
* ARGUMENTS:                                         *
*  pointeur sur la structure INI a sauvegarder       *
* ENVIRONNEMENT:                                     *
* VALEUR RETOURNEE:                                  *
*  -1 si un probleme a eu lieu, 0 sinon              *
******************************************************/
int
_saveINI (INI * pIni)
{
	FILE *pFile = NULL;
	char *str = NULL;
	SECTION *pSection_pred = NULL, *pSection = NULL;
	KEY *pKey_pred = NULL, *pKey = NULL;
	int offset = 0;

	pFile = fopen (pIni->fileName, "w");
	if (pFile == NULL)
		THROW (ERR_FOPEN);
	//printf("#SAUVEGARDE Fichier INI#\n");
	//printf("# %s #\n\n",pIni->fileName);
	if (pIni->fils != NULL)
	{
		pSection = pIni->fils;
		do
		{

			fprintf (pFile, "%s", pSection->name);
			fprintf (pFile, "\n");
			pKey = pSection->fils;
			do
			{

				offset = strlen (pKey->name) + strlen (pKey->value) + 2;

				TRY
				{
					str = (char *) m_malloc (sizeof (char) * offset);
				}
				CATCH (ERR_MALLOC)
				{
					perror ("probleme d'allocation memoire");
					exit (EXIT_FAILURE);
				}
				ENDTRY;
				memset (str, 0, offset);
				strcpy (str, pKey->name);
				offset = strlen (pKey->name);
				strcpy (str + offset, "=");
				offset = strlen (pKey->name) + 1;
				strcpy (str + offset, pKey->value);
				fprintf (pFile, "%s", str);
				fprintf (pFile, "\n");
				pKey_pred = pKey;
				pKey = pKey->next;
				free (str);
			}
			while (pKey != pKey_pred);

			pSection_pred = pSection;
			pSection = pSection->next;
		}
		while (pSection != pSection_pred);
	}
	/*nettoyage */
	fclose (pFile);

	return 0;
}

/******************************************************
 * NOM:                                               *
 *                  loadINI                           *
 * SPECIFICATION:                                     *
 *  Charge le contenu d'un fichier .ini en memoire    *
 * ARGUMENTS:                                         *
 *  nom du fichier                                    *
 * ENVIRONNEMENT:                                     *
 * VALEUR RETOURNEE:                                  *
 *  pointeur sur la structure INI contenant le fichier*
 *  .ini avec les structures decrites dans le .h      *
 * EXCEPTION:                                         *
 * ERR_FOPEN, ERR_MALLOC
 ******************************************************/
INI *
_loadINI (const char *fileName)
{
	FILE *pFile = NULL;
	char *pBuf;
	int nbRead = 0;
	SECTION *pSection = NULL;
	KEY *pKey = NULL;

	INI *pIni = NULL;
	pBuf = (char *) m_malloc (sizeof (char) * MAX_LENGTH);

	pFile = fopen (fileName, "r");
	if (pFile == NULL)
	{
		pFile = fopen (fileName, "a");
		if (pFile == NULL)
			THROW (ERR_FOPEN);	/*encore NULL !!? la! il y a vraiment un probleme !! */
	}

	pIni = (INI *) m_malloc (sizeof (INI));
	pIni->fileName = (char *) m_malloc (sizeof (char) * strlen (fileName) + 1);
	strcpy (pIni->fileName, fileName);
	pIni->fils = NULL;
	/* on lit ligne par ligne */
	nbRead = readline (pFile, pBuf);
	while (nbRead != 0)
	{

		if ((pBuf[0] == '[') && (pBuf[strlen (pBuf) - 1] == ']'))
		{
			/* une section */
			pSection = _newSECTION (pBuf);
			_addSECTION (pIni, pSection);
		}

		/* les cles de la section */
		while (((nbRead = readline (pFile, pBuf)) != 0) && (pBuf[0] != '['))
		{
			int ind = 0;
			pKey = NULL;
			if ((ind = _isKey (pBuf)) >= 0)
			{
				if (pSection==NULL) {
					printf("ERROR in %s: lines appearing outside section or file format problem(line %d)\n",fileName,__LINE__);
					exit(-1);
				}

				char *strName = NULL;
				strName = (char *) m_malloc (sizeof (char) * ind + 1);
				strncpy (strName, pBuf, ind);
				strName[ind] = '\0';
				pKey = _newKEY (strName, pBuf + ind + 1);
				_addKEY (pSection, pKey);
			}
		}
	}

	/*nettoyage */
	free (pBuf);
	fclose (pFile);
	return pIni;
}

/******************************************************
 * NOM:                                               *
 *                 _isKey                             *
 * SPECIFICATION:                                     *
 *  determine si une chaine est une cle valide        *
 * ARGUMENTS:                                         *
 *  pointeur sur la chaine                            *
 * ENVIRONNEMENT:                                     *
 * VALEUR RETOURNEE:                                  *
 *  indice du signe '=' si oui, -1 sinon              *
 ******************************************************/
int
_isKey (char *str)
{
	int ind = 0;
	for (ind; (ind < strlen (str)) && (str[ind] != '='); ind++);
	if (str[ind] == '=')
		return ind;
	else
		return -1;
}


/******************************************************
 * NOM:                                               *
 *                  _printINI                         *
 * SPECIFICATION:                                     *
 * affiche a l'ecran le contenu de structure INI      *
 * ARGUMENTS:                                         *
 *  pointeur sur la structure INI                     *
 * ENVIRONNEMENT:                                     *
 * VALEUR RETOURNEE:                                  *
 ******************************************************/
void
_printINI (INI * pIni)
{
	SECTION *pSection_pred = NULL;
	SECTION *pSection = NULL;
	printf ("#Fichier INI#\n");
	printf ("# %s #\n\n", pIni->fileName);
	pSection = pIni->fils;
	if (pSection != NULL)
	{
		do
		{
			_printSECTION (pSection);
			pSection_pred = pSection;
			pSection = pSection->next;
		}
		while (pSection != pSection_pred);
	}
	printf ("\n#Fin du fichier INI#\n");
}

/******************************************************
 * NOM:                                               *
 *                  _printSECTION                     *
 * SPECIFICATION:                                     *
 * affiche a l'ecran le contenu de structure SECTION  *
 * ARGUMENTS:                                         *
 *  pointeur sur la structure SECTION                 *
 * ENVIRONNEMENT:                                     *
 * VALEUR RETOURNEE:                                  *
 ******************************************************/
void
_printSECTION (SECTION * pSection)
{
	KEY *pKey = NULL;
	KEY *pKey_pred = NULL;
	printf ("%s\n", pSection->name);
	pKey = pSection->fils;
	if (pKey != NULL)
	{
		do
		{
			_printKEY (pKey);
			pKey_pred = pKey;
			pKey = pKey->next;
		}
		while (pKey != pKey_pred);
	}
}

/******************************************************
 * NOM:                                               *
 *                  _printKEY                         *
 * SPECIFICATION:                                     *
 * affiche a l'ecran le contenu de structure SECTION  *
 * ARGUMENTS:                                         *
 *  pointeur sur la structure SECTION                 *
 * ENVIRONNEMENT:                                     *
 * VALEUR RETOURNEE:                                  *
 ******************************************************/
void
_printKEY (KEY * pKey)
{
	printf ("%s=%s\n", pKey->name, pKey->value);
}

/******************************************************
 * NOM:                                               *
 *                  printINI                          *
 * SPECIFICATION:                                     *
 * affiche a l'ecran le contenu du fichier .ini       *
 * ARGUMENTS:                                         *
 *  nom du fichier                                    *
 * ENVIRONNEMENT:                                     *
 * VALEUR RETOURNEE:                                  *
 ******************************************************/
void
printINI (char *file)
{
	INI *pIni = NULL;
	TRY
	{
		pIni = _loadINI (file);
	}
	CATCH (ERR_MALLOC)
	{
		printf ("Erreur d'allocation memoire en chargeant le fichier .ini:%s\n",
				file);
		exit (EXIT_FAILURE);
	}
	CATCH (ERR_FOPEN)
	{
		printf ("Erreur en ouvrant le fichier .ini:%s\n", file);
		exit (EXIT_FAILURE);
	}
	ENDTRY;
	_printINI (pIni);
	free (pIni);
}

/******************************************************
 * NOM:                 jumpline                      *
 * SPECIFICATION:                                     *
 * fonction permettant de sauter des lignes dans le   *
 * fichier a partir de la position courante dans le   *
 * fichier                                            *
 * ARGUMENTS:                                         *
 * nombre de lignes a saute, pointeur sur le fichier  *
 * ENVIRONNEMENT:                                     *
 * VALEUR RETOURNEE: nombre de lignes sautees         *
 ******************************************************/

int
jumpline (int nbline, FILE * pfile)
{
	int compteur = 0, c;

	while ((compteur < nbline) && ((c = getc (pfile)) != EOF))
	{
		if (c == '\n')
			compteur++;
	}
	return (compteur);
}

/******************************************************
 * NOM:                readline                       *
 * SPECIFICATION:                                     *
 * recupere la ligne courante du fichier et renvoie le*
 * nombre de caracteres lus                           *
 * ARGUMENTS:                                         *
 * pointeur sur le fichier a lire                     *
 * pointeur sur le tableau recevant la ligne          *
 * ENVIRONNEMENT:                                     *
 * VALEUR RETOURNEE:                                  *
 * nombre de caracteres lus                           *
 ******************************************************/
int
readline (FILE * pfile, char *tab)
{
	int nbchar = 0;
	char c;
	while ((c = getc (pfile)) != '\n')
	{
		if (c == EOF)
		{
			break;
		}
		tab[nbchar++] = c;
	}
	tab[nbchar] = '\0';
	return (nbchar);
}

/******************************************************
 * NOM:                m_malloc                       *
 * SPECIFICATION:                                     *
 *  clone de malloc renvoyant une exception           *
 * ARGUMENTS:                                         *
 * ENVIRONNEMENT:                                     *
 * VALEUR RETOURNEE:                                  *
 ******************************************************/
void *
m_malloc (size_t size)
{
	void *ptr = malloc (size);
	if (ptr == NULL)
		THROW (ERR_MALLOC);
	else
		return ptr;
}
