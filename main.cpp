#include <iostream>

using namespace std;

class matriu{
private:
    int _files, _columnes;
    float **_mat;

public: //Moltes operacions maten la caché inecessariament, arreglar. Cntl+f amb "i<_files" trobarà la majoria, pero no totes
    matriu(){
        _files=_columnes=0;
        _mat=NULL;
    }

    matriu(int files, int columnes){
        _columnes=columnes; _files=files;
        _mat=new float*[_columnes];
        for(int i=0;i<_columnes;i++){
            _mat[i]=new float[_files];
        }
    }

    ~matriu(){
        cout<<"Destructor: "<<this<<" "<<_files<<" x "<<_columnes;
        for(int i=0;i<_columnes;i++){
            delete []_mat[i];
        }
        delete []_mat;
        cout<<"...Ok"<<endl;
    }

    matriu& operator=(const matriu &b){
        if(this!=&b){
            matriu(_files,_columnes);
            for(int i=0;i<_files;i++){
                for(int j=0;j<_columnes;j++){
                    _mat[j][i]=b._mat[j][i];
                }
            }
        }
        cout<<"Copia: "<<this<<" "<<_files<<" x "<<_columnes<<endl;
        return *this;
    }

    matriu& copia_i_esborrat(matriu &b){
        if(this!=&b){
            _files=b._files;
            _columnes=b._columnes;
            _mat=b._mat;
            b._files=b._columnes=0;
            b._mat=NULL;
        }
        return *this;
    }

    //!Pre:longitud(arr)==_files*_columnes
    void emplenar(float arr[]){ //NO CHECKS! THAT'S NOT SAFE!
        int pos=0;
        for(int i=0;i<_files;i++){
            for(int j=0;j<_columnes;j++){
                _mat[j][i]=arr[pos++];
            }
        }
    }

    void mostrar(){
        cout<<"Mostrar: "<<this<<endl;
        for(int i=0;i<_files;i++){
            for(int j=0;j<_columnes;j++){
                cout<<_mat[j][i]<<" ";
            }
            cout<<endl;
        }
        cout<<endl;
    }

    //si a cada operació hem d'allocatejar memoria no nem bé... hauriem de reaprofitar els objectes allocatejats... podriem fer una pila...
    //perquè sobreescriure la matriu actual no és una opcio oi?
    matriu suma(const matriu &b)const{
        if(_files!=b._files or _columnes!=b._columnes){
            cout<<"No es pot fer la suma"<<endl;
            return matriu();
        }
        matriu res(_files,_columnes);
        for(int i=0;i<_files;i++){
            for(int j=0;j<_columnes;j++){
                res._mat[j][i]=_mat[j][i]+b._mat[j][i]; //RIP caché (en matrius grans pot fer molt de mal)
            }
        }
        return res;
    }

    matriu resta(const matriu &b)const{
        if(_files!=b._files or _columnes!=b._columnes){
            cout<<"No es pot fer la resta"<<endl;
            return matriu();
        }
        matriu res(_files,_columnes);
        for(int i=0;i<_files;i++){
            for(int j=0;j<_columnes;j++){
                res._mat[j][i]=_mat[j][i]+b._mat[j][i];
            }
        }
        return res;
    }

    matriu producte_escalar(float f){ //es diu aixi?
        matriu res(_files,_columnes);
        for(int i=0;i<_files;i++){
            for(int j=0;j<_columnes;j++){
                res._mat[j][i]=_mat[j][i]*f;
            }
        }
        return res;
    }

    matriu producte_lent(const matriu &b)const{
        if(_columnes!=b._files){
            cout<<"No es pot fer la multiplicacio"<<endl;
            return matriu();
        }
        matriu res(_files,b._columnes);
        for(int i=0;i<_files;i++){
            for(int j=0;j<b._columnes;j++){
                float temp=0;
                for(int k=0;k<_columnes;k++){
                    temp+=_mat[k][i]*b._mat[j][k];
                }
                res._mat[j][i]=temp;
            }
        }
        return res;
    }

    matriu inversa(){
        float A=determinant_r();
        if(_files!=_columnes or A==0){ //si no es quadrada o det=0 no podem fer la inv
            cout<<"No es pot fer la inversa"<<endl;
            return matriu();
        }
        matriu res(_files,_files), temp(_files-1,_files-1);
        int tamany=(_files-1)*(_files-1);
        float *temp_arr=new float[tamany]; //antic candidat a memory leak. Punters explícits = mal
        for(int i=0;i<_files;i++){
            for(int j=0;j<_columnes;j++){
                //creem la minimatriu
                int pos=0;
                for(int k=0;k<_files;k++){
                    if(k!=i){
                        for(int l=0;l<_columnes;l++){
                            if(l!=j){
                                temp_arr[pos++]=_mat[l][k];
                            }
                        }
                    }
                }
            temp.emplenar(temp_arr);
            res._mat[j][i]=((i+j)&1?-1:1)*temp.determinant_r(); //podem optimitzar el ternari a bool b=!b començant amb b fals
            }
        }
        delete []temp_arr;
        res.mostrar();
        //res=res.producte_escalar(1/A);
        //res.mostrar();
        return res;
    }

    matriu transposada(){
        matriu res(_files,_columnes);
        for(int i=0;i<_files;i++){
            for(int j=0;j<_columnes;j++){
                res._mat[j][i]=_mat[i][j];
            }
        }
        return res;
    }

    float determinant_3x3(){
        if(_files!=3 or _columnes!=3){
            cout<<"No es pot fer el determinant"<<endl;
            return 0;
        }
        float a,b,**&m=_mat;
        a=m[0][0]*m[1][1]*m[2][2]+m[0][1]*m[1][2]*m[2][0]+m[1][0]*m[2][1]*m[0][2];
        b=m[2][0]*m[1][1]*m[0][2]+m[1][2]*m[2][1]*m[0][0]+m[0][1]*m[1][0]*m[2][2];
        return a-b;
    }

    float determinant_r(){ //mateixes optimitzacions que al determinant_r3plus + masses crides recursives
        if(_files!=_columnes){ //si no es quadrada no podem fer el det
            cout<<"No es pot fer el determinant"<<endl;
            return 0;
        }

        const int col_sel=0; //!hardcodejat per ara, al deshardcodejar pensar en que el SIGNE del primer de la columna no és sempre +!
        float res;
        /*if(_files==1)
            res=_mat[0][0];*/
        if(_files==2)
            res=_mat[0][0]*_mat[1][1]-_mat[0][1]*_mat[1][0];
        else{
            res=0;
            int tamany=_files*_columnes;
            float *temp_arr=new float[tamany]; //antic candidat a memory leak. Punters explícits = mal
            for(int i=0;i<_files;i++){
                int pos=0;
                for(int j=0;j<_files;j++){
                    if(j!=i){
                        for(int k=0;k<_columnes;k++){
                            if(k!=col_sel){
                                temp_arr[pos++]=_mat[k][j];
                            }
                        }
                    }
                }
                matriu temp_mat(_files-1,_files-1);
                temp_mat.emplenar(temp_arr);
                res+=(i&1?-1:1)*_mat[col_sel][i]*temp_mat.determinant_r();
            }
            delete []temp_arr;
        }
        return res;
    }

    void set_matriu_translacio(int mida, int dist){}
    //homotècia
    //projeccio
    //simetria
    void set_matriu_rotacio(int mida, float angle){}
};

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
    a.mostrar();
    b.mostrar();
    c.mostrar();
    a.suma(b).mostrar();
    //c.inversa();
    //matriu r=a.suma(b);
    //r.mostrar();
    //cout<<"Aqui ve la inversa"<<endl;
    //c.inversa();
    //a.resta(b).mostrar();
    //a.producte_lent(b).mostrar();
    //a.resta(b).mostrar();
    //matriu res=a.producte_lent(b);
    //res.mostrar();
    return 0;
}

//!Pre: Matriu quadrada de n>=3
    /*float determinant_r3plus(){ //sense cap tipus d'optimitzacio. possibilitats: buscar files amb 0's, no fer crida recursiva si el nombre del centre de la creu és 0, no crear una nova matriu??(hard), no fer el check files=columnes usant immersió...
        if(_files!=_columnes){ //si no es quadrada no podem fer el det
            cout<<"No es pot fer el determinant"<<endl;
            return 0;
        }

        const int col_sel=0; //!hardcodejat per ara, al deshardcodejar pensar en que el SIGNE del primer de la columna no és sempre +!
        float res;
        if(_files==3){
            res=determinant_3x3();
        }
        else{
            res=0;
            int tamany=_files*_columnes;
            float *temp_arr=new float[tamany]; //antic candidat a memory leak. Punters explícits = mal
            for(int i=0;i<_files;i++){
                int pos=0;
                for(int j=0;j<_files;j++){
                    if(j!=i){
                        for(int k=0;k<_columnes;k++){
                            if(k!=col_sel){
                                temp_arr[pos++]=_mat[k][j];
                            }
                        }
                    }
                }
                matriu temp_mat(_files-1,_files-1);
                temp_mat.emplenar(temp_arr);
                res+=(i&1?-1:1)*_mat[col_sel][i]*temp_mat.determinant_r3plus();
            }
            delete []temp_arr;
        }
        return res;
    }*/

    /*float determinant(){
        if(_files!=_columnes){ //si no es quadrada no podem fer el det
            cout<<"No es pot fer el determinant"<<endl;
            return 0;
        }
        if(_files==2){
            return _mat[0][0]*_mat[1][1]-_mat[0][1]*_mat[1][0];
        }
        else if(_files==3){

        }
    }*/
