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
        for(int i=0;i<_columnes;i++){
            delete []_mat[i];
        }
        delete []_mat;
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
        for(int i=0;i<_files;i++){
            for(int j=0;j<_columnes;j++){
                cout<<_mat[j][i];
            }
            cout<<endl;
        }
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
        if(_files!=_columnes){ //si no es quadrada no podem fer la inv
            cout<<"No es pot fer la inversa"<<endl;
            return matriu();
        }
        float A=determinant_r();
        matriu res(_files,_files);
        for(int i=0;i<_files;i++){
            for(int j=0;j<_columnes;j++){
/*
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
                }
*/
            }
        }
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

    float determinant_r(){ //mateixes optimitzacions que al determinant_r3plus + masses crides recursives
        if(_files!=_columnes){ //si no es quadrada no podem fer el det
            cout<<"No es pot fer el determinant"<<endl;
            return 0;
        }

        const int col_sel=0; //!hardcodejat per ara, al deshardcodejar pensar en que el SIGNE del primer de la columna no és sempre +!
        float res;
        if(_files==1){
            res=_mat[0][0];
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
                res+=(i&1?-1:1)*_mat[col_sel][i]*temp_mat.determinant_r();
            }
            delete []temp_arr;
        }
        return res;
    }

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

    void set_matriu_translacio(int mida, int dist){}
    //homotècia
    //projeccio
    //simetria
    void set_matriu_rotacio(int mida, float angle){}

};
