# include "image.h"
# include <stdlib.h>
# include <stdio.h>

matrice* create_matrice(unsigned int largeur, unsigned int longueur)
{
	matrice* im = (matrice*)malloc(sizeof(matrice));
	im->largeur = 0;
	im->longueur = 0;
	if (largeur > 0 && longueur > 0)
	{
		if (im != NULL)
		{
			im->largeur = largeur;
			im->longueur = longueur;
			im->content = (pixel**)malloc(longueur * sizeof(pixel));
			if (im->content != NULL)
			{
				for (int i = 0; i < longueur;i++)
				{
					im->content[i] = (pixel*)malloc(largeur * sizeof(pixel));
				}
			}
		}
	}
	return im;
}

image* create_image(unsigned int largeur, unsigned int longueur, header infos)
{
	image* im = NULL;
	matrice* m = create_matrice(largeur, longueur);
	if (m!=NULL)
	{
		im = (image*)malloc(sizeof(image));
		if (im != NULL)
		{
			im->infos = infos;
			im->mat = m;
		}
	}
	return im;
}


void set_pixel(matrice* p_matrice, pixel const* p_pixel, unsigned int l, unsigned int c)
{
	if (p_matrice != NULL && p_pixel != NULL)
	{
		if (l >= 0 && l<p_matrice->longueur && c >= 0 && c < p_matrice->largeur)
		{
			p_matrice->content[l][c] = *p_pixel;
		}	
	}
}

pixel get_pixel(matrice const* p_matrice, unsigned int l, unsigned int c)
{
	pixel pix = { 0,0,0 };
	if (p_matrice != NULL)
	{
		if (l >= 0 && l < p_matrice->longueur && c >= 0 && c < p_matrice->largeur)
		{
			pix = p_matrice->content[l][c];
		}
	}
	return pix;
}

void detruite_matrice(matrice* p_matrice)
{
	if (p_matrice != NULL)
	{
		if (p_matrice->content != NULL)
		{
			for (int i = 0; i < p_matrice->longueur;i++)
			{
				free(p_matrice->content[i]);
			}
			free(p_matrice->content);
		}
		free(p_matrice);
	}
}

void detruite_image(image* p_image)
{
	detruite_matrice(p_image->mat);
	//free(&(p_image->infos));
}

// Va chercher les fichiers dans le répertoire:\SM_Stereo\SM_Stereo
image* lire_image(char* nom_fichier)
{
	image* im = NULL;

	FILE* fichier = NULL;
	errno_t err = fopen_s(&fichier, nom_fichier, "rb");

	header infos;
	pixel pix;
	unsigned char chainepix[3];		// on ne veut des nombres qu'entre 0 et 255 et non -127 et +128
	int octets_to_add;		// nbre d'octets à ajouter en fin de ligne si le nombre total d'octects de la ligne n'est pas
							// un multiple de 4 comme l'impose le format bmp
	char octets[4] = { 0,3,2,1 };		// contient les 4 possibilités du nombre d'octects à ajouter
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

		octets_to_add = octets[(3 * infos.width) % 4];
		for (int i = (im->mat)->longueur - 1; i >= 0; i--)
		{
			for (int j = 0; j < (im->mat)->largeur;j++)
			{
				fread(&chainepix, 1, 3, fichier);
				pix.b = (int)chainepix[0];
				pix.v = (int)chainepix[1];
				pix.r = (int)chainepix[2];
				set_pixel(im->mat, &pix, i, j);
			}
			fread(&chainepix, 1, octets_to_add, fichier);
		}

		fclose(fichier); // fermeture du fichier

	}
	else
	{
		printf("Probleme lors de l'ouverture du fichier %s\n", nom_fichier);
	}

	return im;
}

void sauv_image(const image* p_image, char* nom_fichier)
{
	if (p_image != NULL)
	{
		FILE* fichier = NULL;
		errno_t err = fopen_s(&fichier, nom_fichier, "wb");

		pixel pix;
		unsigned char chainepix[3];
		int octets_to_add;		// nbre d'octets à ajouter en fin de ligne si le nombre total d'octects de la ligne n'est pas
							// un multiple de 4 comme l'impose le format bmp
		char octets[4] = { 0,3,2,1 };		// contient les 4 possibilités du nombre d'octects à ajouter
		if (err == 0)		// ou if(fichier!=NULL)
		{

			fwrite(&((p_image->infos)).typeFichier[0], 1, 1, fichier);
			fwrite(&((p_image->infos)).typeFichier[1], 1, 1, fichier);
			fwrite(&((p_image->infos)).taille, 4, 1, fichier);
			fwrite(&((p_image->infos)).rsv, 4, 1, fichier);
			fwrite(&((p_image->infos)).offsetim, 4, 1, fichier);
			fwrite(&((p_image->infos)).size_imhead, 4, 1, fichier);
			fwrite(&((p_image->infos)).width, 4, 1, fichier);
			fwrite(&((p_image->infos)).height, 4, 1, fichier);
			fwrite(&((p_image->infos)).nbplans, 2, 1, fichier);
			fwrite(&((p_image->infos)).bpp, 2, 1, fichier);
			fwrite(&((p_image->infos)).compression, 4, 1, fichier);
			fwrite(&((p_image->infos)).sizeim, 4, 1, fichier);
			fwrite(&((p_image->infos)).hres, 4, 1, fichier);
			fwrite(&((p_image->infos)).vres, 4, 1, fichier);
			fwrite(&((p_image->infos)).cpalette, 4, 1, fichier);
			fwrite(&((p_image->infos)).cIpalette, 4, 1, fichier);

			octets_to_add = octets[(3 * (p_image->infos).width) % 4];
			for (int i = (p_image->mat)->longueur - 1; i >= 0; i--)
			{
				for (int j = 0; j < (p_image->mat)->largeur;j++)
				{
					pix = get_pixel(p_image->mat, i, j);
					chainepix[0] = pix.b;
					chainepix[1] = pix.v;
					chainepix[2] = pix.r;
					fwrite(&chainepix, 1, 3, fichier);

				}
				chainepix[0] = chainepix[1] = chainepix[2] = 0;
				fwrite(&chainepix, 1, octets_to_add, fichier);
			}
			fclose(fichier); // fermeture du fichier
		}
		else
		{
			printf("Probleme lors de l'ouverture du fichier %s\n", nom_fichier);
		}
	}
}

void niveau_gris(matrice* p_matrice)
{
	if (p_matrice != NULL)
	{
		pixel pix;
		int val;
		for (int i = 0;i < p_matrice->longueur;i++)
		{
			for (int j = 0; j < p_matrice->largeur;j++)
			{
				val = p_matrice->content[i][j].r + p_matrice->content[i][j].v + p_matrice->content[i][j].b;
				val /= 3;
				pix.r = val;
				pix.v = val;
				pix.b = val;
				set_pixel(p_matrice, &pix, i, j);

			}
		}
	}
}