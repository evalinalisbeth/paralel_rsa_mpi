#include<stdio.h>
#include<mpi.h>
#include<string.h>
#include<math.h>


int komprima(int prima, int sigma_N); //MENENTUKAN BILANGAN PRIMA
long int BigMod(int b,int p,int m);
long int square(long long a);
void loadStringFromFile(char *filename, char *s);
int kunci_rahasia(int e, int sigma_N);
void dekripsi(int c[], int d,long N,int strlength,char text[20]);
const int SIZE = 10000;

int main (int argc, char* argv[]){
	int i,start,end, strlength;
	int m[SIZE], c[SIZE];
	char text[SIZE];
	int rank, jumlahProc;
	int N, sigma_N, found;
	int p,q,e,d;
	int status_prima;
	loadStringFromFile("text.txt",text); //membaca text
    strlength=strlen(text);
    printf("banyak karakter : %i\n",strlength);
	MPI_Init (&argc, &argv);
	MPI_Status status;
	MPI_Comm_rank (MPI_COMM_WORLD, &rank); // baca id prosesor
	MPI_Comm_size (MPI_COMM_WORLD, &jumlahProc);  // baca banyak prosesor
	int RANKSIZE = strlength/(jumlahProc-1);
	int modRANKSIZE=strlength%(jumlahProc-1);
	
	/*if(modRANKSIZE!=0){
		printf("tidak diproses");
		MPI_Abort(MPI_COMM_WORLD,911);
	}*/
	if(rank==0){
		p=19;
		q=17;
		e=23;
		N=p*q;
		sigma_N=(p-1)*(q-1);
		status_prima=koprima(e,sigma_N);
		if(status_prima==1){
			for(i=0;i<SIZE;i++){
				m[i]=text[i];
			}
			MPI_Bcast (&e, 1, MPI_INT, 0, MPI_COMM_WORLD);
			MPI_Bcast (&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
			MPI_Bcast (&m[0],strlength, MPI_INT, 0, MPI_COMM_WORLD);
			
			for (i=1; i<jumlahProc; i++){
				start= (i-1)*RANKSIZE;
				end=(i*RANKSIZE)-1;
				MPI_Send(&start,1,MPI_INT,i,0,MPI_COMM_WORLD);
				MPI_Send(&end,1,MPI_INT,i,0,MPI_COMM_WORLD);
			}
			for (i=1; i<jumlahProc; i++){
				start= (i-1)*RANKSIZE;
				MPI_Recv(&c[start],RANKSIZE,MPI_INT,i,0,MPI_COMM_WORLD,&status);
				printf("%d receive dari array enkripsi . Messagesource = %d\n", rank, status.MPI_SOURCE);
			}
			
			FILE* data;
			data=fopen("ENKRIPSI_RSA.txt","wb");
			for(i=0;i<strlength;i++){
				fprintf(data,"%i\n",c[i]);
			}
			fclose(data);
			
			d=kunci_rahasia(e,sigma_N);
			dekripsi(c,d,N,strlength,"DESKRIPSI_RSA.txt");
			
		}
	
	}
	else {
		MPI_Status status;
		MPI_Bcast (&e, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast (&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast (&m[0], strlength, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Recv(&start,1,MPI_INT,0,0,MPI_COMM_WORLD,&status);
		MPI_Recv(&end,1,MPI_INT,0,0,MPI_COMM_WORLD,&status);
		printf("%d receive vaiabel e = %d dan N = %d dari induk\n",e,N,rank);    
		printf("start : %d",start);
		printf("end : %d",end);
		for(i=start;i<=end;i++){
			c[i]=BigMod(m[i],e,N);
			printf("rank ke%i %i %i\n ",rank,i,m[i]);
		}
		MPI_Send(&c[start],RANKSIZE,MPI_INT,0,0,MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return 0;
}
void loadStringFromFile(char *filename, char *s){
    FILE *fp;
	int i,j;
    if((fp=fopen(filename, "r"))==NULL) {
        printf("Cannot open file.\n");
    } else {
        char x[10000];
        fgets(x,10000,fp);
        fclose(fp);
        strcpy(s,x);
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

long int BigMod(int b,int p,int m){
    if(p==0)return 1;
    else if(p%2==0)return square(BigMod(b,p/2,m))%m;
    else return ((b % m) * BigMod(b,p-1,m)) % m;
}
long int square(long long a){
    return a*a;
}

void dekripsi(int c[], int d,long N,int strlength,char text[20]){
    FILE * data2;
    data2 = fopen(text,"wb");
	int z[10000];
	int i;
    for(i=0;i<strlength;i++){
       z[i]=BigMod(c[i],d,N);
       fprintf(data2,"%c",z[i]);
    }
	fclose(data2);
}

int kunci_rahasia(int e, int sigma_N){
    int d;
    int ketemu=0;
    int i=0;
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
