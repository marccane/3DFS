#ifndef MATRIU_H
#define MATRIU_H
#include <iostream>
using namespace std;

const float TAMANY_HORIT=80, TAMANY_VERT=20;

struct min_max_xy{
    float minx, miny, maxx, maxy;
};

//!Invariant: _files i _columnes>0
class matriu{
private:
    int _files, _columnes;
    float **_mat;

public:
    matriu();
    //!Pre:files i columnes>0
    matriu(int files, int columnes, float *arr=NULL);
    matriu(const matriu &b);
    matriu& operator=(const matriu &b);
    matriu& operator*();
    void copia(const matriu &b);
    ~matriu();

    void mostrar();
    //!Pre:longitud(arr)==_files*_columnes
    void emplenar(float arr[]);
    matriu suma(const matriu &b)const;
    matriu resta(const matriu &b)const;
    matriu producte_escalar(float f);
    matriu producte_lent(const matriu &b)const;
    matriu inversa();
    matriu transposada();
    float determinant_r();
    min_max_xy minmaxxy();
    void preparar_matriu(min_max_xy m);
    void escriure_fb(char fb[][(int)TAMANY_VERT]);
    //void set_matriu_translacio(int mida, int dist);
    //homotècia
    //projeccio
    //simetria
    //void set_matriu_rotacio(int mida, float angle);
};

#endif // MATRIU_H
