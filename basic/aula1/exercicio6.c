#include <stdio.h>
#include <string.h>
int main(){
	char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	int c=0,c2=0;
	for(c=0;c<12;c++){
		for(c2=0;c2<3;c2++){
			printf("%c", *(months[c]+c2));
		}
		printf("\n");
	}
	string s1, s2;
	scanf("%s %s",&s1 &s2);

	}
//exercicio b
int no_months(char *mon1, char *mon2){
	int i=0,i1=0,cp=0,cs=0;
	boolean primeiro=false;
	for(i=0;i<12;i++){
		if(cp==3){
			cp==i;
			primeiro=true;
		}
		if(cs==3){
			cs==i;
		}

		for(i1=0;i1<3;i1++){
			if(!primeiro && (*(mon1+i1)==*(months[i]+i1))
				cp++;
			else if(*(mon2+i1)==*(months[i]+i1))
				cs++;
	}
	int r=cs-cp;
	printf("Nr de meses que faltam é:%d\n", r);
}