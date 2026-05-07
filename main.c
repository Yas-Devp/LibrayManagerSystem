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

typedef struct Pile{
    int nb_elem;
    int max;
    Livre *livres;
}Pile;

Pile undos;


void supprimer1erUndo(Pile *p){
    if(p->nb_elem >= p->max){
        for(int i=0; i < p->max - 1 ; i++){
            p->livres[i] = p->livres[i+1];
        }
        p->nb_elem--;
    }
}

void ajouterUndo(Pile *p, Livre livre){
    if(p->nb_elem >= p->max){
        //printf("undos reaches limit !");
        supprimer1erUndo(p);
        //return ;
    }
    p->livres[p->nb_elem] = livre;
    p->nb_elem ++;
}

void undoSupprimer(Library **lib, Pile *p){
    if (p->nb_elem == 0){
        printf("\nTu n'a pas supprime aucun livre !");
        return ;
    }

    Livre l_supprime = p->livres[p->nb_elem - 1];


    if(*lib == NULL){
        *lib = (Library*) malloc(sizeof(Library));
        (*lib)->livre = l_supprime;
        (*lib)->next = NULL ;
        return ;
    }

    Library* tmp = *lib;
    while(tmp != NULL){
        if(tmp->next == NULL){
            tmp->next = (Library*) malloc(sizeof(Library));
            tmp->next->livre = l_supprime;
            tmp->next->next = NULL;
            break;
        }

        tmp = tmp->next;
    }
    printf("\n livre avec le code %d a restaure !", l_supprime.code);
    p->nb_elem --;
}


void afficherMenu(){
    printf("\n\n|=========================[Library Manager]============================|\n");
    printf("|                                                                      |\n");
    printf("|   1 - Ajouter un nouveau livre                                       |\n");
    printf("|   2 - Afficher tous les livres                                       |\n");
    printf("|   3 - Rechercher un livre par code                                   |\n");
    printf("|   4 - Supprimer un livre                                             |\n");
    printf("|   5 - Restaurer le dernier livre supprime                            |\n");
    printf("|   6 - Modifier les informations d'un livre                           |\n");
    printf("|   7 - Afficher les livres disponibles                                |\n");
    printf("|   8 - Trier les livres par annee                                     |\n");
    printf("|   9 - Ajouter une demande d'emprunt                                  |\n");
    printf("|   10 - Afficher les demandes d'emprunt                               |\n");
    printf("|   11 - Traiter une demande d'emprunt                                 |\n");
    printf("|   12 - Quitter le programme                                          |\n");
    printf("|                                                                      |\n");
    printf("|======================================================================|\n\n");
}

Livre *chercherLivre(Library* lib, int code){
    Library *cur = lib;
    if(cur==NULL) return NULL ;

    while(cur != NULL){
        if(cur->livre.code == code) return &(cur->livre);
        cur = cur->next;
    }

    return NULL;
}

void supprimerLivre(Library **lib, int code){
    Library *cur = *lib;
    Library *tmp = cur ;
    if(cur == NULL) return ;

    if(cur->livre.code == code){
        *lib = cur->next;
        ajouterUndo(&undos, tmp->livre);
        free(tmp);
        return ;
    }

    while(cur->next != NULL){
        if(cur->next->livre.code == code){
            if(cur->next->next != NULL ){
                tmp = cur->next ;
                cur->next = tmp->next;
                ajouterUndo(&undos, tmp->livre);
                free(tmp);
                return ;
            }else{
                ajouterUndo(&undos, cur->next->livre);
                free(cur->next);
                cur->next = NULL ;
                return ;
            }
        }
        cur = cur->next;
    }
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

    do{
        printf("Entrer le code de Livre : ");
        scanf("%d", &code);
    }while(code <= 0 || chercherLivre(*lib , code) != NULL);

    do{
        printf("Entrer l'annee d'ecriture du Livre : ");
        scanf("%d", &annee);
    }while(annee <= 0);

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

void modifierLivre(Library **lib, int code){
    Livre *l = chercherLivre(*lib, code);
    if(l == NULL){
        printf("Livre non trouve!\n");
        return;
    }

    printf("\n\n================[Modifier Livre]==================\n\n");

    printf("ancien titre: %s\n", l->titre);
    printf("nouveau titre : ");
    gets(l->titre);

    printf("ancien auteur : %s\n", l->auteur);
    printf("nouveau auteur: ");
    gets(l->auteur);

    printf("ancienne annee : %d\n", l->annee);
    printf("nouvelle annee:  ");
    scanf("%d", &l->annee);

    printf("ancienne disponibilite : %d\n", l->disponible);
    printf("nouvelle disponibilite: ");
    scanf("%d", &l->disponible);
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

void disponibleLivres(Library *lib){
    Library* cur = lib;

    printf("\n\n|=========================Livres Disponibles============================|\n");
    printf("|                                                            |\n");
    while(cur != NULL){
        if (cur->livre.disponible > 0){
            printf("|             |=================================|            |\n");
            printf("|             | titre: %-24s |            |\n", cur->livre.titre);
            printf("|             | auteur: %-23s |            |\n", cur->livre.auteur);
            printf("|             | code: %-25d |            |\n",cur->livre.code);
            printf("|             | annee: %-24d |            |\n",cur->livre.annee);
            printf("|             | disponible: %-19d |            |\n",cur->livre.disponible);
            printf("|             |=================================|            |\n");
        }
        cur = cur->next ;
    }
    printf("|                                                            |\n");
    printf("|========================================================================|\n");
}

void trierParAnnee(Library *lib){
    if(lib == NULL) return;

    int swapped;
    Library *cur;
    Library *last = NULL;

    do {
        swapped = 0;
        cur = lib;

        while(cur->next != last){
            if(cur->livre.annee > cur->next->livre.annee){
                Livre temp = cur->livre;
                cur->livre = cur->next->livre;
                cur->next->livre = temp;
                swapped = 1;
            }
            cur = cur->next;
        }
        last = cur;
    } while(swapped);

    printf("\nLivres tries par annee avec succes!\n");
}

int main()
{
    //for undo feature setup
    undos.nb_elem = 0;
    undos.max = 5;
    undos.livres = (Livre*) malloc(undos.max * sizeof(Livre));


    //for library system
    afficherMenu();
    int choix = 0;
    Library* lib = NULL;
    int code_ = 0;

    do {
        printf("\n\n >> voitre choix : ");
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
        case 3:
            printf("Entrer le code de livre : ");

            scanf("%d", &code_);
            Livre *l = chercherLivre(lib, code_);
            code_ = 0;
            if(l == NULL){
                printf("le Livre n'est pas trouve !");
                break ;
            }
            printf("|             |=================================|            |\n");
            printf("|             | titre: %-24s |            |\n", l->titre);
            printf("|             | auteur: %-23s |            |\n", l->auteur);
            printf("|             | code: %-25d |            |\n",l->code);
            printf("|             | annee: %-24d |            |\n",l->annee);
            printf("|             | disponible: %-19d |            |\n", l->disponible);
            printf("|             |=================================|            |\n");
            break ;
        case 4:
            printf("Entrer le code de livre a supprimer : ");
            scanf("%d", &code_);
            supprimerLivre(&lib, code_);
            printf("DONE !");
            code_ = 0;
            break ;
        case 5:
            printf("\nRestauration du dernier livre suprime ...");
            undoSupprimer(&lib, &undos);
            printf("\nDONE !");
            break ;
        case 6:
            printf("Entrer le code de livre a modifier : ");
            scanf("%d", &code_);
            getchar();
            modifierLivre(&lib, code_);
            printf("DONE !");
            code_ = 0;
            break ;
        case 7:
            disponibleLivres(lib);
            break ;
        case 8:
            trierParAnnee(lib);
            afficherLibrary(lib);
            break ;
        }

    }while(choix != 12);


    return 0;
}
