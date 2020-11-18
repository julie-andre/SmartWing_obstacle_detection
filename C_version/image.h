#pragma once

#ifndef IMAGE_H
# define IMAGE_H

struct _header
{
	char typeFichier[3];		// contient 'B' , 'M'  et '\0' annonçant la fin de la chaine
	int taille;
	int rsv;		// champs réservé, 0 par défaut
	int offsetim;		// indice auquel commence le contenu de l'image
	int size_imhead;	// taille entête
	int width;			// largeur de l'image
	int height;			// longueur de l'image
	short nbplans;		// nombre de plans : toujours 1
	short bpp;		// nombre de bits par couleur, on ne supporte que les les bpp de 24 bits
	int compression;	// 0 si non compressé ; on ne gère que les fichiers non compressés
	int sizeim;			// taille totale de l'image
	int hres;			// résolution horizontale
	int vres;			// résolution verticale
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
 * Fonction qui crée dynamiquement une matrice, dont le contenu est vide (i.e. sans élément).
 * longueur : nombre de lignes de la matrice créée
 * largeur : nombre de colonnes
 * Retour : un pointeur sur la matrice créée
 */
matrice* create_matrice(unsigned int largeur, unsigned int longueur);

/*
 * Fonction qui crée dynamiquement une image, dont le contenu est vide (i.e. sans élément).
 * longueur : nombre de lignes de la matrice créée
 * largeur : nombre de colonnes
 * infos : header de l'image
 * Retour : un pointeur sur la matrice créée
 */
image* create_image(unsigned int largeur, unsigned int longueur, header  infos);


/*
 * Fonction qui modifie un élément (pixel) dans la matrice en position (ligne,colonne)
  * Paramètre p_matrice : pointeur de la matrice (en accès lecture et écriture)
 * Paramètre pixel : pointeur du pixel à modifier
 * Paramètre l : ligne de la matrice
 * Paramètre c : colonne de la matrice
 */
void set_pixel(matrice* p_matrice, pixel const * p_pixel, unsigned int l, unsigned int c);

/*
 * Fonction qui récupère la valeur d'un élément(pixel) dans une matrice selon sa position
  * Paramètre p_matrice : la (pointeur en accès en lecture seule sur la) matrice dans laquelle accéder à l'élément
 * Paramètre l : la ligne de la position de l'élément à accéder (1ère position : index 0)
 * Paramètre c: la colonne de la position de l'élément à accéder (1ère position : index 0)
 * Retour : copie du pixel recherché
 */
pixel get_pixel(matrice const * p_matrice, unsigned int l, unsigned int c);


/*
 * Fonction qui détuit la matrice et libère toute la mémoire associée.
 * Fonction qui désalloue tout ce qui a été alloué pour la matrice
 * Paramètre p_matrice : la (pointeur sur la) matrice à détuire
 * Note : penser à mettre à NULL la variable passée en paramètre après utilisation de cette fonction.
 */
void detruite_matrice(matrice* p_matrice);

/*
 * Fonction qui détuit l'image (matrice et header)
 * Fonction qui désalloue tout ce qui a été alloué pour l'image
 * Paramètre p_image : la (pointeur sur l') image à détuire
 * Note : penser à mettre à NULL la variable passée en paramètre après utilisation de cette fonction.
 */
void detruite_image(image* p_image);

/*
Fonction permettant de lire un fichier au format bmp et de remplir une matrice ainsi qu'un header 
associés à cette image
Paramètre nom_fichier : chaîne de caractères contenant le nom du fichier ex : "test.bmp"
Retour : la matrice associée
*/
image* lire_image(char* nom_fichier);

/*
* Fonction permettant de sauvegarder une matrice de pixels en fichier bmp
* p_image : pointeur pointant sur l'image (en lecture seule)
* nom_fichier : nom du fichier d'enregistrement
*/
void sauv_image(const image* p_image,char* nom_fichier);

/*
* Fonction permettant de convertir une image en niveaux de gris (chaque composante R,V,B d'un pixel a la même valeur)
* pour les traitements futurs
* p_matrice : pointeur pointnat sur la matrice à modifier
*/
void niveau_gris(matrice* p_matrice);
#endif