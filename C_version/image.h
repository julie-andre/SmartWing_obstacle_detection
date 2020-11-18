#pragma once

#ifndef IMAGE_H
# define IMAGE_H

struct _header
{
	char typeFichier[3];		// contient 'B' , 'M'  et '\0' annon�ant la fin de la chaine
	int taille;
	int rsv;		// champs r�serv�, 0 par d�faut
	int offsetim;		// indice auquel commence le contenu de l'image
	int size_imhead;	// taille ent�te
	int width;			// largeur de l'image
	int height;			// longueur de l'image
	short nbplans;		// nombre de plans : toujours 1
	short bpp;		// nombre de bits par couleur, on ne supporte que les les bpp de 24 bits
	int compression;	// 0 si non compress� ; on ne g�re que les fichiers non compress�s
	int sizeim;			// taille totale de l'image
	int hres;			// r�solution horizontale
	int vres;			// r�solution verticale
	int cpalette;		// nbre de couleurs de la palette mais pour du 24 bits pas de palette
	int cIpalette;		// nbre de couleurs importantes de la palette, 0 si toutes les couleurs sont importantes
};
typedef struct _header header;

struct _pixel
{
	int r;
	int v;
	int b;
};
typedef struct _pixel pixel;

typedef struct
{
	int longueur;
	int largeur;
	pixel** content;
}matrice;

struct _image
{
	matrice* mat;
	header infos;	
};
typedef struct _image image;


/*
 * Fonction qui cr�e dynamiquement une matrice, dont le contenu est vide (i.e. sans �l�ment).
 * longueur : nombre de lignes de la matrice cr��e
 * largeur : nombre de colonnes
 * Retour : un pointeur sur la matrice cr��e
 */
matrice* create_matrice(unsigned int largeur, unsigned int longueur);

/*
 * Fonction qui cr�e dynamiquement une image, dont le contenu est vide (i.e. sans �l�ment).
 * longueur : nombre de lignes de la matrice cr��e
 * largeur : nombre de colonnes
 * infos : header de l'image
 * Retour : un pointeur sur la matrice cr��e
 */
image* create_image(unsigned int largeur, unsigned int longueur, header  infos);


/*
 * Fonction qui modifie un �l�ment (pixel) dans la matrice en position (ligne,colonne)
  * Param�tre p_matrice : pointeur de la matrice (en acc�s lecture et �criture)
 * Param�tre pixel : pointeur du pixel � modifier
 * Param�tre l : ligne de la matrice
 * Param�tre c : colonne de la matrice
 */
void set_pixel(matrice* p_matrice, pixel const * p_pixel, unsigned int l, unsigned int c);

/*
 * Fonction qui r�cup�re la valeur d'un �l�ment(pixel) dans une matrice selon sa position
  * Param�tre p_matrice : la (pointeur en acc�s en lecture seule sur la) matrice dans laquelle acc�der � l'�l�ment
 * Param�tre l : la ligne de la position de l'�l�ment � acc�der (1�re position : index 0)
 * Param�tre c: la colonne de la position de l'�l�ment � acc�der (1�re position : index 0)
 * Retour : copie du pixel recherch�
 */
pixel get_pixel(matrice const * p_matrice, unsigned int l, unsigned int c);


/*
 * Fonction qui d�tuit la matrice et lib�re toute la m�moire associ�e.
 * Fonction qui d�salloue tout ce qui a �t� allou� pour la matrice
 * Param�tre p_matrice : la (pointeur sur la) matrice � d�tuire
 * Note : penser � mettre � NULL la variable pass�e en param�tre apr�s utilisation de cette fonction.
 */
void detruite_matrice(matrice* p_matrice);

/*
 * Fonction qui d�tuit l'image (matrice et header)
 * Fonction qui d�salloue tout ce qui a �t� allou� pour l'image
 * Param�tre p_image : la (pointeur sur l') image � d�tuire
 * Note : penser � mettre � NULL la variable pass�e en param�tre apr�s utilisation de cette fonction.
 */
void detruite_image(image* p_image);

/*
Fonction permettant de lire un fichier au format bmp et de remplir une matrice ainsi qu'un header 
associ�s � cette image
Param�tre nom_fichier : cha�ne de caract�res contenant le nom du fichier ex : "test.bmp"
Retour : la matrice associ�e
*/
image* lire_image(char* nom_fichier);

/*
* Fonction permettant de sauvegarder une matrice de pixels en fichier bmp
* p_image : pointeur pointant sur l'image (en lecture seule)
* nom_fichier : nom du fichier d'enregistrement
*/
void sauv_image(const image* p_image,char* nom_fichier);

/*
* Fonction permettant de convertir une image en niveaux de gris (chaque composante R,V,B d'un pixel a la m�me valeur)
* pour les traitements futurs
* p_matrice : pointeur pointnat sur la matrice � modifier
*/
void niveau_gris(matrice* p_matrice);
#endif