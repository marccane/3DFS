#include "matriu.h"
#include <iso646.h>

matriu::matriu(){
    _files=_columnes=0;
    _mat=NULL;
}

//!Pre:files i columnes>0
matriu::matriu(int files, int columnes){
    _files=files; _columnes=columnes;
    _mat=new float*[_columnes];
    for(int i=0;i<_columnes;i++){
        _mat[i]=new float[_files];
    }
}

matriu::~matriu(){
    cout<<"Destructor: "<<this<<" "<<_files<<" x "<<_columnes;
    for(int i=0;i<_columnes;i++){
        delete []_mat[i];
    }
    delete []_mat;
    cout<<"...Ok"<<endl;
}

matriu::matriu(const matriu &b){
    //matriu(b._files,b._columnes); //nope? fa crash
	copia(b);
	mostrar();
    cout<<"Copia: "<<this<<" "<<_files<<" x "<<_columnes<<endl;
}

matriu& matriu::operator=(const matriu &b){
    if(this!=&b){
		copia(b);
    }
    //matriu(b); //wtf? error: declaration of 'matriu b' shadows a parameter|
    //cout<<"Assignacio: "<<this<<" "<<_files<<" x "<<_columnes<<endl;
    return *this;
}

//!Pre:longitud(arr)==_files*_columnes
void matriu::emplenar(float arr[]){ //NO CHECKS! THAT'S NOT SAFE!
    int pos=0;
    for(int i=0;i<_files;i++){
        for(int j=0;j<_columnes;j++){
            _mat[j][i]=arr[pos++];
        }
    }
}

void matriu::mostrar(){
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

matriu matriu::suma(const matriu &b)const{
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

matriu matriu::resta(const matriu &b)const{
    if(_files!=b._files or _columnes!=b._columnes){
        cout<<"No es pot fer la resta"<<endl;
        return matriu();
    }
    matriu res(_files,_columnes);
    for(int i=0;i<_files;i++){
        for(int j=0;j<_columnes;j++){
            res._mat[j][i]=_mat[j][i]-b._mat[j][i];
        }
    }
    return res;
}

matriu matriu::producte_escalar(float f){ //es diu aixi?
    matriu res(_files,_columnes);
    for(int i=0;i<_files;i++){
        for(int j=0;j<_columnes;j++){
            res._mat[j][i]=_mat[j][i]*f;
        }
    }
    return res;
}

matriu matriu::producte_lent(const matriu &b)const{
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

matriu matriu::inversa(){
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
            int pos=0;
            for(int k=0;k<_files;k++){ //Creem les submatrius
                if(k!=i){
                    for(int l=0;l<_columnes;l++){
                        if(l!=j){
                            temp_arr[pos++]=_mat[k][l]; //està invertit pero no és un error! Ens evita fer la transposada
                        }
                    }
                }
            }
        temp.emplenar(temp_arr);
        res._mat[j][i]=((i+j)&1?-1.0f:1.0f)*temp.determinant_r(); //podem optimitzar el ternari a bool b=!b començant amb b fals
        }
    }
    delete []temp_arr;
    //res=res.transposada(); //Ja està transposada!
    res.mostrar();
    res=res.producte_escalar(1.0f/A);
    return res;
}

matriu matriu::transposada(){
    matriu res(_columnes,_files);
    for(int i=0;i<_files;i++){
        for(int j=0;j<_columnes;j++){
            res._mat[j][i]=_mat[i][j];
        }
    }
    return res;
}

float matriu::determinant_r(){ //sense optimitzacio. possibilitats: buscar files amb 0's, no fer crida recursiva si el nombre del centre de la creu és 0, no crear una nova matriu??(hard), no fer el check files=columnes usant immersió...
    if(_files!=_columnes){ //si no es quadrada no podem fer el det
        cout<<"No es pot fer el determinant"<<endl;
        return 0;
    }

    const int col_sel=0; //!hardcodejat per ara, al deshardcodejar pensar en que el SIGNE del primer de la columna no és sempre +!
    float res;
    if(_files<=3){
        if(_files==3){
            float a,b,**&m=_mat;
            a=m[0][0]*m[1][1]*m[2][2]+m[0][1]*m[1][2]*m[2][0]+m[1][0]*m[2][1]*m[0][2]; //ho podem reordenar per ajudar la cache?
            b=m[2][0]*m[1][1]*m[0][2]+m[1][2]*m[2][1]*m[0][0]+m[0][1]*m[1][0]*m[2][2];
            res=a-b;
        }
        else if(_files==2)
            res=_mat[0][0]*_mat[1][1]-_mat[0][1]*_mat[1][0];
        else if(_files==1)
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

void matriu::copia(const matriu& b) {
	_files = b._files; _columnes = b._columnes;
	_mat = new float*[_columnes];
	for (int i = 0; i<_columnes; i++) {
		_mat[i] = new float[_files];
	} //hem de fer delete de lu vell? memory leak?

	for (int i = 0; i<_files; i++) {
		for (int j = 0; j<_columnes; j++) {
			_mat[j][i] = b._mat[j][i];
		}
	}
}

//!Pre: Matriu quadrada de n>=3
/*float determinant_r3plus(){
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

/*float determinant_3x3(){
    if(_files!=3 or _columnes!=3){
        cout<<"No es pot fer el determinant"<<endl;
        return 0;
    }
    float a,b,**&m=_mat;
    a=m[0][0]*m[1][1]*m[2][2]+m[0][1]*m[1][2]*m[2][0]+m[1][0]*m[2][1]*m[0][2];
    b=m[2][0]*m[1][1]*m[0][2]+m[1][2]*m[2][1]*m[0][0]+m[0][1]*m[1][0]*m[2][2];
    return a-b;
}*/

/*matriu& moure(matriu &b){
    if(this!=&b){
        _files=b._files;
        _columnes=b._columnes;
        _mat=b._mat;
        b._files=b._columnes=0;
        b._mat=NULL;
    }
    return *this;
}*/ //useless perquè per moure una matriu l'objecte ha de ser un lvalue i per obtenir l'lvalue hem de fer una copia, que fa que aixo perdi tot el sentit
//res.moure(res.producte_escalar(1/A)); //f*** els return values son rvalues

/*matriu& matriu::operator=(const matriu &b){
    if(this!=&b){
        *this=matriu(b); //sembla que no xuta
    }
    //matriu(b); //wtf? error: declaration of 'matriu b' shadows a parameter|
    //cout<<"Assignacio: "<<this<<" "<<_files<<" x "<<_columnes<<endl;
    return *this;
}*/  //eto no va :(

/*matriu::matriu()
{
    //ctor
}

matriu::~matriu()
{
    //dtor
}

matriu::matriu(const matriu& other)
{
    //copy ctor
}

matriu& matriu::operator=(const matriu& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}*/
