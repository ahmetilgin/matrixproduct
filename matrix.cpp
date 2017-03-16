#include <pthread.h>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <iostream>

using namespace std;



vector <vector<double>> matrisCarpimi(vector<double>ilkVector, vector<vector<double>>matrix){

matrix.push_back(ilkVector);

for(int i = 0 ; i < ilkVector.size();i++){
    cout<<ilkVector[i]<<endl;;
}

return matrix;
}

int main(){
vector <double> dizi;
vector<vector<double>>matrix;
matrix.push_back(dizi);
srand(time(NULL));


for(int i = 0 ; i < 10;i++){
    dizi.push_back(rand());
}
matrix.push_back(dizi);

// Böyle
//pthread_t carpimThread;
//
//pthread_create (&carpimThread, NULL, matrisCarpimi,(dizi,matrix));




matrisCarpimi(dizi,matrix);

return 0;
}
