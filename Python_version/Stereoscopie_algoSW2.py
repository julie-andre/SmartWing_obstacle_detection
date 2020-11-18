# -*- coding: utf-8 -*-
"""
Created on Fri May 10 17:08:12 2020

@author: julie

                             STEREOSCOPIE ALGO
                             SMARTWING  2019-2020
"""

import matplotlib.image as mpimg    # permet d'importer des images
import numpy as np                  # manipulation des images sous forme de matrice de pixels
import matplotlib.pyplot as plt     # permet d'afficher des images dans la console

# matrice de convolution pour le filtre de détection de contours
cov = np.array(([0,1,0],[1,-4,1],[0,1,0]))   

# Pour toutes les manipulations on ne considèrera pas les pixels en bordure de l'image
# On suppose également que les images traitées seront en niveaux de gris, ainsi
# on peut se contenter de seulement les pixels R (ou V ou B puisqu'ils ont les mêmes valeurs)
# Sinon il faudra appliquer un filtre de nveaux de gris afin d'homogénéiser les pixels
# et ne conserver qu'une seule dimensions(et non 3) pour plus de simplicité


# Conversion d'une image en niveau de gris
def niveauDeGris(im,sortie):
    val=0
    for i in range(im.shape[0]):
        for j in range(im.shape[1]):
            val=int(im[i,j][0])+ int(im[i,j][1]) + int(im[i,j][2])
            val=int(val/3)
            for k in range(3):
                sortie[i,j][k]=val
    #  return np.dot(im[...,:3],[0.33,0.33,0.34])

# Fonction utilisée pour la détection de contours, retourne la valeur du produit de convolution
def SommeProduitCov(im, cov):
    temp = im*cov    #  convolution des deux matrices
    cpt =0;
    for k in range(temp.shape[0]):
        for l in range(temp.shape[1]):
            cpt = cpt + temp[k,l] 
    if (cpt<0): cpt=0
    return cpt


# Retourne la matrice filtrée par détection de contours
def DetectionContours(img,sortie):
    im = np.zeros((3, 3), dtype=np.uint8)
    cpt=0
    for i in range(1,img.shape[0]-1):
        for j in range(1,img.shape[1]-1):
            im[0,0] = img[i-1,j-1][0]
            im[0,1] = img[i-1,j][0]
            im[0,2] = img[i-1,j+1][0]
            im[1,0] = img[i,j-1][0]
            im[1,1] = img[i,j][0]
            im[1,2] = img[i,j+1][0]
            im[2,0] = img[i+1,j-1][0]
            im[2,1] = img[i+1,j][0]
            im[2,2] = img[i+1,j+1][0]
            
            cpt=SommeProduitCov(im,cov)
            sortie[i,j] = (cpt,cpt,cpt)   # modifie les pixels R,V,B de la matrice de sortie
            

# Calcul la somme des carrés des différences entre les pixels de l'image G et les pixels de l'image D
def SommeCarreDiff(matG, matD):
    somme=0
    for i in range(matG.shape[0]):
        for j in range(matG.shape[1]):
            somme = somme + (int(matG[i,j])-int(matD[i,j]))**2
    return somme


# Parcourt les points de l'image gauche, détermine leur point homologue dans l'image droite
def PointHomologue(imG, imD, ligne, colonne):
    somme = -1
    absc = 0
    ordo = 0
    matG = np.array(([imG[ligne-1,colonne-1][0], imG[ligne-1,colonne][0],imG[ligne-1,colonne+1][0]],[imG[ligne,colonne-1][0],imG[ligne,colonne][0],imG[ligne,colonne+1][0]],[imG[ligne+1,colonne-1][0],imG[ligne+1,colonne][0],imG[ligne+1,colonne+1][0]]))
    matD = np.zeros((3,3), dtype=np.uint8)
    deb = ligne-2    # il se peut que les deux caméras ne soient pas parfaitement sur le même axe
    fin = ligne+2    # donc on parcourt également quelques lignes au dessus et en dessous, pour éviter les erreurs
    if (deb<1): deb=1
    if (fin>=imD.shape[0]-1): fin = imD.shape[0]-1
    for i in range(deb,fin): # réduit la zone de recherches en parcourant quelques lignes au dessus et en dessous de la ligne concernée
        for j in range(1,imD.shape[1]-1):  # on parcourt toutes les colonnes de la ligne i
            for lig in range(i-1,i+2):
                for col in range(j-1,j+2):
                    matD[lig-i+1,col-j+1] = imD[lig,col][0]  
                    #print(lig-i+1, col-j+1, sep = " ")
            s = SommeCarreDiff(matG,matD)    # calcul sur le nuage de pixels entourant le pixel [ligne,colonne]
            if(s<somme or somme<0): 
                somme = s
                absc = i
                ordo = j
    if(somme>10000): # Pose un seuil minimale pour "sommer" au delà duquel on consière que l'on a pas trouvé d'homologue 
        # somme = 10 000 correspond à un écart d'intensité moyen de 35 entre chaque pixel
        absc=colonne   # On retourne donc la ligne et la colonne du point de l'image gauche
        ordo=ligne
               
    return absc,ordo


''' Construit la matrice contenant, aux indices i et j, l'abscisse (et plus 
tard l'odonnée?) du point homologue du pt de coordonnées i et j dans l'image gauche
Calcule ensuite la distance en pixels entre 2 points homologues et retourne une carte de disparités'''
def MatDistances(imContoursG, imContoursD,seuil_intensite):
    mat = np.zeros((imContoursG.shape[0],imContoursG.shape[1]))
    for i in range(1,imContoursG.shape[0]-1):
        #print(i)
        for j in range(1,imContoursG.shape[1]-1):
            if(imContoursG[i,j][0]<seuil_intensite):   # l'intensité n'est pas jugée suffisante, on ne considère pas ce point comme un point d'intérêt
                mat[i,j]=j     # l'abscisse reste la même
            else:
                absc, ordo =PointHomologue(imContoursG,imContoursD,i,j)
                mat[i,j]=absc
            mat[i,j]=abs(mat[i,j]-j)
    return mat
            


'''retourne la distance en cm entre 2 points homologues, imH contient les 
abscisses des points homologues '''
def DistancePoints(matDistances, Dcam, Dfoc):   # Dcam => distance entre les foyers des 2 caméras, Dfoc => distance focale
    matDist = np.zeros((3,3))
    for i in range(1,matDistances.shape[0]-1): 
        for j in range(1,matDistances.shape[1]-1):
            # quand on aura la distance entre les deux caméras et la distance focale
            '''if(matDistances[i,j]!=0):
                matDist[i,j] = Dcam*Dfoc/matDistances[i,j]
            else:
                matDist[i,j] = Dcam*Dfoc/0.001  => très grande distance car pas de points homologues, ne nous intéresse pas'''
    return matDist



# Il faut un petit nombre de points d'intérêts étudiés sinon cela prend trop de temps
# D'où la condition imposée d'avoir une intensité d'au moins X pour un pixel dans l'image obtenue après
# avoir appliqué le filtre de détection des contours

def TestStereo():
    # On importe les images issues des caméras gauche et droite
    # Les deux images doivent avoir les mêmes dimensions, c'est également pourquoi
    # elles doivent provenir de deux caméras identiques,
    imgG = mpimg.imread("C:/Users/julie/Documents/école/PING/imGaucheFontRed.bmp")
    imgD = mpimg.imread("C:/Users/julie/Documents/école/PING/imDroiteFontRed.bmp")
    if imgG.dtype == np.float32: # Si le résultat n'est pas un tableau d'entiers
        imgG = (imgG * 255).astype(np.uint8)
    if imgD.dtype == np.float32: # Si le résultat n'est pas un tableau d'entiers
        imgD = (imgD * 255).astype(np.uint8)
    
    # Affichage pour visualiser 
    plt.imshow(imgG)
    plt.show()
    
    print(imgG.shape)
    print(imgD.shape)
    
    # On convertie les images en niveaux de gris
    # On appelle la méthode de détection de contours pour déterminer les points d'intérêt
    sortie1 = imgG.copy()
    sortie2=imgD.copy()
    niveauDeGris(imgG,sortie1)
    niveauDeGris(imgD,sortie2)
    
    temp1 = sortie1.copy()
    temp2=sortie2.copy()
    DetectionContours(temp1,sortie1)    
    DetectionContours(temp2,sortie2)        
    #plt.imshow(sortie1)
    #plt.show()
    
    # Affichage des points dont l'intensité est élevée dans l'image de contours
    seuil_intensite=150  # seuil d'intensité minimale
    cpt=0
    for i in range(1,sortie1.shape[0]-1):
        for j in range(1,sortie1.shape[1]-1):
            if(sortie1[i,j][0]>=seuil_intensite):
                cpt+=1
                print("[",i,",",j, "] =(",sortie1[i,j][0],",",sortie1[i,j][1],",",sortie1[i,j][2],")",sep=" ")
    print(cpt)
    
    # On récupère la matrice des disparités <=> matrice récapitulant les distances (en pixels)
    # trouvées entre un point de l'image de gauche et son point homologue de l'image droite
    seuil_intensite=150   # seuil d'intensité minimale
    mat=MatDistances(sortie1,sortie2,seuil_intensite)
    
    
#     cpt=0
#    for i in range(1,15):
#        for j in range(1,15):
#                print("[",i,",",j, "] =(",sortie1[i,j][0],",",sortie1[i,j][1],",",sortie1[i,j][2],")",sep=" ")
#                cpt+=1
#    print(cpt," points retournés")


    ''' Optionnel, permet de visualiser les résultats 
    ##############################################################################'''
    seuil = 1   # seuil de disparité, à déterminer de manière empirique selon 
    # le modèle des caméras pour ne pas se retrouver avec trop de points
    # On affiche les points de l'image ayant une forte disparité
    AfficherPointsForteDisparite(mat,seuil)
    
    # On enregistre la matrice des disparités (les disparités seront représentées par des pixels blancs)
    imagee=imgG.copy()
    for i in range (imgG.shape[0]):
        for j in range(imgG.shape[1]):
             for k in range(3):
                 imagee[i,j][k]= 255 if mat[i,j]>0 else 0
    mpimg.imsave("mat_disparitesNB.png",imagee)
    '''############################################################################'''
    
    
    return mat

 # Affiche les points de l'image ayant une forte disparité
def AfficherPointsForteDisparite(matrice,seuil):
    cpt=0
    for i in range(1,matrice.shape[0]-1):
        for j in range(1,matrice.shape[1]-1):
            if(matrice[i,j]>=seuil):
                print("Ligne :",i,"Colonne :",j, " disparité : ", matrice[i,j],sep=" ")
                cpt+=1
    print(cpt," points retournés")
    

if __name__ == '__main__':
    
    test=TestStereo()

   
    

    
''' Axes d'amélioration :
    Pour faciliter la détermination du seuil de disparité ou d'intensité => faire une étude
    statistique de l'intensité générale des points de l'image puis déterminer une intensité
    moyenne ou quartilienne (3eme quartile par exemple) qui sera le seuil
    => inconvénient, peut rajouter du temps de calcul
    
    Une fois les modèles de caméras déterminés => obtenir la distance focale des caméras
    (données par le fournisseur si statique, sinon à déterminer empiriquement par des tests simples:
        cf cours de physique de première S sur les lentilles)
        
    Transcrire ce code dans un autre langage pour améliorer le temps de calcul
    
    Peut être passer par une biliothèque pour déterminer la détection de contours des images
    au lieu de le faire avec l'algo de ce code pour améliorer le temps d'execution
    
    Le programme conserve pour le momment toutes les dimensions des images (composantes R,V,B)
    afin d'appliquer si besoin un filtre de niveaux de gris => sinon inutile de conserver les trois
    Néanmoins les calculs ne prennent en compte que la première dimension et non les 3
    dans l'hypothèse où les images sont en niveaux de gris ou en tout cas que les 3 composantes
    ont sensiblement les mêmes valeurs'''