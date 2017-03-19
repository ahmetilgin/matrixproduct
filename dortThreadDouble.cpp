#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <pthread.h>

using namespace std;

double matrixA[1000][1000];
double matrixB[1000][1000];
double matrixC[1000][1000];

void matrisleriDoldur(int);
void seriCarpim(int);
void paralelCarpim(int);
void *cellSum(void *args);
void displayMatrix(int);

// pthreada artgumanları geçirmek için oluşturulmuş trhead
struct arg_struct{
    int i;
    int satirSayisi;
    int baslangic;
    int bitis;
};

int main(){
	// Matris A ve B ye rastgele double değişkenler atıp doldurduktan sonra MAtrisC yi yani çarpım sonucu olan matrisi 0 lıyor aynı zamanda
	matrisleriDoldur(1000);
    cout<<"****\nDOUBLE Degiskenler çarpılıyor...\nMatris 4 parcaya ayrıldı 4 thread ayrı ayrı çalıştı.."<<endl;


	struct timespec start, finish;		//Gecen Süre hesaplamak için başlangıc ve bitiş zamanı
	double elapsed;   // gecen süre

	clock_gettime(CLOCK_MONOTONIC, &start);  // Saati başlat Seri Çarpım sırasında ki geçen süre hsesabı için
	seriCarpim(1000);
	clock_gettime(CLOCK_MONOTONIC, &finish); // Saati Bitir

	elapsed = (finish.tv_sec - start.tv_sec);  // Gecen Süre hesabı
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;  // Gecen süre hesabı için gerekli çevirme nanodan saniyeye
	cout << "Seri Carpim İcin Gecen Sure " << elapsed << " saniye";
	cout << endl;

	clock_gettime(CLOCK_MONOTONIC, &start);  // Aynı şekilde paralel çarpma için yapılıyor
	paralelCarpim(1000);					//Paralel çarpım işlemi 1000 satır ve sutun sayısı 
	clock_gettime(CLOCK_MONOTONIC, &finish);

	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	cout << "Paralel Carpim İcin Gecen Sure: " << elapsed << " Saniye";
	cout << endl;
	//displayMatrix(5); // ilk 5 elemanını yazdır.
}

//To fill the two matrices with random values and to clean matrix C.
void matrisleriDoldur(int satirSayisi){
	
	srand(time(NULL)); // rastgeleliği değişken yapmak için

	for(int i = 0; i < satirSayisi; i++){
		for(int j =0; j< satirSayisi; j++){
            // ilk matris
			matrixA[i][j] = rand();
            // ikinci
			matrixB[i][j] = rand();
			// çarpım sonucu 
			matrixC[i][j] = 0;
		}
	}
}

//Sonucları Test etmek için yapıldı 
//İlk n lik kısmı yazıyor
void displayMatrix(int n){
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			cout << " " << matrixC[i][j];
			cout << " ";

		}
		cout << endl;
	}
}

//Matris seri çarpım
void seriCarpim(int satirSayisi){
	//ara toplamı tutan değişken
	int sum = 0;

	//double firstMatrixRow [satirSayisi];
	//double secondMatrixColumn [satirSayisi];


	for (int i = 0; i < satirSayisi; i++){ // ilk matrisin satırları üzerinde hareket için
		for (int j = 0; j < satirSayisi; j++){ // ikinci matrisin sutunu üzerinde hareket için
			sum = 0; // matrislerde elemanarın çarpımları sonrası ara toplamı sıfırlamak gerek onun için
			for (int k = 0; k < satirSayisi; k++){  // ilk matrisde satir üzerinde ilerken ikinci matriste sutun üzerinde hareket sağlandı
				sum = sum + matrixA[i][k] * matrixB[k][j];  // çarpımları ara toplam şekilde ifade ediyoruz
			}
			matrixC[i][j] = sum;  // en son bitince toplam sonucunu sonuc matrisine yaz
		}
	}
	cout << endl;
}

//Paralel Carpma
void paralelCarpim(int satirSayisi){

	pthread_t threads[(4)];  // satır sayısı kadar thread belirle
	for (int i = 0; i < 4; i++){			//Elde edilen her matris için bir thread oluşturuyoruz
			// burda amaç cellSum fonksiyonuna birdne fazla arguman geçirmek bunun için struct kullandık
			struct arg_struct args;
			// i burda hangi satırda olduğumuzu söylüyor

			args.baslangic = i * ( satirSayisi / 4);
			args.bitis = args.baslangic + satirSayisi / 4;
			args.satirSayisi = satirSayisi;
			// Her satır için ayrı thread oluşturduk. cellSum fonksiyon fonksiyonları 2 vektorun elemanlarını çarpıp topluyor MAtris C ye yazıyor
			pthread_create(&threads[i], NULL,cellSum, (void *)&args); // Args da i ve satirSayısını geçirebilmek için Bide void istiyor
	}
	for (int i = 0; i < 4; i++){
		pthread_join(threads[i], NULL);			//Tüm threadların işlerini bitirmesini bekliyoruz.

	};
}

void *cellSum(void *arguments){
	// Burda dönüşüm gerekti void olarak aldığımız için
	struct arg_struct *args = (struct arg_struct *)arguments; 

	// Fonksiyona geçirilen i değerini okuyoruz
	int i = (int)args->i;					
	int baslangic = (int)args->baslangic;
	int bitis = (int) args -> bitis;
	int satirSayisi = (int)args->satirSayisi; //fonksiyona geçirilen satır Sayisini alıyoruz
	for (int i = baslangic; i < bitis; i++){ // ilk matrisin satırları üzerinde hareket için
		for (int j = 0; j < satirSayisi; j++){ // ikinci matrisin sutunu üzerinde hareket için
			int sum = 0; // matrislerde elemanarın çarpımları sonrası ara toplamı sıfırlamak gerek onun için
			for (int k = 0; k < satirSayisi; k++){  // ilk matrisde satir üzerinde ilerken ikinci matriste sutun üzerinde hareket sağlandı
				sum = sum + matrixA[i][k] * matrixB[k][j];  // çarpımları ara toplam şekilde ifade ediyoruz
			}
			matrixC[i][j] = sum;  // en son bitince toplam sonucunu sonuc matrisine yaz
		}
	}
}
