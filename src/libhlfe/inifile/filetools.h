/*******************************************************
                          filetools.h
                          -----------
    begin                : Wed Dec 13 2000
    copyright            : (C) 2000 by Sylvain Le Petit
    email                : sylvainlepetit@wanadoo.fr
 *******************************************************/
#include <stdio.h>
#include <string.h>
#include "exception.h"
/* Structures de donnees */


typedef struct section
{
	char *name;
	struct key *fils;
	struct ini *parent;
	struct section *prev;
	struct section *next;

}
SECTION;

typedef struct key
{
	char *name;
	char *value;
	SECTION *parent;
	struct key *prev;
	struct key *next;
}
KEY;

typedef struct ini
{
	char *fileName;
	SECTION *fils;
}
INI;

/* declaration des prototypes*/
/*PUBLIC*/ int jumpline (int nbline, FILE * pfile);
int readline (FILE * pfile, char *tab);
/*************************************************************************
gestion des fichier .ini
**************************************************************************/

int readINI (char *section, char *key, char **ppStr, const char *fileName);
/*******************************************************************************
 lire une valeur

section: nom de la section
key : nom de la cle
ppStr: adresse du pointeur sur la valeur de la cle retour, NULL si pas trouve, memoire allouee dans la fonction
fileName : nom du fichier contenant les cles, pas forcement xxx.ini c'est pas obligatoire

**********************************************************************************/


char *writeINI (char *section, char *key, char *pKeyValue, const char *fileName);
/*******************************************************************************
  ecrire une cle avec sa valeur, que se soit une nouvelle cle ou un mise a jour
c'est pareil

section: nom de la section
key : nom de la cle
pKeyValue: valeur de la cle, evitez de mettre NULL pas encore tester ce cas limite
fileName : nom du fichier contenant les cles, pas forcement xxx.ini c'est pas obligatoire

 *******************************************************************************/

int delINI (char *sectionName, char *pKeyName, char **ppStr, const char *fileName);
/*************************************************************************/
/* permet de supprimer une cle ou une section complete                   */
/* >suppression d'une cle, donner le nomde la section et le nom de la cle*/
/*  si la section est vide elle est supprimee                            */
/* >suppression d'une section complete, mettre NULL comme nom de cle     */
/*************************************************************************/

void printINI (char *fileName);
/*****************************************************************/
/*  permet d'afficher le contenu du fichier .ini, seulement les: */
/*  [section]                                                    */
/*  cle=valeur                                                   */
/*****************************************************************/



/*PRIVATE: ne pas utiliser directement*/
int _saveINI (INI * ini);
INI *_loadINI (const char *fileName);
void _printINI (INI * pIni);
void _printSECTION (SECTION * pSection);
void _printKEY (KEY * pKey);
KEY *_newKEY (char *pKeyName, char *pKeyValue);
SECTION *_newSECTION (char *pSectionName);
SECTION *_findSECTION (INI * pIni, char *pSectionName);
KEY *_findKEY (SECTION * pSection, char *pKeyName);
KEY *_lastKEY (SECTION * pSection);
SECTION *_lastSECTION (INI * pIni);
SECTION *_addSECTION (INI * pIni, SECTION * pSection);
KEY *_addKEY (SECTION * pSection, KEY * pKey);
int _isKey (char *str);
void *m_malloc (size_t size);	/*renvoi l'exception ERR_MALLOC */
void _freeKEY (KEY * pKey);
void _freeSECTION (SECTION * pSection);
int _delKEY (KEY * pKey);
int _delSECTION (SECTION * pSection);
