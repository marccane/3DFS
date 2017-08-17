#include "matriu.h"
#include <iso646.h>

matriu::matriu(){
    _files=_columnes=0;
    _mat=NULL;
}

//!Pre:files i columnes>0
matriu::matriu(int files, int columnes, float *arr){
    _files=files; _columnes=columnes;
    _mat=new float*[_columnes];

    //if(_files>10 or _columnes>10)
        //cout<<"WTF dude"<<endl;

    for(int i=0;i<_columnes;i++){
        _mat[i]=new float[_files];
    }
    if(arr!=NULL){
        emplenar(arr);
    }
}

matriu::~matriu(){
    //cout<<"Destructor: "<<this<<" "<<_files<<" x "<<_columnes;
    for(int i=0;i<_columnes;i++){
        delete []_mat[i];
    }
    delete []_mat;
    //cout<<"...Ok"<<endl;
}

matriu::matriu(const matriu &b){
    //matriu(b._files,b._columnes); //nope? fa crash
	//!Quan arribem aqui la matriu actual (this) cont� garbage SEMPRE (oi? perqu� apart dels returns es crida en alguna altre ocasi� el constructor de copia?)
	_mat=NULL;
	copia(b);
    //cout<<"Copia: "<<this<<" "<<_files<<" x "<<_columnes<<endl;
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

void matriu::copia(const matriu& b){
    //crec que hi continua haguent un memory leak important aqui:
    //si els punters de la matriu "a" es sobreescriuen el destructor no els podra netejar //Fixed?
    if(_mat==NULL or _files!=b._files or _columnes!=b._columnes){ //si la matriu no t� prou memoria hem de fer delete i tornar a demanar-ne
        if(_mat!=NULL){ //si la matriu cont� dades, les borrem
            for(int i=0;i<_columnes;i++) //com que no podem crida el destructor explicitament hem de copiar el codi... (enserio c++?)
                delete []_mat[i];
            delete []_mat;
        }

        _files=b._files; _columnes=b._columnes;
        _mat=new float*[b._columnes];
        for(int i=0;i<b._columnes;i++)
            _mat[i]=new float[b._files];
    }
    //Intent d'aprofitar les matrius m�s grosses: RIP, aixo crea un memory leak dels guapos, hem d'afegir atributs (_files_reals i _cols_reals) o nom�s acceptar les matrius que siguin ja del mateix tamany
    //si no, no alliberem tota la memoria ja que limitem la mida de l'array inicial que pot contenir m�s punters a les files de la matriu m�s grossa
    //Si volguessim apurar molt podriem acceptar matrius que tinguessin m�s files que la nostra i aquestes s'alliberarien igualment.
    //El que no podem fer MAI �s dir que tenim menys columnes de les que realment tenim.

	for(int i=0; i<_files;i++){
		for(int j=0;j<_columnes;j++){
			_mat[j][i]=b._mat[j][i];
		}
	}
}

void matriu::mostrar(){
    //cout<<"Mostrar: "<<this<<endl;
    for(int i=0;i<_files;i++){
        for(int j=0;j<_columnes;j++){
            cout<<_mat[j][i]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}

//si a cada operaci� hem d'allocatejar memoria no nem b�... hauriem de reaprofitar els objectes allocatejats... podriem fer una pila...
//perqu� sobreescriure la matriu actual no �s una opcio oi?

matriu matriu::suma(const matriu &b)const{
    if(_files!=b._files or _columnes!=b._columnes){
        cout<<"No es pot fer la suma"<<endl;
        return matriu();
    }
    matriu res(_files,_columnes);
    for(int i=0;i<_files;i++){
        for(int j=0;j<_columnes;j++){
            res._mat[j][i]=_mat[j][i]+b._mat[j][i]; //RIP cach� (en matrius grans pot fer molt de mal)
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

matriu matriu::producte(float k){ //es diu aixi?
    matriu res(_files,_columnes);
    for(int i=0;i<_files;i++){
        for(int j=0;j<_columnes;j++){
            res._mat[j][i]=_mat[j][i]*k;
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
    float *temp_arr=new float[tamany]; //antic candidat a memory leak. Punters expl�cits = mal
    for(int i=0;i<_files;i++){
        for(int j=0;j<_columnes;j++){
            int pos=0;
            for(int k=0;k<_files;k++){ //Creem les submatrius
                if(k!=i){
                    for(int l=0;l<_columnes;l++){
                        if(l!=j){
                            temp_arr[pos++]=_mat[k][l]; //est� invertit pero no �s un error! Ens evita fer la transposada
                        }
                    }
                }
            }
        temp.emplenar(temp_arr);
        res._mat[j][i]=((i+j)&1?-1.0f:1.0f)*temp.determinant_r(); //podem optimitzar el ternari a bool b=!b comen�ant amb b fals
        }
    }
    delete []temp_arr;
    //res=res.transposada(); //Ja est� transposada!
    res=res.producte(1.0f/A);
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

float matriu::determinant_r(){ //sense optimitzacio. possibilitats: buscar files amb 0's, no fer crida recursiva si el nombre del centre de la creu �s 0, no crear una nova matriu??(hard), no fer el check files=columnes usant immersi�...
    if(_files!=_columnes){ //si no es quadrada no podem fer el det
        cout<<"No es pot fer el determinant"<<endl;
        return 0;
    }

    const int col_sel=0; //!hardcodejat per ara, al deshardcodejar pensar en que el SIGNE del primer de la columna no �s sempre +!
    float res;
    if(_files<=3){
        if(_files==3){
            float a,b, **&m=_mat /* **m=&_mat */; //perqu� no funciona/no �s el mateix?
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
        float *temp_arr=new float[tamany]; //antic candidat a memory leak. Punters expl�cits = mal
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

min_max_xy matriu::minmaxxy(){ //aix� no �s una operaci� propia d'una matriu...
    if(_files<2 or _columnes<=0){
        cout<<"Error al calcular minmaxxy"<<endl;
        return minmaxxy();
    }
    min_max_xy res;
    res.minx=res.maxx=_mat[0][0];
    res.miny=res.maxy=_mat[0][1];
    for(int i=1;i<_columnes;i++){
         if(_mat[i][1]>res.maxx)res.maxx=_mat[i][1];
         if(_mat[i][1]<res.minx)res.minx=_mat[i][1];
         if(_mat[i][0]>res.maxy)res.maxy=_mat[i][0];
         if(_mat[i][0]<res.miny)res.miny=_mat[i][0];
    }
    return res;
}

void matriu::preparar_matriu(min_max_xy m){//no pertany a matriu... + modifica la matriu perqu� s�c aixi de coherent
    if(m.minx<0){
        for(int i=0;i<_columnes;i++)
            _mat[i][1]-=m.minx;
        m.maxx-=m.minx;
        m.minx=0;
    }
    if(m.miny<0){
        for(int i=0;i<_columnes;i++)
            _mat[i][0]-=m.miny;
        m.maxy-=m.miny;
        m.miny=0;
    }
    //const float /*rang_usable_x=m.maxx-m.minx, rang_usable_y=m.maxy-m.miny,*/
    /*multiplicador_x=(TAMANY_HORIT-1)/m.maxx, multiplicador_y=(TAMANY_VERT-1)/m.maxy;
    for(int i=0;i<_columnes;i++){
        _mat[i][0]*=multiplicador_y;
        _mat[i][1]*=multiplicador_x;
    }*/
    /*for(int i=0;i<_columnes;i++){
        _mat[i][0]=(_mat[i][0]/(m.maxy-m.miny))*(TAMANY_VERT-1);
        _mat[i][1]=(_mat[i][1]/(m.maxx-m.minx))*(TAMANY_HORIT-1);
    }*/
    static const float CONST_MULT_Y=(TAMANY_VERT-1)/4, CONST_MULT_X=(TAMANY_HORIT-1)/4;
    for(int i=0;i<_columnes;i++){
        _mat[i][0]*=CONST_MULT_Y;
        _mat[i][1]*=CONST_MULT_X;
    }
}

const int maxim_model_xy=2;
/*
void matriu::preparar_matriu2(){ //aixo no �s manera de fer les coses, nom�s un apanyo temporal per veure algun resultat. No hauriem de modificar la matriu si no tractar les dades al escriure al fb
    for(int i=0;i<_columnes;i++){
        _mat[i][0]+=maxim_model_xy;
        _mat[i][0]=(_mat[i][0]/(maxim_model_xy*2))*TAMANY_VERT;
        _mat[i][1]+=maxim_model_xy;
        _mat[i][1]=(_mat[i][1]/(maxim_model_xy*2))*TAMANY_HORIT;
    }
}
*/
void matriu::escriure_fb(char fb[][(int)TAMANY_VERT]){
    for(int i=0;i<_columnes;i++){ //per cada punt...
        //debuk
        int a=(int)_mat[i][1], b=(int)_mat[i][0];
        if(a>=TAMANY_HORIT or a<0)
            cout<<"OOB: a="<<a<<" i= "<<i<<endl;
        else if(b>=TAMANY_VERT or b<0)
            cout<<"OOB: b="<<b<<" i= "<<i<<endl;
        else fb[a][b]='#';
        //fb[(int)_mat[i][1]][(int)_mat[i][0]]='#'; //festa de out of bounds writes. Realment no ha sigut la meva millor linia de codi
    }

}

//!Pre: Matriu quadrada de n>=3
/*float determinant_r3plus(){
    if(_files!=_columnes){ //si no es quadrada no podem fer el det
        cout<<"No es pot fer el determinant"<<endl;
        return 0;
    }

    const int col_sel=0; //!hardcodejat per ara, al deshardcodejar pensar en que el SIGNE del primer de la columna no �s sempre +!
    float res;
    if(_files==3){
        res=determinant_3x3();
    }
    else{
        res=0;
        int tamany=_files*_columnes;
        float *temp_arr=new float[tamany]; //antic candidat a memory leak. Punters expl�cits = mal
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
}*/ //useless perqu� per moure una matriu l'objecte ha de ser un lvalue i per obtenir l'lvalue hem de fer una copia, que fa que aixo perdi tot el sentit
//res.moure(res.producte(1/A)); //f*** els return values son rvalues

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
