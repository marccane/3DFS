#include "matriu.h"

using namespace std;

int main()
{
    /*float arr[]={5,6,7,8,10,12,20,30,27};
    matriu test(3,3);
    test.emplenar(arr);
    cout << test.determinant_3x3() << endl;*/

    /*float arr[]={2,8,5,6,4,9,12,5,6,3,7,2,9,8,4,6};
    matriu test(4,4);
    test.emplenar(arr);
    cout << test.determinant_r() << endl;
    cout << test.determinant_r3plus() << endl;*/

    //test per multiplicacio de matrius quadrades i no quadrades
    matriu a(3,3), b(3,3), c(3,3);
    float aa[]={1,2,3,4,5,6,7,8,9};
    float ab[]={6,7,8,9,10,11,12,13,14};
    float ac[]={8,7,6,2,4,5,3,6,8};
    a.emplenar(aa);
    b.emplenar(ab);
    c.emplenar(ac);
    /*a.mostrar();
    b.mostrar();
    a.suma(b).mostrar();
    a.resta(c).mostrar();
    a.producte_lent(b).mostrar();
    a.producte_escalar(5).mostrar();*/
    c.mostrar();
    c.inversa().mostrar();
    return 0;
}
