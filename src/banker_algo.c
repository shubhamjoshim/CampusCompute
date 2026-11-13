#include <stdio.h>

int main(){

    int avail[3]={10,5,7}; 

    int alloc[5][3]={
       //A B C   
        {0,1,0},
        {2,0,0},                  
        {3,0,2},
        {2,1,1},
        {0,0,2}
}; 

int max[5][3] = {
       //A B C   
        {7,5,3},
        {3,2,2},
        {9,0,2},               
        {4,2,2},
        {5,3,2}
};


/* example values for testing when deadlock won't work:
int avail[3] = {1, 0, 0}
int alloc[5][3] = {{0, 1, 0},{2, 0, 0},{3, 0, 2},{2, 1, 1},{0, 0, 2}}
int max[5][3] = {{7, 5, 3}, {3, 2, 2}, {9, 0, 2}, {4, 2, 2}, {5, 3, 3}}
*/


int need[5][3];
printf("resources needed: \n");
for(int i=0; i<5;i++){
    printf("process %d: ",i+1);
    for(int j=0;j<3;j++){
        need[i][j]=max[i][j] - alloc[i][j];  // writes down the need matrix
        printf("%d  ", need[i][j]);
    }
    printf("\n");
}

//the next step would be to now start by finding which of the remaining resources the available memory can fulfil in order
int check=0;


for(int i=0;i<5;i++){
    for(int j=0;j<3;j++){
    avail[j]=avail[j] - alloc[i][j];
    }
}

int finish[5] = {0};
int safe_seq[5];
int flag=0;

for(int rep=0;rep<5;rep++){
for(int i=0;i<5;i++){
    if(need[i][0]<=avail[0]&&need[i][1]<=avail[1]&&need[i][2]<=avail[2]&&finish[i]==0){
        avail[0]=avail[0] + alloc[i][0];
        avail[1]=avail[1] + alloc[i][1];
        avail[2]=avail[2] + alloc[i][2];
        check++;
        if(check>0&&flag==0){
            printf("PROCESS ORDER: \n");
            flag++;
        }
        safe_seq[check] = i;
        finish[i]=1;
        if(check<5){
            printf("P%d -> ",i+1);
        }
        else printf("P%d ",i+1);
    }
    }
}
 printf("\n");
if (check == 5) {
    printf("SAFE STATE");
   } 
   else {
    printf("UNSAFE STATE\n");
}
}



