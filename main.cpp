#include "matriu.h"
#include <windows.h>
#include <cmath>

using namespace std;
const float PI=3.141592;

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
        //cout<<endl;
    }
}

void producte_vectorial(float a[], float b[], float c[]){
    c[0]=a[1]*b[0]-a[2]*b[1];
    c[1]=a[2]*b[0]-a[0]*b[2];
    c[2]=a[0]*b[1]-a[1]*b[0];
}

/*void normalitzar_vector(float a[], float b[]){
    float modul, temp=a[0]*a[0]+a[1]*a[1]+a[2]*a[2];
    modul=sqrt(temp);
    for(int i=0;i<3;i++)b[i]=a[i]/modul;
}*/

int main()
{
    //char framebuffer[(int)TAMANY_HORIT][(int)TAMANY_VERT];
    //char framebuffer[80][40]; //temp
    char framebuffer[(int)TAMANY_HORIT+10][(int)TAMANY_VERT];
    float vista[]={45.0,45.0},
    an1[]={ cos(vista[0]*PI/180.0)*sin(vista[1]*PI/180.0),
            sin(vista[0]*PI/180.0)*sin(vista[1]*PI/180.0),
            cos(vista[1]*PI/180.0)
            },
    av1[]={-an1[1],an1[0],0}, aw1[3]/*, n[3], v[3], w[3]*/;
    float *n=an1,*v=av1,*w=aw1;

    producte_vectorial(an1,av1,aw1);
    /*normalitzar_vector(an1,n);
    normalitzar_vector(av1,v);
    normalitzar_vector(aw1,w);*/

    float
    aM[]={  v[0],w[0],n[0],0,
            v[1],w[1],n[1],0,
            v[2],w[2],n[2],0,
            0,0,0,1
            },
    aPxy[]={1,0,0,0,
            0,1,0,0,
            0,0,0,0,
            0,0,0,1
            },
    acb[]={ 1,1,1,1,2,2,2,2,
            1,2,2,1,1,2,2,1,
            1,1,2,2,1,1,2,2,
            1,1,1,1,1,1,1,1
            };

    matriu Pn, M(4,4,aM), Pxy(4,4,aPxy), cb(4,8,acb), cbp;
    Pn=M.producte_lent(Pxy).producte_lent(M.inversa());
    //Pn.mostrar();
    cbp=Pn.producte_lent(cb);
    //cbp.mostrar();
    matriu cpla=M.inversa().producte_lent(cbp);

    //clearscreen();
    netejar_fb(framebuffer);
    cpla.preparar_matriu(cpla.minmaxxy());
    cpla.escriure_fb(framebuffer);
    //framebuffer[0][0]='A';
    //framebuffer[TAMANY_HORIT-1][TAMANY_VERT-1]='A';
    dibuixar_fb(framebuffer);

    //char a;cin>>a;

    return 0;
}

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
    a.producte_escalar(5).mostrar();*/
