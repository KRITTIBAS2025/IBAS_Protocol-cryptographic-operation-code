#include "/home/pi/PBC/pbc-0.5.14/include/pbc.h"
#include <stdint.h>
#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
//#include "pbc.h"
#include "/home/pi/PBC/pbc-0.5.14/include/pbc_test.h"

#define SUCCESS 0
#define FAILURE -1



void Display_pairing_element(pairing_t* pairing){
    printf("--------- Verifying Pairing properties -----------\n");
    printf("[DEBUG] Pairing is symmetric: %s\n",(pairing_is_symmetric(*pairing)? "TRUE": "FALSE"));
    printf("[DEBUG] Group G1 bit size: %d\n", (pairing_length_in_bytes_G1(*pairing) * 8));
    printf("[DEBUG] Group G2 bit size: %d\n", (pairing_length_in_bytes_G2(*pairing) * 8));
    printf("[DEBUG] Group GT bit size: %d\n", (pairing_length_in_bytes_GT(*pairing) * 8));
    printf("[DEBUG] Group Zr bit size: %d\n", (pairing_length_in_bytes_Zr(*pairing) * 8));
}

int main(){
	
 	pairing_t pairing;
	char param[15360];
	size_t count = fread(param, 1, 15360, stdin);
	if (!count) {
		printf("Failure in reading input");
		return 0;
		}

	pairing_init_set_buf(pairing, param, count); // initialize pairing
	
	Display_pairing_element(&pairing);   // Display pairing element
	
	int i,n=100; 
	double tem,avg,sum=0,sum1,avg1,t1,t2;
	clock_t start, end;	
	//////////////////////////////////////////////	
	printf("\n No of Repeatition =%d",n);
	for(i=1;i<=n;i++){
	element_t g, h,out;
	
	element_init_G1(h, pairing);
	element_init_G2(g, pairing);
	element_init_GT(out,pairing);
	element_random(g);
	element_random(h);
	element_random(out);
	//element_printf("[DEBUG] Chosen a Group element: %B\n", g);
	//element_printf("[DEBUG] Chosen b group : %B\n", h);
	//element_printf("[DEBUG] Chosen out element: %B\n", out);
	
	
	//start = clock();
	t1 = pbc_get_time();
	element_pairing(out, g,  h); 							//out=e(g,h)
 	//end = clock();
 	t2 = pbc_get_time();
	//tem=((double) (end - start)) / CLOCKS_PER_SEC;
	tem=t2-t1;
	sum=sum+tem;
	element_clear(g);
	element_clear(h);
	element_clear(out);
	}
	
	avg=sum/n;
	//printf(" \n Execution time(ms) for Pairing Operation new =%f \n",avg);	
	printf(" \n Execution time(ms) for Bilinear Pairing Operation=%f ",avg*1000);
				
	/////////////////////////////////////////////
	
	
	 sum=0;
	for(i=1;i<=n;i++){
	element_t h;element_t sig1;
	element_init_G1(h, pairing);
	element_random(h);
	element_init_G1(sig1, pairing);
	element_random(sig1);
	int nn = pairing_length_in_bytes_x_only_G1(pairing);
	unsigned char *data = malloc(nn);
	element_to_bytes_compressed(data, sig1);	
	
	char *str="KRITTIBAS PARAI";
	//char *str ="ABCDEFGHIJKLMNOPQRSTUVWXYZABCDABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHABCDEFGHIJKLMNOPQRSTUVWXYZABCDABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHABCDEFGHIJKLMNOPQRSTUVWXYZABCDABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHABCDEFGHIJKLMNOPQRSTUVWXYZABCDABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGH";
	t1 = pbc_get_time();	
	element_from_hash(h, str,strlen(str));  							//Map to point
//element_printf("[DEBUG] Chosen after mat to point element: %B\n", h);
	
	t2 = pbc_get_time();
	//tem=((double) (end - start)) / CLOCKS_PER_SEC;
	tem=(t2-t1);
	sum=sum+tem;
	element_clear(h);
	}
	//element_printf("[DEBUG] Chosen after mat to point element: %B\n", sig1);
	avg=sum/n;
	printf(" \n Execution time (ms) MAP to point hash Operation=%f ",avg*1000);
	
	//////////////////////////////////////////////////////////
	
	sum=0;
	int rr;
	rr=99999;
	for(i=1;i<=n;i++){
	element_t Pr;  element_t group_generator;  	element_t randomness;	
	//rr=rand();
	element_init_Zr(randomness, pairing);
	element_random(randomness);
	element_init_G1(group_generator, pairing);
	element_random(group_generator);
	element_init_G1(Pr, pairing);
	element_random(Pr);
	
	t1 = pbc_get_time();
	element_mul_zn(Pr, group_generator, randomness); 
	//element_mul_si(Pr, group_generator, rr); 					//Pr = r.P
	//element_mul(Pr, group_generator, randomness);
	t2 = pbc_get_time();

	//tem=((double) (end - start)) / CLOCKS_PER_SEC;
	tem=t2-t1;
	sum=sum+tem;
	element_clear(Pr);
	element_clear(randomness);
	element_clear(group_generator);
	}
	avg=sum/n;
	printf(" \n Execution time ms for Point multiplication Operation=%f ",avg*1000);
	
	///////////////////////////////////////////////////////////////////////
	sum=0;
	for(i=1;i<=n;i++){	
	
	element_t n1; element_t a;element_t b;
	element_init_G1(a, pairing);
	element_random(a);
	element_init_G1(b, pairing);
	element_random(b);
	element_init_G1(n1, pairing);
	element_random(n1);

	t1 = pbc_get_time();
	element_add( n1,  a,  b); 				//n=a+b
	t2 = pbc_get_time();

	//tem=((double) (end - start)) / CLOCKS_PER_SEC;
	tem=t2-t1;
	sum=sum+tem;
	element_clear(a);
	element_clear(b);
	element_clear(n1);
	}
	avg=sum/n;
	printf(" \n Execution time ms for Point Addition Operation=%f ",avg*1000);
	///////////////////////////////////////////////////////////////////////
	/*
	sum=0;
	for(i=1;i<=n;i++){	
		
	char *data ="ABCDEFGHIJKLMNOPQRSTUVWXYZABCDABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHABCDEFGHIJKLMNOPQRSTUVWXYZABCDABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHABCDEFGHIJKLMNOPQRSTUVWXYZABCDABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHABCDEFGHIJKLMNOPQRSTUVWXYZABCDABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGH";
	
	element_t e;
	element_init_G1(e, pairing);
	element_random(e);
	start = clock();				
	int x=element_to_bytes(data, e);		//Hash
	//int y=element_from_bytes(e, data);
	end = clock();
	tem=((double) (end - start)) / CLOCKS_PER_SEC;
	sum=sum+tem;
	element_clear(e);
	}
	avg=sum/n;
	printf(" \n Execution time for Hash( meaasge digest) Operation=%f \n",avg*1000);
	*/
	/////////////////////////////////////////////////////////////////////
	/*
	sum=0;
	sum1=0;
	for(i=1;i<=n;i++){
	element_t g,h,x,y,sig;
	element_t temp1, temp2;
	element_init_G2(g, pairing);
	element_init_G2(y, pairing);
	element_init_G1(h, pairing);
	element_init_G1(sig, pairing);
	element_init_GT(temp1, pairing);
	element_init_GT(temp2, pairing);
	element_init_Zr(x, pairing);
	element_random(h);
	element_random(g);
	element_random(y);
	element_random(x);
	element_random(sig);
	element_random(temp1);
	element_random(temp2);
	
	t1 = pbc_get_time();
	element_pow_zn(y, g, x);  //y=g^x
	t2 = pbc_get_time();
	//tem=((double) (end - start)) / CLOCKS_PER_SEC;
	tem=t2-t1;
	sum=sum+tem;
	element_pow_zn(sig, h, x); // sin=h^x
	
	
	t1 = pbc_get_time();
	pairing_apply(temp1, sig, g, pairing);  //temp1=sin^g =h^xg
	t2 = pbc_get_time();
	//tem=((double) (end - start)) / CLOCKS_PER_SEC;
	tem=t2-t1;
	sum1=sum1+tem;
	
	element_clear(y);
	element_clear(x);
	element_clear(sig);
	element_clear(temp1);
	element_clear(temp2);
	element_clear(g);
	element_clear(h);
	}
	avg=sum/n;
	avg1=sum1/n;
	printf(" \n Execution time ms Bilinear-pairing Operation=%f \n",avg1*1000);
	//printf(" \n Execution time ms pairing g^y Operation=%f \n",avg*1000);

		
	///////////////////////////////////////////////////////
  */
	printf("\n *******EXIT**********\n");
	
    return SUCCESS;
}
