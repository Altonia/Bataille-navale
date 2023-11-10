#include<iostream>
#include<array>
#include<cmath>
#include<string>
#include<cstdlib>
#include<ctype.h>
#include<ctime>

const int nbateau = 5;
const int ligne = 10;
const int colonne = 10;

using tab = std::array<int, colonne>;
using grille = std::array<tab, ligne>;


struct joueur {
    std::string nomJ;//nom joueur
    grille grille_bataille;//grille où on affiche les emplacements des bateaux du joueurs
    grille grille_info;//grille où le joueurs attack
};

struct bateau {
    std::string name;//nom joueur
    int L;//longeur des bateaux
}
bateau[nbateau];

struct coordonee
{
    int i;
    int j;
    char sense;// sense du bateau (h if horzontale v if verticale)
};

void caracteristique()
{

    bateau[0].name = "porteavion";
    bateau[1].name = "croiseur";
    bateau[2].name = "contre-troupilleur";
    bateau[3].name = "contre-troupilleur";
    bateau[4].name = "troupilleure";
    bateau[0].L = 5;
    bateau[1].L = 4;
    bateau[2].L = 3;
    bateau[3].L = 3;
    bateau[4].L = 2;
}


bool case_existe(int i, int j)/*fonction qui verifie si une case existe dans la grille
                                                                        ( j'ai ajouté cette fonction juste pour le deplacement)*/
{
    if (i<0 or i>colonne - 1 or j<0 or j>ligne - 1)
        return false;
    else return true;
}

void init(joueur& j1)//initialisation des deux grilles de chaque joueur
{
    for (int i = 0;i < ligne;++i)
    {
        for (int j = 0;j < colonne;++j)
        {
            j1.grille_bataille[i][j] = 0;
            j1.grille_info[i][j] = 0;
        }
    }
}

void debut_jeu(joueur& j1, joueur& j2)
{
    std::cout << "saisire le nom du premier joueur " << std::endl;
    std::cin >> j1.nomJ;
    std::cout << "saisire le nom du deuxiemme joueur " << std::endl;
    std::cin >> j2.nomJ;
    std::cout << "*****Bataille Navalle *******" << std::endl;
    std::cout << "La partie peut commencer !" << std::endl;
}

void affiche(grille g)/*affichage des deux grilles de chaque joueurs*/
{
    std::cout << std::endl;
    std::cout << "   ";
    for (int i = 0;i < ligne;++i)
        std::cout << (char(97 + i)) << "|";
    std::cout << std::endl;


    for (int i = 0;i < ligne - 1;++i)
    {
        std::cout << i + 1 << " |";
        for (int j = 0; j < colonne;++j)
        {
            if (g[i][j] == 0)
                std::cout << ' ' << "|";
            else if (g[i][j] == -1)
                std::cout << 'r' << "|";
            else if (g[i][j] == -2)
                std::cout << 't' << "|";
            else
                std::cout << '*' << "|";

        }
        std::cout << std::endl;
    }

    std::cout << 10 << '|';
    for (int i = 0;i < colonne;++i)
    {
        if (g[ligne - 1][i] == 0)
            std::cout << ' ' << "|";
        else if (g[ligne - 1][i] == -1)
            std::cout << 'r' << "|";
        else if (g[ligne - 1][i] == -2)
            std::cout << 't' << "|";
        else
            std::cout << '*' << "|";
    }
    std::cout << std::endl;
}

void affiche_bateau(grille g)/*fonction qui affiche l'emplacement de chaque bateau avec son numero dans une grille
                                   pour que chaque joueur sache quel bateau il veux deplacer */
{
    std::cout << std::endl;
    std::cout << "   ";
    for (int i = 0;i < ligne;++i)
        std::cout << (char(97 + i)) << "|";
    std::cout << std::endl;


    for (int i = 0;i < ligne - 1;++i)
    {
        std::cout << i + 1 << " |";
        for (int j = 0; j < colonne;++j)
        {
            if (g[i][j] == 0)
                std::cout << ' ' << "|";
            else if (g[i][j] == -1)
                std::cout << 'r' << "|";
            else if (g[i][j] == -2)
                std::cout << 't' << "|";
            else
                std::cout << g[i][j] << "|";

        }
        std::cout << std::endl;
    }

    std::cout << 10 << '|';
    for (int i = 0;i < colonne;++i)
    {
        if (g[ligne - 1][i] == 0)
            std::cout << ' ' << "|";
        else if (g[ligne - 1][i] == -1)
            std::cout << 'r' << "|";
        else if (g[ligne - 1][i] == -2)
            std::cout << 't' << "|";
        else
            std::cout << g[ligne - 1][i] << "|";
    }
    std::cout << std::endl;
}


bool verify(coordonee c, grille t, int nbr_cases)/*fonction qui verifie si c possible de placer un bateau horizontalement
                                                            ou verticalement dans la grille*/
{
    //c.i=1 et c.j =1 et nbr_cases=4
    if (c.sense == 'h' or c.sense == 'H')
    {
        if (c.j + nbr_cases - 1 >= colonne)
            return false;
        else
        {
            for (int i = 0;i < nbr_cases;i++)
            {
                if (t[c.i][c.j + i] != 0)
                    return false;
            } return true;
        }
    }
    else if (c.sense == 'v' or c.sense == 'V')
    {
        if (c.i + nbr_cases - 1 >= ligne)
            return false;
        else {

            for (int j = 0;j < nbr_cases;j++)
            {
                if (t[c.i + j][c.j] != 0)
                    return false;
            } return true;
        }
    }
    return true;
}

coordonee saisie(grille t, int nbr_cases, std::string j1)/*placement bateaux*/
{
    coordonee c;
    int i, j;
    char x;
    std::cout << "Au tour de " << j1 << " de jouer :" << std::endl;
    do {
        std::cout << "Dans quel sense voulez vous jouer ? (horizontale : h ; verticale : v )" << std::endl;
        std::cin >> c.sense;
    } while (c.sense != 'h' and c.sense != 'H' and c.sense != 'v' and c.sense != 'V');

    do {
        do {
            std::cout << "Dans quelle ligne voulez vous jouer ? ( 1 - 10 )" << std::endl;
            std::cin >> i;
        } while (i - 1 >= colonne or i - 1 < 0);

        do {
            std::cout << "Dans quelle colonne voulez vous jouer ? ( a - j ) " << std::endl;
            std::cin >> x;
            j = x - 'a' + 1;
        } while (j - 1 >= ligne or j - 1 < 0);

        c.i = i - 1;
        c.j = j - 1;
    } while (not(verify(c, t, nbr_cases)));

    return c;

}

bool ligne_pleine(grille t, int i)/* return si une ligne est vide pour placement bateau IA*/
{
    for (int j = 0;j < colonne;j++)
    {
        if (t[i][j] == 0)
            return false;
    }
    return true;
}


bool colonne_pleine(grille t, int j)/*return si une colonne est vide pour placement bateau IA*/
{
    for (int i = 0;i < ligne;i++)
    {
        if (t[i][j] == 0)
            return false;
    }
    return true;
}


coordonee  grille_ia(grille t, int num_bateau) //procedure pour remplire la grille bataille de l'ia
{
    coordonee c;
    int i = rand() % 2;
    if (i == 0)
        c.sense = 'v';
    else
        c.sense = 'h';

    std::cout << "sense " << c.sense << std::endl;

    do {

        do {
            i = rand() % 10;
            c.i = i;
            std::cout << "ligne " << c.i << std::endl;
        } while (ligne_pleine(t, i));


        do {
            i = rand() % 10;
            c.j = i;
        } while (colonne_pleine(t, i));

        std::cout << "colonne " << c.j << std::endl;
    } while (not(verify(c, t, bateau[num_bateau].L)));


    std::cout << "ligne " << c.i << " colone " << c.j << std::endl;

    return c;
}



void mettre_a_jour(grille& t, std::string j)/*fonction qui mis a jour les deux grilles de chaque joueurs et de l'ia 
                                                 après chaque attaque ou deplacement*/
{
    coordonee c;
    for (int k = 0;k < nbateau;k++)
    {
        if (j != "ia" and j != "IA")
        {
            std::cout << "le navire " << bateau[k].name << " de taille " << bateau[k].L << std::endl;
            c = saisie(t, bateau[k].L, j);
        }
        else {
            std::cout << "c'est au tour de  l'ia de jouer" << std::endl;
            c = grille_ia(t, k);
            std::cout << "bateau de taille " << bateau[k].L;

        }

        if (c.sense == 'v' or c.sense == 'V')
        {
            {
                for (int i = 0;i < bateau[k].L;i++)
                    if (case_existe(c.i + i, c.j))
                        t[c.i + i][c.j] = k + 1;
            }
        }
        else if (c.sense == 'H' or c.sense == 'h')
        {
            for (int j = 0;j < bateau[k].L;j++)
            {
                if (case_existe(c.i, c.j + j))
                    t[c.i][c.j + j] = k + 1;
            }
        }
        affiche(t);
    }
}

bool touche_coulee(int b, grille t)
{

    for (int i = 0;i < ligne;i++)
    {
        for (int j = 0;j < colonne;j++)
        {
            if (t[i][j] == b)
                return false;
        }
    }
    return true;
}


coordonee attaque_ia(grille t)/*fonction attaque aleatoire*/
{
    int i;
    coordonee c;

    do {
        i = rand() % 10;
        c.i = i;
        i = rand() % 10;
        c.j = i;
    } while (t[c.i][c.j] == -2);

    return c;

}

void cache()
{
#if defined _WIN32
    system("cls");
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    system("clear");
#elif defined (__APPLE__)
    system("clear");
#endif
}



void attaque(joueur& j1, joueur& j2)
{
    int i, b;
    char j, x;
    coordonee c;
    if (j1.nomJ != "ia" and j1.nomJ != "IA")
    {
        std::cout << j1.nomJ << " ou voulez vous attaquer ?" << std::endl;
        do {
            std::cout << "la ligne :" << std::endl;
            std::cin >> i;
            c.i = i - 1;
            std::cout << "la colonne :" << std::endl;
            std::cin >> j;
            c.j = j - 'a';
            b = j2.grille_bataille[c.i][c.j];

        } while (not(verify(c, j1.grille_info, 1) or j2.grille_bataille[c.i][c.j] == -2));
    }
    else {
        c = attaque_ia(j1.grille_info);
        b = j2.grille_bataille[c.i][c.j];
    }
    if (j2.grille_bataille[c.i][c.j] != 0 and j2.grille_bataille[c.i][c.j] != -1)  /*si bateau touche*/
    {
        j1.grille_info[c.i][c.j] = -2;
        j2.grille_bataille[c.i][c.j] = -2;

        if (touche_coulee(b, j2.grille_bataille))
        {
            if (j1.nomJ != "ia" and j1.nomJ != "IA")
            {
                std::cout << "vous avez touche-coulee le bateau   " << std::endl;
                do {
                    std::cout << "vous avez fini ? (if yes : y )" << std::endl;
                    std::cin >> x;
                } while (x != 'y' and x != 'Y');
            }
            cache();
            std::cout << "Le joueur " << j1.nomJ << " a touche-coulee le navire du joueur " << j2.nomJ << std::endl;
        }
        else
        {
            if (j1.nomJ != "ia" and j1.nomJ != "IA")
            {
                std::cout << "vous avez touche le bateau " << std::endl;
                do {
                    std::cout << "vous avez fini ? (if yes : y )" << std::endl;
                    std::cin >> x;
                } while (x != 'y' and x != 'Y');
            }
            std::cout << "le joueur " << j1.nomJ << " a touche le navire du joueur " << j2.nomJ << std::endl;
            cache();
        }
    }
    else /*si ce n'est pas touche*/
    {
        j1.grille_info[c.i][c.j] = -1;
        j2.grille_bataille[c.i][c.j] = -1;
        if (j1.nomJ != "ia" and j1.nomJ != "IA")
        {
            std::cout << "vous aves rate le bateau " << std::endl;
            do {
                std::cout << "vous avez fini ? (if yes : y )" << std::endl;
                std::cin >> x;
            } while (x != 'y' and x != 'Y');
        }
        cache();
        std::cout << "le joueur " << j1.nomJ << " a rate le navire du joueur " << j2.nomJ << std::endl;

    }
}


/*pour gagner il faut toucher couler tout les bateau donc toucher tte les cases de chque bateau ce qui fait 17 t*/
bool gagne(grille t)
{
    int cpt = 0;
    for (int i = 0;i < ligne;i++)
    {
        for (int j = 0;j < colonne;j++)
        {
            if (t[i][j] == -2)
                cpt++;
        }
    }
    if (cpt == 17)
        return true;
    else
        return false;
}

bool partie_finie(grille g1, grille g2)
{
    return (gagne(g1) or gagne(g2));
}


coordonee premiere_occurence(grille t, int num_bateau)
{
    coordonee c;
    for (int i = 0;i < ligne;i++)
        for (int j = 0;j < colonne;j++)
            if (t[i][j] == num_bateau)
            {
                c.i = i;
                c.j = j;
                return c;
            }
    c.i = -1;   /*dans le cas ou ce bateau n'existe pas dans la grille */
    c.j = -1;
    return c;
}

char sense_bateau(grille t, int num_bateau)
{
    coordonee c;
    c = premiere_occurence(t, num_bateau);
    if (case_existe(c.i, c.j + 1)) {
        if (t[c.i][c.j + 1] == num_bateau)
            return 'h';
        else if (case_existe(c.i, c.j - 1)) {
            if (t[c.i][c.j - 1] == num_bateau) {
                return 'h';
            }
            else return 'v';
        }
    }
    /*si il n'y a pas une case du bateau ni a droite ni a gauche de la premier case du bateau cad le bateau n'est pas horizontale */

    return 'v'; /*jamais ce cas ne sera atteint juste pour enelever le warning */
}


coordonee case_haute(grille t, int num_bateau)/*return les coordonnées de la premiére case en haut verticalement du bateau*/
{
    coordonee c, h;
    c = premiere_occurence(t, num_bateau);
    h.i = c.i;    /*initialisation de h au cas ou nous sommes deja sur la case la plus haute*/
    h.j = c.j;
    for (int i = 1;i < bateau[num_bateau - 1].L;i++)
        if (case_existe(c.i - i, c.j))
            if (t[c.i - i][c.j] == num_bateau)
            {
                h.i = c.i - i;
                h.j = c.j;
            }
    return h;
}

coordonee case_basse(grille t, int num_bateau)/*return les coordonnées de la premiére case en bas verticalement du bateau*/
{
    coordonee c, b;
    c = premiere_occurence(t, num_bateau);
    b.i = c.i;    /*initialisation de b au cas ou nous sommes deja sur la case la plus basse*/
    b.j = c.j;
    for (int i = 1;i < bateau[num_bateau - 1].L;i++)
        if (case_existe(c.i + i, c.j))
            if (t[c.i + i][c.j] == num_bateau)
            {
                b.i = c.i + i;
                b.j = c.j;
            }
    return b;
}

coordonee case_gauche(grille t, int num_bateau)/*return les coordonnées de la premiére case a droite horizontal du bateau*/
{
    coordonee c, g;
    c = premiere_occurence(t, num_bateau);
    g.i = c.i;    /*initialisation de g au cas ou nous sommes deja sur la case la plus a gauche*/
    g.j = c.j;
    for (int i = 1;i < bateau[num_bateau - 1].L;i++)
        if (case_existe(c.i, c.j - i))
            if (t[c.i][c.j - i] == num_bateau)
            {
                g.i = c.i;
                g.j = c.j - i;
            }
    return g;
}

coordonee case_droite(grille t, int num_bateau)/*return les coordonnées de la premiére case a droite horzontale du bateau*/
{
    coordonee c, d;
    c = premiere_occurence(t, num_bateau);
    d.i = c.i;    /*initialisation de d au cas ou nous sommes deja sur la case la plus a droite*/
    d.j = c.j;
    for (int i = 1;i < bateau[num_bateau - 1].L;i++)
        if (case_existe(c.i, c.j + i))
            if (t[c.i][c.j + i] == num_bateau)
            {
                d.i = c.i;
                d.j = c.j + i;
            }
    return d;
}

bool possible(grille t, int num_bateau, char sense_deplacement)/*verifier si c'est possible d'ffectuer un deplacement*/
{
    coordonee c;
    if (sense_deplacement == 'r' or sense_deplacement == 'R') /*deplacement vers la droite*/
    {
        //bool verify(coordonee c, grille t, int nbr_cases)

        c = case_droite(t, num_bateau);
        c.j = c.j + 1;
        if (case_existe(c.i, c.j) and verify(c, t, 1)) {
            if ((t[c.i][c.j] == 0) or (t[c.i][c.j + 1] == -1))  /*si la case ou on veur deplacer existe et si elle est vide ou rate*/
                return true;
            else return false;
        }
    }
    else if (sense_deplacement == 'l' or sense_deplacement == 'L') /*deplacement vers la gauche*/
    {
        c = case_gauche(t, num_bateau);
        c.j = c.j - 1;
        if (case_existe(c.i, c.j) and verify(c, t, 1)) {
            if ((t[c.i][c.j] == 0) or (t[c.i][c.j - 1] == -1))
                return true;
            else return false;
        }
    }
    else if (sense_deplacement == 't' or sense_deplacement == 'T') /*deplacement vers le haut*/
    {
        c = case_haute(t, num_bateau);
        if (case_existe(c.i - 1, c.j)) {
            if ((t[c.i - 1][c.j] == 0) or (t[c.i - 1][c.j] == -1))
                return true;
            else return false;
        }
    }
    else  /*deplacement vers le bas*/
    {
        c = case_basse(t, num_bateau);
        if (case_existe(c.i + 1, c.j)) {
            if ((t[c.i + 1][c.j] == 0) or (t[c.i + 1][c.j] == -1))
                return true;
            else return false;
        }
    }

    return false; /*jamais ce cas ne sera atteint juste pour enelver le warning*/

}

bool bateau_touche(grille t, int num_bateau)/*on a ajouter ces deux fonction pour verifier si tout les bateaux d'un joueurs sont touché 
                                                    si c'est le cas pour afficher un message au joueur comme quoi tous ces bateaux sont touchées
                                                            donc il peut plus deplacer il doit attaquer*/
{
    int cpt = 0;
    for (int i = 0;i < ligne;i++)
        for (int j = 0;j < colonne;j++)
            if (t[i][j] == num_bateau + 1)
                cpt++;

    if (bateau[num_bateau].L > cpt)
        return true;
    else return false;
}

bool tout_bateau_touche(grille t)
{

    for (int i = 0;i < 5;i++)
    {
        if (not(bateau_touche(t, i)))
        {
            return false;
        }
    }

    return true;

}


void deplacement_ou_attaque(joueur& j1, joueur& j2)
{
    char x, s, cbn;
    int n;
    coordonee c;

    if (j1.nomJ != "ia" and j1.nomJ != "IA") {
        do {
            std::cout << j1.nomJ << " voulez vous attaquer ou deplacer un bateau ? (deplacer : d ,attaquer : a )" << std::endl;
            std::cin >> x;

        } while (x != 'a' and x != 'A' and x != 'D' and x != 'd');

        if (tout_bateau_touche(j1.grille_bataille))
        {
            std::cout << "tout vos bateaux sont touchees ,vous ne pouvez pas deplacer ,vous devez attaquer " << std::endl;
            x = 'a';
        }
        if (x == 'd' or x == 'D')
        {
            std::cout << "quelle bateau voulez vous deplacer ?" << std::endl;
            std::cout << "pour rappelle voici les noms des bateau avec leurs numeros :" << std::endl;
            for (int i = 0;i < 5;i++)
                std::cout << bateau[i].name << " : " << i + 1 << std::endl;

            std::cout << "voici votre grille de bataille avec les numeros des bateau " << std::endl;
            affiche_bateau(j1.grille_bataille);

            do {
                std::cout << "saisire le numero du bateau a deplacer : " << std::endl;
                std::cin >> n;
            } while ((n < 1 or n>5) or (bateau_touche(j1.grille_bataille, n - 1)));

            s = sense_bateau(j1.grille_bataille, n);

            if (s == 'h')
            {
                do {
                    do {
                        std::cout << "voulez vous deplacer le bateau vers la droite ou vers la gauche ? (if left : l ; right : r )" << std::endl;
                        std::cin >> x;
                    } while (x != 'r' and x != 'R' and x != 'l' and x != 'L');

                    if (not(possible(j1.grille_bataille, n, x)))
                        std::cout << "ce deplacement n'est pas posible" << std::endl;

                } while (not(possible(j1.grille_bataille, n, x)));

                if (x == 'r' or x == 'R') /*user veut deplacer le bateau vers la droite*/
                {
                    c = case_gauche(j1.grille_bataille, n);
                    j1.grille_bataille[c.i][c.j] = 0;  /*on efface la case la plus a gauche*/
                    c = case_droite(j1.grille_bataille, n);
                    j1.grille_bataille[c.i][c.j + 1] = n; /*on ajoute a la fin du bateau a droite une case*/
                    affiche(j1.grille_bataille);
                    std::cout << "vous avez deplacer un bateau ." << std::endl;


                }

                else /*si user veut deplacer le bateau vers la gauche*/
                {
                    c = case_droite(j1.grille_bataille, n);
                    j1.grille_bataille[c.i][c.j] = 0;  /*on efface la case la plus a droite*/
                    c = case_gauche(j1.grille_bataille, n);
                    j1.grille_bataille[c.i][c.j - 1] = n; /*on ajoute a la fin du bateau a gauche une case*/
                    affiche(j1.grille_bataille);
                    std::cout << "vous avez deplacer un bateau ." << std::endl;
                }


            }
            else /*cad si s=='v'*/
            {
                do {
                    do {
                        std::cout << "voulez vous deplacer le bateau vers le haut ou vers le bas ? (if top : t ; if down : d )" << std::endl;
                        std::cin >> x;
                    } while (x != 't' and x != 'T' and x != 'd' and x != 'D');

                    if (not(possible(j1.grille_bataille, n, x)))
                        std::cout << "ce deplacement n'est pas posible" << std::endl;

                } while (not(possible(j1.grille_bataille, n, x)));

                if (x == 't' or x == 'T') /*si user veut deplacer le bateau vers le haut*/
                {
                    c = case_basse(j1.grille_bataille, n);
                    j1.grille_bataille[c.i][c.j] = 0;
                    c = case_haute(j1.grille_bataille, n);
                    j1.grille_bataille[c.i - 1][c.j] = n;
                    affiche(j1.grille_bataille);
                    std::cout << "vous avez deplacer un bateau ." << std::endl;

                }
                else /*si user veut deplacer le bateau vers le bas*/
                {
                    c = case_haute(j1.grille_bataille, n);
                    j1.grille_bataille[c.i][c.j] = 0;
                    c = case_basse(j1.grille_bataille, n);
                    j1.grille_bataille[c.i + 1][c.j] = n;
                    affiche(j1.grille_bataille);

                }
            }
            do {
                std::cout << "vous avez fini ? (if yes : y )" << std::endl;
                std::cin >> cbn;
            } while (cbn != 'y' and cbn != 'Y');
            cache();
            std::cout << "le joueur " << j1.nomJ << " a deplacer un bateau ." << std::endl;
        }
        else /*cad si x==a (user veut attaquer)*/
            attaque(j1, j2);
    }
    else
    {
        attaque(j1, j2);
        cache();
        std::cout << "l'ia a attaque" << std::endl;
    }
}




int main()
{

    caracteristique();
    joueur j1;
    joueur j2;
    init(j1);
    init(j2);
    srand(time(NULL));

    debut_jeu(j1, j2);

    affiche(j1.grille_bataille);
    mettre_a_jour(j1.grille_bataille, j1.nomJ);
    cache();


    affiche(j2.grille_bataille);
    mettre_a_jour(j2.grille_bataille, j2.nomJ);
    cache();

    do {
        affiche(j1.grille_bataille);
        affiche(j1.grille_info);
        deplacement_ou_attaque(j1, j2);


        if (gagne(j1.grille_info))
            std::cout << j1.nomJ << " a gagne la partie !!!" << std::endl;
        else
        {
            affiche(j2.grille_bataille);
            affiche(j2.grille_info);
            deplacement_ou_attaque(j2, j1);

        }
        if (gagne(j2.grille_info))
            std::cout << j2.nomJ << " a gagne la partie !!! " << std::endl;
    } while (not(partie_finie(j1.grille_info, j2.grille_info)));

    return 0;


}


