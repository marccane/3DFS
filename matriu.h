#ifndef MATRIU_H
#define MATRIU_H
#include <iostream>
using namespace std;

class matriu{
private:
    int _files, _columnes;
    float **_mat;

public:
    matriu();
    matriu(int files, int columnes);
    ~matriu();
    matriu(const matriu &b);
    matriu& operator=(const matriu &b);
    //!Pre:longitud(arr)==_files*_columnes
    void emplenar(float arr[]);
    void mostrar();
    matriu suma(const matriu &b)const;
    matriu resta(const matriu &b)const;
    matriu producte_escalar(float f);
    matriu producte_lent(const matriu &b)const;
    matriu inversa();
    matriu transposada();
    float determinant_r();
    void set_matriu_translacio(int mida, int dist);
    //homotècia
    //projeccio
    //simetria
    void set_matriu_rotacio(int mida, float angle);
};

#endif // MATRIU_H
