#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Livre{
    char titre[50];
    char auteur[50];
    int code;
    int annee;
    int disponible;
} Livre;

typedef struct Library{
    Livre livre;
    struct Library *next;
} Library;

void afficherMenu(){
    printf("\n\n|=========================[Library Manager]============================|\n");
    printf("|                                                                      |\n");
    printf("|   1 - Ajouter un nouveau livre                                       |\n");
    printf("|   2 - Afficher tous les livres                                       |\n");
    printf("|   3 - Rechercher un livre par code                                   |\n");
    printf("|   4 - Supprimer un livre                                             |\n");
    printf("|   5 - Restaurer le dernier livre supprimé                            |\n");
    printf("|   6 - Modifier les informations d’un livre                           |\n");
    printf("|   7 - Afficher les livres disponibles                                |\n");
    printf("|   8 - Trier les livres par année                                     |\n");
    printf("|   9 - Ajouter une demande d’emprunt                                  |\n");
    printf("|   10 - Afficher les demandes d’emprunt                               |\n");
    printf("|   11 - Traiter une demande d’emprunt                                 |\n");
    printf("|   12 - Quitter le programme                                          |\n");
    printf("|                                                                      |\n");
    printf("|======================================================================|\n\n");
}


void ajouterLivre(Library **lib){
    char titre[50];
    char auteur[50];
    int code = 0;
    int annee = 0;
    int disponible = 0;

    printf("\n\n================[Ajouter un Livre]==================\n\n");
    printf("Entrer le titre de livre : ");
    gets(titre);
    printf("Enter le nom de l'auteur : ");
    gets(auteur);
    printf("Entrer le code de Livre : ");
    scanf("%d", &code);
    printf("Entrer l'annee d'ecriture du Livre : ");
    scanf("%d", &annee);
    printf("Entrer la quantite disponible de Livre : ");
    scanf("%d", &disponible);

    Livre *newLivre =(Livre*) malloc(sizeof(Livre));
    strcpy(newLivre->titre , titre);
    strcpy(newLivre->auteur, auteur);
    newLivre->code = code;
    newLivre->annee = annee;
    newLivre->disponible= disponible;

    if(*lib == NULL){
        *lib = (Library*) malloc(sizeof(Library));
        (*lib)->livre = *newLivre;
        (*lib)->next = NULL ;
        return ;
    }

    Library* tmp = *lib;
    while(tmp != NULL){
        if(tmp->next == NULL){
            tmp->next = (Library*) malloc(sizeof(Library));
            tmp->next->livre = *newLivre;
            tmp->next->next = NULL;
            break;
        }

        tmp = tmp->next;
    }

    free(newLivre);
}

void afficherLibrary(Library *lib){
    Library* cur = lib;

    printf("\n\n|=========================Library============================|\n");
    printf("|                                                            |\n");
    while(cur != NULL){
        printf("|             |=================================|            |\n");
        printf("|             | titre: %-24s |            |\n", cur->livre.titre);
        printf("|             | auteur: %-23s |            |\n", cur->livre.auteur);
        printf("|             | code: %-25d |            |\n",cur->livre.code);
        printf("|             | annee: %-24d |            |\n",cur->livre.annee);
        printf("|             | disponible: %-19d |            |\n",cur->livre.disponible);
        printf("|             |=================================|            |\n");
        cur = cur->next ;
    }
    printf("|                                                            |\n");
    printf("|============================================================|\n");
}

int main()
{
    afficherMenu();
    int choix = 0;
    Library* lib = NULL;


    do {
        printf(" >> voitre choix : ");
        scanf("%d", &choix);
        if(choix > 12 || choix <= 0){
            printf("l'operation ou le service %d n'exist pas !", choix);
        }
        if(choix == 12) break ;

        switch(choix){
        case 1:
            getchar();
            ajouterLivre(&lib);
            break;
        case 2:
            afficherLibrary(lib);
            break ;
        }

    }while(choix != 12);

    return 0;
}
