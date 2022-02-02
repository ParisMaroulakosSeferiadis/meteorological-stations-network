#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define N 288 /* frame dimension for QCIF format */
#define M 352 /* frame dimension for QCIF format */
#define filename "akiyo_cif_0_yuv444.yuv"
#define file_y "akiyo2y.yuv"



/* code for armulator*/
#pragma arm section zidata="ram"
int current_y[N][M];
int gauss[N+2][M+2];
int newgauss[N+2][M+2];
int nonmaximum[N+2][M+2];
int threshold[N+2][M+2];
int sobel[N+2][M+2];
int newsobelx[N][M];
int newsobely[N][M];
double es[N][M];
float  eo[N][M];
double z=0;
float k=0;
int q=0;
int r=0;
int t1=70;
int t2=60;

#pragma arm section

int i,j;


void canny(){


	for(i=0;i<N+2;i++){
		for(j=0;j<M;j+=4){
			gauss[i][j]=0;
			gauss[i][j+1]=0;
			gauss[i][j+2]=0;
			gauss[i][j+3]=0;
			sobel[i][j]=0;
			sobel[i][j+1]=0;
			sobel[i][j+2]=0;
			sobel[i][j+3]=0;
		    nonmaximum[i][j]=0;
			nonmaximum[i][j+1]=0;
			nonmaximum[i][j+2]=0;
			nonmaximum[i][j+3]=0;
            threshold[i][j]=0;
			threshold[i][j+1]=0;
			threshold[i][j+2]=0;
			threshold[i][j+3]=0;
	    } 
		gauss[i][352]=0;
		gauss[i][353]=0;
		sobel[i][352]=0;
		sobel[i][353]=0;
		nonmaximum[i][352]=0;
		nonmaximum[i][353]=0;
		threshold[i][352]=0;
		threshold[i][353]=0;
    }
           
            


	for(i=0;i<N+2;i++){
		for(j=0;j<M;j+=4){
			gauss[i][j]=current_y[i-1][j-1];
		    gauss[i][j+1]=current_y[i-1][j];
			gauss[i][j+2]=current_y[i-1][j+1];
			gauss[i][j+3]=current_y[i-1][j+2];
		}
	}

	for(i=1;i<N+1;i++)
		for(j=1;j<M+1;j+=4){
			newgauss[i][j]=(gauss[i][j] *(4) + // pollaplasiazei to idio stoixeio me 4
                          gauss[i-1][j-1] *(1) +   // stoixeio panw aristera
                          gauss[i][j-1]   *(2) +   // stoixeio aristera
                          gauss[i+1][j-1] *(1) +   // stoixeio katw aristera
                          gauss[i-1][j]   *(2) +   // stoixeio apo panw
                          gauss[i+1][j]   *(2) +   // stoixeio apo katw
                          gauss[i-1][j+1] *(1) +   // stoixeio panw deksia
                          gauss[i][j+1]   *(2) +   // stoixeio deksia
                          gauss[i+1][j+1] *(1)  // stoixeio katw deksia
                          );
			newgauss[i][j+1]=(gauss[i][j+1] *(4) + // pollaplasiazei to idio stoixeio me 4
                          gauss[i-1][j]  *(1) +   // stoixeio panw aristera
                          gauss[i][j]    *(2) +   // stoixeio aristera
                          gauss[i+1][j]  *(1) +   // stoixeio katw aristera
                          gauss[i-1][j+1]*(2) +   // stoixeio apo panw
                          gauss[i+1][j+1]*(2) +   // stoixeio apo katw
                          gauss[i-1][j+2]*(1) +   // stoixeio panw deksia
                          gauss[i][j+2]  *(2) +   // stoixeio deksia
                          gauss[i+1][j+2]*(1)  // stoixeio katw deksia
                          );
			newgauss[i][j+2]=(gauss[i][j+2] *(4) + // pollaplasiazei to idio stoixeio me 4
                          gauss[i-1][j+1] *(1) +   // stoixeio panw aristera
                          gauss[i][j+1]   *(2) +   // stoixeio aristera
                          gauss[i+1][j+1] *(1) +   // stoixeio katw aristera
                          gauss[i-1][j+2]   *(2) +   // stoixeio apo panw
                          gauss[i+1][j+2]   *(2) +   // stoixeio apo katw
                          gauss[i-1][j+3] *(1) +   // stoixeio panw deksia
                          gauss[i][j+3]   *(2) +   // stoixeio deksia
                          gauss[i+1][j+3] *(1)  // stoixeio katw deksia
                          );
			newgauss[i][j+3]=(gauss[i][j+3] *(4) + // pollaplasiazei to idio stoixeio me 4
                          gauss[i-1][j+2] *(1) +   // stoixeio panw aristera
                          gauss[i][j+2]   *(2) +   // stoixeio aristera
                          gauss[i+1][j+2] *(1) +   // stoixeio katw aristera
                          gauss[i-1][j+3]   *(2) +   // stoixeio apo panw
                          gauss[i+1][j+3]   *(2) +   // stoixeio apo katw
                          gauss[i-1][j+4] *(1) +   // stoixeio panw deksia
                          gauss[i][j+4]   *(2) +   // stoixeio deksia
                          gauss[i+1][j+4] *(1)  // stoixeio katw deksia
		                  );
		}			  

	for(i=1;i<N+1;i++)
		for(j=1;j<M+1;j+=4){
			current_y[i-1][j-1]=newgauss[i][j]/16;
			current_y[i-1][j]=newgauss[i][j+1]/16;
			current_y[i-1][j+1]=newgauss[i][j+2]/16;
			current_y[i-1][j+2]=newgauss[i][j+3]/16;
            sobel[i-1][j-1]=current_y[i-1][j-1];
			sobel[i-1][j]=current_y[i-1][j];
			sobel[i-1][j+1]=current_y[i-1][j+1];
			sobel[i-1][j+2]=current_y[i-1][j+2];
        }
    




//sobel 
   	for(i=1;i<N+1;i++)
		for(j=1;j<M+1;j++)
	     newsobelx[i][j]=(sobel[i][j] *(0) + // pollaplasiazei to idio stoixeio me 0
                          sobel[i-1][j-1] *(-1) +   // stoixeio panw aristera
                          sobel[i][j-1]   *(-2) +   // stoixeio aristera
                          sobel[i+1][j-1] *(-1) +   // stoixeio katw aristera
                          sobel[i-1][j]   *(0) +   // stoixeio apo panw
                          sobel[i+1][j]   *(0) +   // stoixeio apo katw
                          sobel[i-1][j+1] *(1) +   // stoixeio panw deksia
                          sobel[i][j+1]   *(2) +   // stoixeio deksia
                          sobel[i+1][j+1] *(1)  // stoixeio katw deksia
                          );
		 				  
     for(j=1;j<M+1;j++)
        for(i=1;i<N+1;i++)                           
         newsobely[i][j]=(sobel[i][j] *(0) + // pollaplasiazei to idio stoixeio me 0
                          sobel[i-1][j-1] *(1) +   // stoixeio panw aristera
                          sobel[i][j-1]   *(0) +   // stoixeio aristera
                          sobel[i+1][j-1] *(-1) +   // stoixeio katw aristera
                          sobel[i-1][j]   *(2) +   // stoixeio apo panw
                          sobel[i+1][j]   *(-2) +   // stoixeio apo katw
                          sobel[i-1][j+1] *(1) +   // stoixeio panw deksia
                          sobel[i][j+1]   *(0) +   // stoixeio deksia
                          sobel[i+1][j+1] *(-1)  // stoixeio katw deksia
                          );
         
                                     
	for(i=0;i<N;i++)
		for(j=0;j<M;j++)
        {    
		    if (newsobely[i][j]==0){
		        newsobely[i][j]=1;
		    }   
       
             z=(newsobelx[i][j]*newsobelx[i][j])+(newsobely[i][j]*newsobely[i][j]);
             k= (float)newsobelx[i][j]/(float)newsobely[i][j];
		     es[i][j]=sqrt(z);//plath eikostoixeiwn
		     eo[i][j]=atan(k);//klisi eikonostoixeiwn       
          }
          
          
	for(i=1;i<N+1;i++)
		for(j=1;j<M+1;j+=4){
			current_y[i-1][j-1]=es[i][j];
			current_y[i-1][j]=es[i][j+1];
			current_y[i-1][j+1]=es[i][j+2];
			current_y[i-1][j+2]=es[i][j+3];
        }
      
      //non maximun
    
      for(i=1;i<N;i++)
		for(j=1;j<M;j+=4){
			eo[i][j]=(eo[i][j]*180)/3.14;
			eo[i][j+1]=(eo[i][j+1]*180)/3.14;
			eo[i][j+2]=(eo[i][j+2]*180)/3.14;
			eo[i][j+3]=(eo[i][j+3]*180)/3.14;
            if (eo[i][j]<0){
		        eo[i][j]+=180;
		    } 
		    if (eo[i][j+1]<0){
		        eo[i][j+1]+=180;
		    } 
		    if (eo[i][j+2]<0){
		        eo[i][j+2]+=180;
		    } 
		    if (eo[i][j+3]<0){
		        eo[i][j+3]+=180;
		    }     
      }  
     
	for(j=1;j<M-1;j++)   
		for(i=1;i<N-1;i++){
		    q=0;
		    r=0;  
		    if ((0<=eo[i][j] && eo[i][j] <22.5) || (157.5 <=eo[i][j] &&eo[i][j] <= 180)){
		        q =current_y[i][j+1];
                r =current_y[i][j-1]; 
		        }  
            else if (22.5<=eo[i][j] && eo[i][j]<67.5){
                q =eo[i+1][j-1];
                r =eo[i-1][j+1];
                } 
            else if (67.5<=eo[i][j]&& eo[i][j]<112.5){
                q = current_y[i+1][j];
                r = current_y[i-1][j];
           
                } 
            else if (112.5<=eo[i][j] && eo[i][j]<157.5){
                q =current_y[i-1][j-1];
                r= current_y[i+1][j+1];
                } 
                 
             
             if(current_y[i][j]>=q || current_y[i][j]>=r){
                 nonmaximum[i][j]=current_y[i][j];
                 } 
              else {
                 nonmaximum[i][j]=0;
                   }  
                  
                       
      }
        
    for(i=1;i<N+1;i++)
		 for(j=1;j<M+1;j+=4){
			current_y[i-1][j-1]=nonmaximum[i][j];
			current_y[i-1][j]=nonmaximum[i][j+1];
			current_y[i-1][j+1]=nonmaximum[i][j+2];
			current_y[i-1][j+2]=nonmaximum[i][j+3];
        }  
      
       
   //thresholding

        for(j=0;j<M;j++)
		     for(i=0;i<N;i++){
         
            if (current_y[i][j]<=t1 && current_y[i][j]>=t2){
             threshold[i][j]=100;            
        }
            else if (current_y[i][j]>t1){
               threshold[i][j]=255;              
               }
            else if (current_y[i][j]<t2) {
                 threshold[i][j]=0;           
                }                           
       }    
      
	for(i=1;i<N-1;i++)
		for(j=1;j<M-1;j++){
             if (threshold[i][j]==100){
                 if(threshold[i+1][j-1] == 255 || threshold[i+1][j] == 255 || threshold[i+1][j+1] == 255
                        || threshold[i][j-1] == 255 || threshold[i][j+1] ==255
                        || threshold[i-1][j-1] == 255 || threshold[i-1][ j] == 255 || threshold[i-1][j+1] == 255)
                        {
                            threshold[i][j]=255;
                            }
                 else{
                            threshold[i][j]=0;
                            }
                 }
      }
      for(i=1;i<N+1;i++)
		 for(j=1;j<M+1;j+=4){
            current_y[i-1][j-1]=threshold[i][j];
            current_y[i-1][j]=threshold[i][j+1];
            current_y[i-1][j+1]=threshold[i][j+2];
            current_y[i-1][j+2]=threshold[i][j+3];

      }
      
      
}
void read()
{
  FILE *frame_c;
  if((frame_c=fopen(filename,"rb"))==NULL)
  {
    printf("current frame doesn't exist\n");
    getchar();
   
    exit(-1);
  }

  for(i=0;i<N;i++)
  {
    for(j=0;j<M;j++)
    {
      current_y[i][j]=fgetc(frame_c);
    }
  }
}

void write()
{
  FILE *frame_y;
  frame_y=fopen(file_y,"wb");

  for(i=0;i<N;i++)
  {
    for(j=0;j<M;j++)
    {
      fputc(current_y[i][j],frame_y);
    }
  }
  fclose(frame_y);
 
}

int main()
{
  read();
  canny();
  write();
 
}
