#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<math.h>

int komprima(int prima, int sigma_N); //MENENTUKAN BILANGAN PRIMA
long int BigMod(int b,int p,int m);
long int square(long long a);
void loadStringFromFile(char *filename, char *s);
void enkripsi(long int k_ascii[],int e,long N,int count,char text[6]);
int kunci_rahasia(int e, int sigma_N);
void dekripsi(int d,long N,int count,char text[20]);
int i;
int z[100000];

//19 17 23

int main(){
    int p,q,d,sigma_N,e,found;
    long N;
    long int k_ascii[100000];
    int count=0;
    char kata[8]="HARI INI";
    //loadStringFromFile("text.txt",kata);
    //int strlength;
    //strlength=strlen(kata);
//    printf("banyak karakter : %i\n",strlength);
    printf("tugas besar\n");
    printf("masukkan nilai p : ");
    scanf("%i",&p);
    printf("masukkan nilai q : ");
    scanf("%i",&q);
    N=p*q;
    sigma_N=(p-1)*(q-1);
    printf("nilai N adalah %d dan sigma_N adalah %d\n",N,sigma_N);
    printf("masukkan e : ");
    scanf("%i",&e);
    found=koprima(e,sigma_N);
    printf("e termasuk prima %d\n",found);
    d=kunci_rahasia(e,sigma_N);
    printf("kunci rahasia %d\n",d);
    if(found==1){
        for(i=0;kata[i]!=NULL;i++){
            k_ascii[i]=kata[i];
            printf("ascii %i \n",k_ascii[i]);
            count++;
        }
        printf("banyak karakter : %i\n",count);
        enkripsi(k_ascii,e,N,count,"hasil_rsa.txt");
        printf("\n");
        printf("\n");
        dekripsi(d,N,count,"rsa_hasil.txt");
    }else{
        printf("kunci  salah");
    }
}


int koprima(int prima, int sigma_N){
    int found=0;
    int i=0;
    if(prima==2){
        found=1;
    }else{
        for(i=2;i<prima;i++){
            if(prima%i==0){
                found=0;
                break;
            } else {
                found=1;
            }
        }
    }
    int r;
    while((r=prima % sigma_N)!=0){
        prima=sigma_N;
        sigma_N=r;
    }
    return sigma_N;
}


int kunci_rahasia(int e, int sigma_N){
    int d;
    int ketemu=0;
    i=0;
    int r;
    while(ketemu==0 && i<e){
        d=(1+(i*sigma_N))/e;
        if ((d*e)%sigma_N==1) {
            ketemu=1;
        }else{
            i++;
        }
    }
    return d;
}
void loadStringFromFile(char *filename, char *s){
    FILE *fp;
	int i,j;
    if((fp=fopen(filename, "r"))==NULL) {
        printf("Cannot open file.\n");
    } else {
        char x[100000];
        fgets(x,100000,fp);
        fclose(fp);
        strcpy(s,x);
    }
}


long int BigMod(int b,int p,int m){
    if(p==0)return 1;
    else if(p%2==0)return square(BigMod(b,p/2,m))%m;
    else return ((b % m) * BigMod(b,p-1,m)) % m;

}
long int square(long long a){
    return a*a;
}

void enkripsi(long int k_ascii[],int e,long N,int count,char text[20]){
    FILE * data;
    data = fopen(text,"wb");
    FILE * numerik;
    numerik = fopen("rsa_hasil.txt","wb");
    for(i=0;i<count;i++){
       z[i]=BigMod(k_ascii[i],e,N);
       printf("%i\n",z[i]);
       fprintf(data,"%i",z[i]);
    }
    fclose(data);
    for(i=0;i<count;i++){
        fprintf(numerik,"%i\n",z[i]);
    }
    fclose(numerik);

}

void dekripsi(int d,long N,int count,char text[20]){
    FILE * data;
    data = fopen(text,"rb");
    for(i=0;i<count;i++){
        fscanf(data,"%i",&z[i]);
        //printf("%i\n",z[i]);
    }
    fclose(data);
    printf("\n");
    printf("\n");
    for(i=0;i<count;i++){
       z[i]=BigMod(z[i],d,N);
       printf("%i",z[i]);
    }
    printf("\n");
    printf("\n");
    FILE * dekrip;
    dekrip = fopen("dekripsi.txt","wb");
    for(i=0;i<count;i++){
        fprintf(dekrip,"%c",z[i]);
        printf("%c",z[i]);
    }

}
