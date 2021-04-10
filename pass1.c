#include<stdio.h>
#include<string.h>

struct optab{
		char op[10];
		char value[11];
		
};
typedef struct optab table;


void file_entry(char label[11], int locctr)
{
			int ret_val;
			ret_val=file_check(label);
			if(ret_val==0)
			{
				return ;
			}
			FILE *pf=fopen("symbol.txt", "a");
			
			fprintf(pf, "%s \t \t %04x\n", label, locctr);
			fclose(pf);
			return ;	
}
	

int file_check(char label[11])
{
	char obj[11], locctr[8];
	FILE *fp=fopen("symbol.txt", "r");
	while(fscanf(fp,"%s \t%s\n", obj, locctr)!=EOF)
	{
		if(strcmp(obj, label)==0)
		{
			fclose(fp);
			return (0);
		}
	}
	fclose(fp);
	return (1);
}



int main()
{
	table record[30];
	int i=0, m, j;
	char op1[10], value1[11];
	FILE * pf=fopen("optable.txt", "r");
	while(fscanf(pf, "%s 	%s", op1, value1)!=EOF)
	{
		strcpy(record[i].op, op1);
		strcpy(record[i].value, value1);
//		printf("%s	%s\n", record[i].op, record[i].value);
		i++;
	}
	
	fclose(pf);

	char filename[30];
	printf("\n Enter filename:");
	scanf("%s", filename);	
	FILE * fp=fopen(filename, "r");
	if(fp==NULL)
	{
		printf("\n File does not exist\n ");
		return (0);
	}
	char label[11], opcode[11], operand[11], ch;
	int addr=0, count=0, locctr, len ;

	while(fscanf(fp, "%[^\t \n]s", label)!=EOF)
	{
		if(label[0]=='\0')
		{
			ch=fgetc(fp);
			while(ch==' '|| ch=='\t'|| ch=='\n')
			{
				ch=fgetc(fp);
			}
			fseek(fp, -1, 1);
			continue;
		}
		for(j=0; j<i; j++)
		{						
			if(strcmp(label, record[j].op)==0)
			{
				len=strlen(label);
				fseek(fp, -len, 1);
				label[0]='\0';
				break;
			}
			
		}

		ch=fgetc(fp);
		while(ch==' '|| ch=='\t')
		{
			ch=fgetc(fp);
		}
		fseek(fp, -1, 1);
		fscanf(fp,"%[^\t ]s", opcode);
					
		for(j=0; j<i; j++)
		{						
			if(strcmp(opcode, record[j].op)==0)
			{
				
				break;
			}
			
		}
		ch=fgetc(fp);
	
		while(ch==' '|| ch=='\t')
		{
			ch=fgetc(fp);
		}
		fseek(fp, -1, 1);
		fscanf(fp,"%[^\t \n]s", operand);
		ch=fgetc(fp);
		if(ch!='\n')
		{
			while(ch==' '|| ch=='\t')
			{
				ch=fgetc(fp);
			}
				
		}
//		printf("(%d)%s	(%d)%s	(%d)%s\n",strlen(label),label,strlen(opcode),opcode, strlen(operand),operand); 
/*		count++;
		if(count==4)
		{
			break;
		}*/
		if(strcmp(opcode, "START")==0)
		{
			addr=(int)strtol(operand, NULL, 16);
			locctr=addr;
		
		
			FILE *pf=fopen("symbol.txt", "w");
			
			fclose(pf);

		}
		
		else if(strcmp(label, "FIRST")==0)
		{
			
			file_entry(label, locctr);
			locctr+=3;
		}
		
		else if(strcmp(opcode, "RESB")==0)
		{
			
			file_entry(label, locctr);
			
			addr=atoi(operand);
			locctr+=addr;
			


		}
		else if(strcmp(opcode, "END")==0 || strcmp(label, "END")==0)
		{
			printf("\n Symbol table successfully generated in  symbol.txt file\n ");
			break;
		}	
		
		else if(strcmp(opcode, "BYTE")==0)
		{ 
			if(operand[0]=='C')

			{
				file_entry(label, locctr);
				
				len=strlen(operand)-3;
				locctr+=len;
			}

			else if(operand[0]=='X')
			{
				file_entry(label, locctr);
				len=strlen(operand)-3;
				m=len/2;
				if(len%2==0)
					locctr+=m;	
				else
					locctr+=m+1;
			}
		}

		else if(label[0]!='\0')
		{
		

			file_entry(label, locctr);
			locctr+=3;
			
		}				
//		
		else if(label[0]=='\0')
		{
		
			locctr+=3;
		
		}				
		 
		
		label[0]='\0';	
		opcode[0]='\0';	
		operand[0]='\0';	


	}
	fclose(fp);
}
