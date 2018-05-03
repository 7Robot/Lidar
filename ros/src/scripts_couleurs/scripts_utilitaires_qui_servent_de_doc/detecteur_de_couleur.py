

#### Méthodes que j'ai mis parceque je sais pas importer un autre script python sans jouer avec le Shells d'anaconda.

### Classifieurs :

Code_Couleur = [1,2,3,4,5]
#couleur = ['orange','jaune','vert','bleu','noir']
Intervales = [40,70,130,340] 
#""" Frontière : [orange,jaune,vert,bleu,orange] """
#Centroid = [50,0.3,0.02] 
""" Centroïd noir dans l'espace [Teine, Saturation, Valeur] """ 
Seuil = [100,0.4,0.05] 
""" Techniquement on peut dire que le noir ce situe dans le pavé de point diagonalement oposé : [0,0,0] et Seuil. 
Ma première méthode de classification va donc se basé sur l'inclusion de la mesure dans ce pavé"""

# Classifieur par angle :
def ClasfColor(TSV) :
    for i in range(0,len(Intervales)) :
        
        if (TSV[0] < Intervales[i]) :
            return Code_Couleur[i]
            
    return Code_Couleur[0]
            
            
# Classifieur de noir :
def ClasfNoir(TSV) :
    return ( (Seuil[0] > TSV[0]) and (Seuil[1] > TSV[1]) and (Seuil[2] > TSV[2]) )  

# Classifieur general

def Classifieur(TSV) :
    couleur = ClasfColor(TSV)
    if ClasfNoir(TSV):
        couleur = Code_Couleur[4]
    return couleur

### Convertisseur TSV
## Fonction qui retourne le code TSV  en fonction du RVB :

def RVB2TSV(RVB):
    
    Max = max(RVB)
    Min = min(RVB)
    Ecart = Max -Min
    
    # Calcul du T :
    if Ecart == 0 :
        T = 0
    elif (RVB[0] == Max) :
        T =  ((60* (RVB[1] - RVB[2]) / Ecart + 360) % 360)
    elif(RVB[1] == Max):
        T = (60* (RVB[2] - RVB[0]) / Ecart + 120)
    else :
        T = (60* (RVB[0] - RVB[1]) / Ecart + 240)
        
    # Ajout du T et V:
    TSV = [T,0,Max]
    
    # Calcul du S :
    if (Max != 0):
        TSV[1] = (1-Min/Max)
    return TSV

## Fonction qui donne le vecteur RVB/255 associé au vecteur RVB aux valeurs comprises entre 0 et 255 :

def RVBNormalisateur(RVB):
    RVBnorme = [0,0,0]
    for i in range(0,3):
        RVBnorme[i] = RVB[i] / 255
    return RVBnorme
    
### Methode principale : 

def RVBClassifieur(RVB) :
    
    TSV = RVB2TSV(RVBNormalisateur(RVB))
    print(TSV)
    return Classifieur(TSV)
    
    
    
    
