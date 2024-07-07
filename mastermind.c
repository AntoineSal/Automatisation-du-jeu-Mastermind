#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MIN(a,b) (((a)<(b))?(a):(b))

struct jeu_s{
    int n ;
    int k ;
    int* code ;
} ;
typedef struct jeu_s jeu ;

struct car_s{
    int r1 ;
    int r2 ;
} ;
typedef struct car_s car ;

car caracterisitques(jeu g, int* proposition){
    car c;
    c.r1 = 0;
    c.r2 = 0 ;
    int* couleurs_code = malloc(sizeof(int)*g.n) ;
    int* couleurs_prop = malloc(sizeof(int)*g.n) ;
    for(int i = 0; i < g.k; i++){
        couleurs_code[g.code[i]] ++ ;
        couleurs_prop[proposition[i]] ++ ;
    }
    for(int i = 0; i < g.n; i++){
        c.r2 += MIN(couleurs_prop[i], couleurs_code[i]) ;
    }
    for(int i = 0; i < g.k; i++){
        if (g.code[i] == proposition[i]){
            c.r1 ++ ;
            c.r2 -- ;
        }
    }
    return c ;
} ;

struct possibles_s{
    int nb_prop ;
    int** props ;
} ;
typedef struct possibles_s possibles ;

possibles next_possibles(jeu g, possibles p, int* prev_prop){
    car c = caracterisitques(g, prev_prop) ;
    jeu g2;
    g2.n = g.n ;
    g2.k = g.k ;
    g2.code = prev_prop ;
    possibles p2;
    p2.nb_prop = 0 ;
    int* new_possibles = malloc(sizeof(int)*p.nb_prop) ;
    for(int i = 0; i < p.nb_prop; i++){
        car c2 = caracterisitques(g2, p.props[i]) ;
        if (c2.r1 != c.r1 || c2.r2 != c.r2){
            new_possibles[i] = 0 ;
        } else{
            p2.nb_prop ++ ;
            new_possibles[i] = 1 ;
        }
    }
    p2.props = malloc(sizeof(int*)*p2.nb_prop) ;
    int ind = 0 ;
    for(int i = 0; i < p.nb_prop; i++){
        if (new_possibles[i] == 1){
            p2.props[ind] = p.props[i] ;
            ind ++ ;
        }
        else{
            free(p.props[i]) ;
        }
    }
    free(p.props) ;
    return p2 ;
}

int strategie1(jeu g){
    possibles p ;
    p.nb_prop = pow(g.n, g.k) ;
    p.props = malloc(sizeof(int*)*(p.nb_prop)) ;
    p.props[0] = malloc(sizeof(int)*g.k) ;
    for(int i = 0; i < g.k; i++){
        p.props[0][i] = 0 ;
    }
    for(int i = 1; i < p.nb_prop; i++){
        p.props[i] = malloc(sizeof(int)*g.k) ;
        int j = 0;
        while(p.props[i-1][j] == (g.n - 1) && j < g.k){
            p.props[i][j] = 0 ;
            j++ ;
        }
        if (j < g.k && p.props[i-1][j] + 1 < g.n){
            p.props[i][j] = p.props[i-1][j] + 1 ;
            j ++ ;
        }
        for(int j2 = j; j2 < g.k; j2++){
            p.props[i][j2] = 0 ;
        }
    }
    int essais = 0 ;
    car c = caracterisitques(g, p.props[0]) ;
    printf("%d %d", c.r1, c.r2) ;
    while(c.r1 != g.k){
        p = next_possibles(g, p, p.props[0]) ;
        c =  caracterisitques(g, p.props[0]) ;
        for(int i = 0; i < g.k; i++){
            printf("%d; ", p.props[0][i]) ;
        }
        essais ++ ;
        printf(" -> r1 = %d et r2 = %d \n", c.r1, c.r2) ;
    }
    printf("%d", essais) ;
    return essais ;
}

int main(){
    jeu g ;
    g.k = 4 ;
    g.n = 5 ; 
    g.code = malloc(sizeof(int)*g.k) ;
    g.code[0] = 2; 
    g.code[1] = 4; 
    g.code[2] = 3; 
    g.code[3] = 4 ;
    strategie1(g) ;
    return 0;
}
