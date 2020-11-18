// SM_Stereo.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

# include<stdlib.h>
#include <stdio.h>
# include <math.h>

#include "image.h"
# include "image.c"

#pragma pack(1)  // desactive l'alignement mémoire

image * lire_image2(char * nom_fichier)
{
	image* im = NULL;

	
	FILE* fichier = NULL;
	//errno_t err = fopen_s(&fichier, nom_fichier, "w");
	// Mets le fichier dans :\SM_Stereo\SM_Stereo

	errno_t err = fopen_s(&fichier, nom_fichier, "rb");
	printf("%d\n", err);
	printf("%p\n", fichier);
	
	header infos;
	pixel pix;
	unsigned char chainepix[3];		// on ne veut des nombres qu'entre 0 et 255 et non -127 et +128
	if (err == 0)		// ou if(fichier!=NULL)
	{

		fread(&infos.typeFichier, 2, 1, fichier);
		infos.typeFichier[2] = '\0';
		fread(&infos.taille, 4, 1, fichier);
		fread(&infos.rsv, 4, 1, fichier);
		fread(&infos.offsetim, 4, 1, fichier);
		fread(&infos.size_imhead, 4, 1, fichier);
		fread(&infos.width, 4, 1, fichier);
		fread(&infos.height, 4, 1, fichier);
		fread(&infos.nbplans, 2, 1, fichier);
		fread(&infos.bpp, 2, 1, fichier);
		fread(&infos.compression, 4, 1, fichier);
		fread(&infos.sizeim, 4, 1, fichier);
		fread(&infos.hres, 4, 1, fichier);
		fread(&infos.vres, 4, 1, fichier);
		fread(&infos.cpalette, 4, 1, fichier);
		fread(&infos.cIpalette, 4, 1, fichier);

		im = create_image(infos.width, infos.height, infos);
		
		
		for (int i = (im->mat)->longueur - 1; i >= 0; i--)
		{
			for (int j = 0; j < (im->mat)->largeur;j++)
			{
				fread(&chainepix, 1, 3, fichier);
				pix.b = (int)chainepix[0];
				pix.v = (int)chainepix[0];
				pix.r = (int)chainepix[0];
				/*printf("Pixel bleu : %d\n", pix.b);
				printf("Pixel vert : %d\n", pix.b);
				printf("Pixel rouge: %d\n", pix.b);*/
				set_pixel(im->mat, &pix, i, j);
				printf("[%d,%d,%d]\n", pix.b, pix.v, pix.r);
			}
		}
		

		fclose(fichier); // fermeture du fichier

	}
	else
	{
		printf("Probleme lors de l'ouverture du fichier %s\n", nom_fichier);
	}

	printf(infos.typeFichier);
	printf("\n");
	printf("taille du fichier : %d espace reserve : %d  offsetim : %d \n", infos.taille, infos.rsv, infos.offsetim);
	printf("taille du header : %d  Largeur image : %d  Hauteur : %d \n", infos.size_imhead, infos.width, infos.height);
	printf("nbre de plans : %hd nbre bits par couleur : %hd  compression : %d\n", infos.nbplans, infos.bpp, infos.compression);
	printf("taille de l'image : %d\n", infos.sizeim);
	printf("Resolution horiz : %d  Resolution vertic : %d  Nbre de couleurs palette : %d  Nbre coul importantes : %d\n", infos.hres, infos.vres, infos.cpalette, infos.cIpalette);
	
	return im;
}


void sauv_image2(const image * im)
{
	if (im != NULL )
	{
		char nom_fichier[] = "lenaS.bmp";
		FILE* fichier = NULL;

		errno_t err = fopen_s(&fichier, nom_fichier, "wb");
		printf("%d\n", err);
		printf("%p\n", fichier);

		pixel pix;
		unsigned char chainepix[3];
		if (err == 0)		// ou if(fichier!=NULL)
		{
			
			fwrite(&((im->infos)).typeFichier[0], 1, 1, fichier);
			fwrite(&((im->infos)).typeFichier[1], 1, 1, fichier);
			fwrite(&((im->infos)).taille, 4, 1, fichier);
			fwrite(&((im->infos)).rsv, 4, 1, fichier);
			fwrite(&((im->infos)).offsetim, 4, 1, fichier);
			fwrite(&((im->infos)).size_imhead, 4, 1, fichier);
			fwrite(&((im->infos)).width, 4, 1, fichier);
			fwrite(&((im->infos)).height, 4, 1, fichier);
			fwrite(&((im->infos)).nbplans, 2, 1, fichier);
			fwrite(&((im->infos)).bpp, 2, 1, fichier);
			fwrite(&((im->infos)).compression, 4, 1, fichier);
			fwrite(&((im->infos)).sizeim, 4, 1, fichier);
			fwrite(&((im->infos)).hres, 4, 1, fichier);
			fwrite(&((im->infos)).vres, 4, 1, fichier);
			fwrite(&((im->infos)).cpalette, 4, 1, fichier);
			fwrite(&((im->infos)).cIpalette, 4, 1, fichier);

			for (int i = (im->mat)->longueur - 1; i >= 0; i--)
			{
				for (int j = 0; j < (im->mat)->largeur;j++)
				{
					pix = get_pixel(im->mat, i, j);
					chainepix[0] = pix.b;
					chainepix[1] = pix.v;
					chainepix[2] = pix.r;
					fwrite(&chainepix, 1, 3, fichier);
					
				}
				
			}
			fclose(fichier); // fermeture du fichier
		}
		else
		{
			printf("Probleme lors de l'ouverture du fichier %s\n", nom_fichier);
		}
	}
}

int somme_produit_conv(matrice* extrait, matrice*noyau)
{
	int cpt = 0;
	if (extrait != NULL && noyau != NULL && extrait->longueur== noyau->longueur && extrait->largeur==noyau->largeur)
	{
		for (int i = 0; i < extrait->longueur;i++)
		{
			for (int j = 0; j < extrait->largeur;j++)
			{
				cpt += (extrait->content[i][j].r) * (noyau->content[i][j].r);
			}
		}
		if (cpt < 0)
		{
			//printf("Valeur pixel : %d\n", cpt);
			cpt = 0;		// un pixel ne peut être négatif
		}
	}
	return cpt;
}



//Prérequis image et sortie ont les mêmes dimensions
void detection_contours(matrice const * im, matrice * sortie)
{
	if (sortie != NULL && sortie->largeur == im->largeur && sortie->longueur == im->longueur)
	{
		int cpt;
		pixel pix;
		header* h = NULL;
		matrice * extrait = create_matrice(3, 3);
		matrice* noyau = create_matrice(3, 3);

		noyau->content[0][0].r = 0;
		noyau->content[0][1].r = 1;
		noyau->content[0][2].r = 0;
		noyau->content[1][0].r = 1;
		noyau->content[1][1].r = -4;
		noyau->content[1][2].r = 1;
		noyau->content[2][0].r = 0;
		noyau->content[2][1].r = 1;
		noyau->content[2][2].r = 0;

		for (int i = 1;i < im->longueur-1;i++)		// on ne parcourt pas les bords de l'image
		{
			for (int j = 1; j < im->largeur-1;j++)
			{
				for (int k = -1;k <= 1;k++)
				{
					for (int l = -1;l <= 1;l++)
					{
						set_pixel(extrait, &(im->content[i + k][j + l]), k + 1, l + 1);
					}
				}
				cpt = somme_produit_conv(extrait, noyau);
				pix = { cpt,cpt,cpt };
				set_pixel(sortie, &pix, i, j);		// ou set_pixel(sortie, new pixel{ cpt,cpt,cpt }, i, j)
			}
		}

		detruite_matrice(extrait);
		detruite_matrice(noyau);
		extrait = NULL;
		noyau = NULL;
	}
}

// Prérequis tous les composantes d'un pixel ont la même valeur cad l'image est en niveaux de gris
double somme_carre_diff(matrice* matG, matrice* matD)
{
	double somme = 0;
	if (matG->longueur == matD->longueur && matG->largeur == matD->largeur)
	{
		for (int i = 0; i < matG->longueur;i++)
		{
			for (int j = 0; j < matG->largeur;j++)
			{
				somme += pow((double)(matG->content[i][j].r) - (double)(matD->content[i][j].r), 2);
			}
		}
	}
	return somme;
}

// Prérequis imG et imD ont les meme dimensions
int* point_homologue(matrice const* imG, matrice const* imD, int ligne, int colonne)
{
	double somme = -1;
	double s=0;
	int absc = 0;
	int ordo = 0;
	pixel pix;
	matrice* matG = create_matrice(3, 3);
	matrice* matD = create_matrice(3, 3);
	for (int i = -1;i <= 1; i++)
	{
		for (int j = -1;j <= 1;j++)
		{
			pix = get_pixel(imG, ligne + i, colonne + j); 
			set_pixel(matG, &pix, i + 1, j + 1);
		}
	}

	int deb = ligne - 2;	//il se peut que les deux caméras ne soient pas parfaitement sur le même axe
	int fin = ligne + 2;	//donc on parcourt également quelques lignes au dessus et en dessous, pour éviter les erreurs
	if (deb < 1)
	{
		deb = 1;
	}
	if (fin > imD->longueur)
	{
		fin = imD->longueur - 1;
	}
	for (int i = deb; i < fin;i++)	// réduit la zone de recherches en parcourant quelques lignes au dessus et en dessous de la ligne concernée
	{
		for (int j = 1;j < imD->largeur - 1;j++)	//on parcourt toutes les colonnes de la ligne i
		{
			for (int lig = i - 1; lig <= i + 1;lig++)
			{
				for (int col = j - 1; col <= j + 1;col++)
				{
					pix = get_pixel(imD, lig, col);
					set_pixel(matD, &pix, lig-i + 1, col-j + 1);
				}
			}
			s = somme_carre_diff(matG, matD);	// calcul sur le nuage de pixels entourant le pixel [ligne,colonne]
			if (s < somme || somme < 0)
			{
				somme = s;
				absc = j;
				ordo = i;
			}
		}
	}
	if (somme > 10000)	// Pose un seuil minimale pour "sommer" au delà duquel on consière que l'on a pas trouvé d'homologue
	// somme = 10 000 correspond à un écart d'intensité moyen de 35 entre chaque pixel
	{
		absc = colonne;		// On retrourne donc la ligne et la colonne du point de l'image gauche
		ordo = ligne;
		printf("Seuil atteint\n");
	}

	detruite_matrice(matG);
	detruite_matrice(matD);
	matG = NULL;
	matD = NULL;

	int retour[] = { absc, ordo };
	printf("absc : %d\n", absc);
	return retour;
}

matrice* matrice_distances(matrice* contoursG, matrice* contoursD, int seuil_intensite)
{
	matrice* mat = NULL;
	//if (contoursG->longueur == contoursD->longueur && contoursG->largeur == contoursD->largeur)
	//{
		mat = create_matrice(contoursG->largeur, contoursG->longueur);
		pixel pix;
		int* coord;
		int absc;
		for (int i = 1; i < contoursG->longueur-1;i++)
		{
			for (int j = 1; j < contoursG->largeur-1;j++)
			{
				if(contoursG->content[i][j].r < seuil_intensite || i==0 || j ==0 || i== contoursG->longueur-1 || j == contoursG->largeur - 1)
				{
					pix = { 0,0,0 };
					set_pixel(mat, &pix, i, j);
				}
				else
				{
					coord = point_homologue(contoursG, contoursD, i, j);
					absc = abs(coord[0]-j);
					pix = { absc,absc,absc };
					set_pixel(mat, &pix, i, j);
					/*if (absc > 0)
					{
						printf("[%d,%d]= %d\t %d\n", i, j, absc, coord[0]);
					}*/
				}
			}
		}
	//}
	return mat;
}

int main()
{
	// imG fontaine a une ligne en moins par rapport à imD, donc les appeler dans cet ordre pas de pb
	char nom_fichier[] = "imGaucheFontRed.bmp";
	char nom_fichier1[] = "imDroiteFontRed.bmp";
	image * imG =lire_image(nom_fichier);
	image* imD = lire_image(nom_fichier1);
	niveau_gris(imG->mat);
	niveau_gris(imD->mat);

	image* contoursG = create_image(imG->mat->largeur, imG->mat->longueur, imG->infos);
	detection_contours(imG->mat, contoursG->mat);
	image* contoursD = create_image(imD->mat->largeur, imD->mat->longueur, imD->infos);
	detection_contours(imD->mat, contoursD->mat);

	image* disparites = create_image(imG->mat->largeur, imG->mat->longueur, imG->infos);
	disparites->mat = matrice_distances(contoursG->mat, contoursD->mat, 150);
	
	char nom_fichier2[] = "fontaineDisparites.bmp";
	sauv_image(disparites, nom_fichier2);


	int cpt = 0;
	for (int i = 1; i < disparites->mat->longueur-1; i++)
	{
		for (int j = 1; j < disparites->mat->largeur - 1;j++)
		{
			if (disparites->mat->content[i][j].r >=1)
			{
				printf("[%d,%d]= %d\n", i, j, disparites->mat->content[i][j].r);
				cpt++;
			}
			
		}
	}
	printf("%d points retournés\n", cpt);

	//lire_image(nom_fichier2);

	// On libère la mémoire dans le cas où le main serait appelé plusieurs fois
	detruite_image(imG);
	detruite_image(imD);
	detruite_image(contoursG);
	detruite_image(contoursD);
	detruite_image(disparites);
	imG = NULL;
	imD = NULL;
	contoursG = NULL;
	contoursD = NULL;
	disparites = NULL;
	

    return 0;
}

