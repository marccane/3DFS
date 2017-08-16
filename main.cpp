#include "matriu.h"
#include <windows.h>
#include <cmath>
using namespace std;

const float PI=3.14159;

void clearscreen()//codirobat per evitar flickers
{
    COORD Position;
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	Position.X = Position.Y = 0;
	SetConsoleCursorPosition(hOut, Position);
}

void netejar_fb(char fb[][(int)TAMANY_VERT]){
    for(int i=0;i<TAMANY_HORIT;i++)
        for(int j=0;j<TAMANY_VERT;j++)
            fb[i][j]=' ';
}

void dibuixar_fb(char fb[][(int)TAMANY_VERT]){
    for(int i=0;i<TAMANY_VERT;i++){
        for(int j=0;j<TAMANY_HORIT;j++)
            cout<<fb[j][i];
    }
}

//parametres: entrada,entrada,sortida
void producte_vectorial(const float a[], const float b[], float c[]){
    c[0]=a[1]*b[0]-a[2]*b[1];
    c[1]=a[2]*b[0]-a[0]*b[2];
    c[2]=a[0]*b[1]-a[1]*b[0];
}

void llegir_tractar_entrada(float vista[], bool &sortir){
    if (GetAsyncKeyState(VK_UP)){
        vista[0]+=5;
    }
    if (GetAsyncKeyState(VK_DOWN)){
        vista[0]-=5;
    }
    if (GetAsyncKeyState(VK_LEFT)){
        vista[1]-=5;
    }
    if (GetAsyncKeyState(VK_RIGHT)){
        vista[1]+=5;
    }
    if (GetAsyncKeyState('S')){
        sortir=true;
    }
}

int main()
{
    bool sortir=false;
    char framebuffer[(int)TAMANY_HORIT][(int)TAMANY_VERT];
    float vista[]={45.0,45.0},
    aPxy[]={1,0,0,0,
            0,1,0,0,
            0,0,0,0,
            0,0,0,1
            },
    acube[]={   1,1,1,1,2,2,2,2,
                1,2,2,1,1,2,2,1,
                1,1,2,2,1,1,2,2,
                1,1,1,1,1,1,1,1},
    w[3];

    /*const*/ matriu Pxy(4,4,aPxy), cube(4,8,acube), cbp;
    matriu Pn, M(4,4);

    while(!sortir){
        llegir_tractar_entrada(vista,sortir);

        float
        n[]={   cos(vista[0]*PI/180.0)*sin(vista[1]*PI/180.0),
                sin(vista[0]*PI/180.0)*sin(vista[1]*PI/180.0),
                cos(vista[1]*PI/180.0)
                },
        v[]={-n[1],n[0],0}/*,
        aRot[]={1,0,0,0
                0,cos(angle),-sin(angle),0
                0,sin(angle),cos(angle),0
                0,0,0,1}*/;

        producte_vectorial(n,v,w);

        float
        aM[]={  v[0],w[0],n[0],0,
                v[1],w[1],n[1],0,
                v[2],w[2],n[2],0,
                0,0,0,1};

        M.emplenar(aM);
        Pn=M.producte_lent(Pxy).producte_lent(M.inversa());
        cbp=Pn.producte_lent(cube);
        matriu cpla=M.inversa().producte_lent(cbp);

        clearscreen();
        netejar_fb(framebuffer);
        cpla.preparar_matriu(cpla.minmaxxy());
        //cpla.preparar_matriu2();
        cpla.escriure_fb(framebuffer);
        dibuixar_fb(framebuffer);
    }

    return 0;
}

/*void normalitzar_vector(float a[], float b[]){
    float modul, temp=a[0]*a[0]+a[1]*a[1]+a[2]*a[2];
    modul=sqrt(temp);
    for(int i=0;i<3;i++)b[i]=a[i]/modul;
}*/

/*matriu M_SR(3,3);
    float aSR[]={   1,0,0
                    0,1,0
                    0,0,1};
    M_SR.emplenar(aSR);*/

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
    /*matriu a(3,3), b(3,3), c(3,3), d(1,1), e;
    float aa[]={1,2,3,4,5,6,7,8,9};
    float ab[]={6,7,8,9,10,11,12,13,14};
    float ac[]={8,7,6,2,4,5,3,6,8};
    a.emplenar(aa);
    b.emplenar(ab);
    c.emplenar(ac);
    a.mostrar();
    a=c.inversa();
    a.mostrar();
    d.mostrar();
    e=b;
    e.mostrar();
    */
    /*a.mostrar();
    b.mostrar();
    a.suma(b).mostrar();
    a.resta(c).mostrar();
    a.producte_lent(b).mostrar();
    a.producte_k(5).mostrar();*/
