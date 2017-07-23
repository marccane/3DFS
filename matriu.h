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
    //!Pre:files i columnes>0
    matriu(int files, int columnes);
    matriu(const matriu &b);
    ~matriu();
    //matriu& operator=(const matriu &b);

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
};

#endif // MATRIU_H
