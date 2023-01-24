#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MAX_SOMMET 100
#define MAX_ARETE 100
#define MAX_COULEUR 100

typedef struct Sommet Sommet;
struct Sommet
{
	int id;
	int x;
	int y;
	int cardinalite;
	int couleur;
	Sommet * voisin[MAX_SOMMET];
};

typedef struct Arete Arete;
struct Arete
{
	int id;
	Sommet * sommet1;
	Sommet * sommet2;
};

typedef struct Graphe Graphe;
struct Graphe
{
	int n;
	int m;
	Sommet * sommet[MAX_SOMMET];
	Arete * arete[MAX_ARETE];
};

Graphe * creerGraphe(int n, int m)
{
	Graphe * graphe = malloc(sizeof(Graphe));
	graphe->n = n;
	graphe->m = m;
	return graphe;
}

void detruireGraphe(Graphe * graphe)
{
	int i;
	for(i = 0; i < graphe->n; i++)
	{
		free(graphe->sommet[i]);
	}
	for(i = 0; i < graphe->m; i++)
	{
		free(graphe->arete[i]);
	}
	free(graphe);
}

void ajouterSommet(Graphe * graphe, int id, int x, int y)
{
	Sommet * sommet = malloc(sizeof(Sommet));
	sommet->id = id;
	sommet->x = x;
	sommet->y = y;
	sommet->cardinalite = 0;
	sommet->couleur = -1;
	graphe->sommet[id] = sommet;
}

void ajouterArete(Graphe * graphe, int id, int idSommet1, int idSommet2)
{
	Arete * arete = malloc(sizeof(Arete));
	
	arete->id = id;
	arete->sommet1 = graphe->sommet[idSommet1];
	arete->sommet2 = graphe->sommet[idSommet2];
	graphe->arete[id] = arete;
	graphe->sommet[idSommet1]->voisin[graphe->sommet[idSommet1]->cardinalite] = graphe->sommet[idSommet2];
	graphe->sommet[idSommet1]->cardinalite++;
	graphe->sommet[idSommet2]->voisin[graphe->sommet[idSommet2]->cardinalite] = graphe->sommet[idSommet1];
	graphe->sommet[idSommet2]->cardinalite++;
}

void afficherSommet(Sommet * sommet)
{
	printf("Sommet %d : (%d, %d) - Cardinalite : %d - Couleur : %d\n", sommet->id, sommet->x, sommet->y, sommet->cardinalite, sommet->couleur);
}

void afficherArete(Arete * arete)
{
	printf("Arete %d : Sommet %d - Sommet %d\n", arete->id, arete->sommet1->id, arete->sommet2->id);
}

void afficherGraphe(Graphe * graphe)
{
	int i;
	for(i = 0; i < graphe->n; i++)
	{
		afficherSommet(graphe->sommet[i]);
	}
	for(i = 0; i < graphe->m; i++)
	{
		afficherArete(graphe->arete[i]);
	}
}

void lireGraphe(Graphe * graphe, char * nomFichier)
{
	FILE * fichier = fopen(nomFichier, "r");
	if(fichier == NULL)
	{
		printf("Erreur lors de l'ouverture du fichier %s\n", nomFichier);
		exit(1);
	}
	
	int n, m;
	fscanf(fichier, "%d %d", &n, &m);
	
	int i;
	for(i = 0; i < n; i++)
	{
		int x, y;
		fscanf(fichier, "%d %d", &x, &y);
		ajouterSommet(graphe, i, x, y);
	}
	
	for(i = 0; i < m; i++)
	{
		int idSommet1, idSommet2;
		fscanf(fichier, "%d %d", &idSommet1, &idSommet2);
		ajouterArete(graphe, i, idSommet1, idSommet2);
	}
	
	fclose(fichier);
}

int comparerCardinalite(const void * sommet1, const void * sommet2)
{
	return ((Sommet *)sommet2)->cardinalite - ((Sommet *)sommet1)->cardinalite;
}

void colorierGraphe(Graphe * graphe)
{
	Sommet * sommets[graphe->n];
	memcpy(sommets, graphe->sommet, sizeof(Sommet *) * graphe->n);
	qsort(sommets
	, graphe->n
	, sizeof(Sommet *)
	, comparerCardinalite);
	
	int i;
	for(i = 0; i < graphe->n; i++)
	{
		int couleurUtilisee[MAX_COULEUR];
		memset(couleurUtilisee, 0, sizeof(int) * MAX_COULEUR);
		
		int j;
		for(j = 0; j < sommets[i]->cardinalite; j++)
		{
			couleurUtilisee[sommets[i]->voisin[j]->couleur] = 1;
		}
		
		int couleur = 0;
		while(couleurUtilisee[couleur] == 1)
		{
			couleur++;
		}
		
		sommets[i]->couleur = couleur;
	}
}

int main(int argc, char ** argv)
{
	if(argc != 2)
	{
		printf("Usage : %s <nomFichier>\n", argv[0]);
		exit(1);
	}
	
	Graphe * graphe = creerGraphe(0, 0);
	lireGraphe(graphe, argv[1]);
	
	colorierGraphe(graphe);
	afficherGraphe(graphe);
	
	detruireGraphe(graphe);
	
	return 0;
}
