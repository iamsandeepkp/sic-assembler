#include<stdio.h>
//#include<stdlib.h>
#include<string.h>
//#include<math.h>

struct optab{
		char op[10];
		char value[11];
		
};
typedef struct optab table;

int file_check(char operand[10])
{
			FILE *pf=fopen("symbol.txt", "r");
			char label1[10];
			char locctr[10];
			int line;
			while(fscanf(pf, "%s \t  %s\n", label1, locctr)!=EOF)
			{
				
				if(strcmp(label1,operand)==0)
				{
					fclose(pf);
					line=(int)strtol(locctr, NULL, 16);
			//		printf("%x\n", line);
					return line;
				}
			}
			fclose(pf);	
			return (0);
				
}

void file_entry2(int locctr, char label[11], char opcode[11], char operand[11], char objcode[11])
{
			
			FILE *pf=fopen("obj.txt", "a");
			fprintf(pf ,"%04x\t\t%s\t%s\t%s\t%s\n",locctr, label, opcode, operand, objcode); 
			
			fclose(pf);
			return ;	
}

int main()
{
	table record[30];
	int i=0, j, m, length;
	char op1[10], value1[11], prog_name[20], start_rec[10], end_rec[10];
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
	char label[11], opcode[11], operand[11], ch,obj[11], objcode[11], str1[10], buffer[71];
	int addr=0, addr1=0, count=0, locctr, len, nextloc ;
	char str2[10]; 
	objcode[0]='\0';
	while(fscanf(fp, "%[^\t \n]s ", label)!=EOF)
	{
		if(label[0]=='*')
		{
		
			fscanf(fp, "%[^\n]s", buffer);
			ch=fgetc(fp);
			continue;

		}
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

		if(strcmp(opcode, "START")==0)
		{
			strcpy(prog_name, label);
			strcpy(start_rec, "00");

		//	printf("%s", start_rec);
			addr=(int)strtol(operand, NULL, 16);
			locctr=addr;
			sprintf(str1, "%04X", locctr);
			strcat(start_rec, str1);
			FILE *pf=fopen("obj.txt", "w");
			fprintf(pf ,"%04x\t\t%s\t%s\t%s\t%s\n", locctr,label, opcode, operand, objcode); 
			fclose(pf);	

		}
		
	
		 else if(strcmp(label, "FIRST")==0)
		{
	
			for(j=0; j<i; j++)
			{		
				if(strcmp(opcode, record[j].op)==0)
				{
					strcpy(objcode, record[j].value);
					break;
				}
			}

//			printf("%x\n", locctr);		
			nextloc=file_check(operand);
			sprintf(obj, "%04x", nextloc);

			strcat(objcode, obj);

			FILE *pf=fopen("obj.txt", "a");
			fprintf(pf ,"%04x\t\t%s\t%s\t%s\t%s\n",locctr, label, opcode, operand, objcode); 
			
			fclose(pf);
			locctr=locctr+3;
			
			
		}
	
		else if(strcmp(opcode, "RESB")==0)
		{
		/*	FILE *pf=fopen("obj.txt", "a");
			fprintf(pf ,"%04x\t\t%s\t%s\t%s\t%s\n", locctr, label, opcode, operand, objcode); 
			
			fclose(pf);
			*/
			file_entry2(locctr, label, opcode, operand, objcode);
			addr1=atoi(operand);
			locctr+=addr1;
		}
		else if(strcmp(opcode, "END")==0 ||strcmp(label,"END")==0 )
		{
			length=locctr-addr;
			sprintf(str1, "%04x", length);
			strcpy(end_rec, "00");
			strcat(end_rec, str1);
		//	printf("%s", end_rec);
			FILE *pf=fopen("obj.txt", "a");
			fprintf(pf ,"\t\t%s\t%s\t%s\t%s\n",label, opcode, operand, objcode); 
			
			fclose(pf);
			printf("\n Object code successfully generated in  obj.txt file\n ");
			break;
		}	
		
		else if(strcmp(opcode,"RSUB")==0)
		{
			operand[0]='\0';	
		
			for(j=0; j<i; j++)
			{
											
				if(strcmp(opcode, record[j].op)==0)
				{	
					strcpy(objcode, record[j].value);
			//		printf("%s\n", objcode);
					strcat(objcode, "0000");
			//		printf("%s\n", objcode);
					break;
				}
			}
			file_entry2(locctr, label, opcode, operand, objcode);
			locctr+=3;				
		}
		
		else if(strcmp(opcode, "WORD")==0)
		{
			len=strlen(operand);
			if(len==1)
			{
				strcpy(objcode, "00000");
				strcat(objcode, operand);
			}
			else
			{
				addr=atoi(operand);
				sprintf(str2,"%04x", addr);
	//			printf("%s",str2);
				len=strlen(str2);
				if(len== 4)
				{
					strcpy(objcode, "00");
					strcat(objcode, str2);
					
				}
		
			}
			file_entry2(locctr, label, opcode, operand, objcode);
	
				locctr+=3;
			
		}

		else if(strcmp(opcode, "BYTE")==0)
		{ 
			int k=0, l=2, asci;
			str1[0]='\0';

			if(operand[0]=='C')
			{	
				len=strlen(operand)-3;
				while(k<len)
				{
					asci=(int)operand[l];
					sprintf(str2, "%x", asci);
					strcat(str1, str2);
		//			printf("%s", str1);
					k++;
					l++;

				}

				file_entry2(locctr, label, opcode, operand, str1);
				locctr+=len;
				
			}

			else if(operand[0]=='X')
			{
			//	file_entry(label, locctr);
				len=strlen(operand)-3;
				
				while(k<len)
				{
					str1[k]=operand[l];
					k++;
					l++;
				}
				str1[k]='\0';

				file_entry2(locctr, label, opcode, operand, str1);

				str1[0]='\0';
				m=len/2;
				if(len%2==0)
					locctr+=m;	
				else
					locctr+=m+1;
				
			}
		}

		else if(label[0]!='\0')
		{
			//		printf("%s", opcode);
					
			for(j=0; j<i; j++)
			{
				if(strcmp(opcode, record[j].op)==0)
				{
					strcpy(objcode, record[j].value);
				//	printf("%s\n", objcode);
					break;
				}
			}
			
			nextloc=file_check(operand);
			if(nextloc==0)
			{
				obj[0]='\0';
			}
			else
				sprintf(obj, "%04x", nextloc);
			//	printf("%s\n", obj);
			strcat(objcode, obj);
		//	printf("%s\n", objcode);

			file_entry2(locctr, label, opcode, operand, objcode);
			locctr+=3;
			
		}				
//		
		else if(label[0]=='\0')
		{
			for(j=0; j<i; j++)
			{
				if(strcmp(opcode, record[j].op)==0)
				{
					strcpy(objcode, record[j].value);
					break;
				}
			}

			if(strcmp(operand, "BUFFER,X")==0 )
			{
				operand[6]='\0';
				nextloc=file_check(operand);
				nextloc=nextloc+ 4096*8;
	//			printf("%04x", nextloc);
				operand[6]=',';
				operand[7]='X';
				operand[8]='\0';
				
			}

			else		
				nextloc=file_check(operand);
			
			sprintf(obj, "%04x", nextloc);
	//		printf("%s\n", obj);
			
			strcat(objcode, obj);
	//		printf("%s\n", objcode);

			file_entry2(locctr, label, opcode, operand, objcode);

			locctr+=3;
		}		
		if(objcode[0]!='\0')
		{
			strcat(buffer,objcode);
			strcat(buffer, "^");

		}
			obj[0]='\0';
			objcode[0]='\0';
			label[0]='\0';
			opcode[0]='\0';
			operand[0]='\0';
			str1[0]='\0';
			str2[0]='\0';
		
	}
	fclose(fp);
	locctr=0;
	printf("%s^ %s ^ %s ^ %s\n", "H", prog_name, start_rec, end_rec);

	label[0]='\0';
	buffer[0]='\0';
	len=0, length=0;
	FILE *fh=fopen("obj.txt", "r");
	while(fscanf(fh, "%s", str1)!=EOF)
	{ 
		ch=fgetc(fh);
		ch=fgetc(fh);

		fscanf(fh, "%[^\t]s", label);
		ch=fgetc(fh);
		
		count++;
		
	fscanf(fh ,"%s", opcode);
	ch=fgetc(fh);
 
	fscanf(fh,"%[^\t]s", operand);
	ch=fgetc(fh);
	fscanf(fh, "%[^\n]s", objcode);
	ch=fgetc(fp);	
	len=strlen(objcode);
	if(len!=0)
	{
		if(buffer[0]=='\0')
		{
			strcpy(str2, "00");
			strcat(str2, str1);

		}
		else
			strcat(buffer,"^");

		strcat(buffer, objcode);

	}
	
	length=length+len/2;
	if(length==30 || length >=28 && length <=30)
	{
		printf("%s^%s^%02X^%s\n","T",str2, length, buffer);
		buffer[0]='\0';
		length=0;
	}

	else if(strcmp(opcode, "RESB")==0 || strcmp(opcode,"RESW")==0)
	{
		if(buffer[0]!='\0')
		{
			printf("%s^%s^%02X^%s\n","T",str2, length, buffer);
		}
		buffer[0]='\0';
		length=0;
		
	}
	else if(strcmp(str1, "END")==0 )
	{
		printf("%s^%s^%02X^%s\n","T",str2, length, buffer);
		buffer[0]='\0';
		length=0;

	}

//	printf("%s%d %s%d %s%d %s%d  %s%d\n",str1,strlen(str1), label,strlen(label), opcode,strlen(opcode), operand,strlen(operand), objcode, strlen(objcode));

			objcode[0]='\0';
			label[0]='\0';
			opcode[0]='\0';
			operand[0]='\0';
			str1[0]='\0';

	}
				
	fclose(fh);
	printf(" %s ^ %s\n","E",  start_rec);

}
