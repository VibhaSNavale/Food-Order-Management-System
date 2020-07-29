#include<iostream.h>
#include<fstream.h>
#include<stdio.h>
#include<iomanip.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>
#include<dir.h>
#include<dos.h>
#include<bios.h>
#include<graphics.h>

void screen();
void drawdoublelinebox(int,int,int,int);
void drawsinglelinebox(int,int,int,int);
void normal();


#define datafile "food.txt"
#define recsize 78
#define max 8                           
#define bucketsize 3
int hash(char a[]);
fstream file;
char buffer[100];

class Order
{
public:
  char order_no[12],restaurant_code[10],cust_name[12],phone[12],order_time[7],delivery_time[7],order[60], amount_paid[5];
public:
	void initial();
	void read();
	void pack();
	void unpack();
	int  retrieve(int addr,char k[],int i);
	void datadisp();
	int  remove(int addr,char k[]);
	void modify(int addr ,char k[]);
	void bill(int addr,char k[]);
};


void opener(fstream &file,char *fn,int mode)
{
	file.open(fn,mode);
	if(!file)
	{
		cout<<"UNABLE TO OPEN THE FILE\n"<<fn;
		getch();
		exit(1);
	}
}

void Order::initial()
{
	int i,j;
	file.open(datafile,ios::in);
	if(!file)
	{
		file.open(datafile,ios::out);
		if(!file)
		{
			cout<<"UNABLE TO OPEN THE FILE\n";
			getch();
			exit(1);
		}
		for(i=0;i<max;i++)
		{
			file.seekp(i*recsize*3,ios::beg);
			for(j=0;j<(recsize*3)-2;j++)
			   file<<"#";
			file<<endl;
		}
		cout<<"EMPTY FILE CREATED\n";
	}
	file.close();
	return;
}
void Order::read()
{
	int addr,len,len1,len2,len3,len4,len5;
	char ord_no[12],drestaurant_code[10],dorder[8];

   LABEL:
	cout<<"\n\t\t\tENTER THE ORDER ID: ";
	gets(ord_no);
	len=strlen(ord_no);
	addr=hash(ord_no);
	if(len!=8)
	{
		cout<<"\n\t  ENTER THE VALID ORDER ID(8 CHARACTERS): ";
		goto LABEL;
	}

	if(retrieve(addr,ord_no,0)>0)
	{
		cout<<"!!*** ORDER ID IS ALREADY PRESENT, WE CANNOT ADD IT TO THE HASH FILE ***!!\n";
		goto LABEL;
	}
	file.close();
	strcpy(order_no,ord_no);

      DESC:
	cout<<"\n\t\t\tENTER RESTAURANT CODE: ";
	gets(drestaurant_code);
	len1=strlen(drestaurant_code);
	if(len1!=8||drestaurant_code[0]>63||drestaurant_code[1]<63||drestaurant_code[2]<63||drestaurant_code[3]<63 ||drestaurant_code[4]<63 ||drestaurant_code[5]<63||drestaurant_code[6]<63||drestaurant_code[7]<63||drestaurant_code[8]>63)
	{
		cout<<"\n\t  ENTER THE VALID RESTAURANT CODE(8 CHARACTERS): ";
		goto DESC;
	}
	file.close();
	strcpy(restaurant_code,drestaurant_code);

     NAME:
	cout<<"\n\t\t\tENTER THE CUSTOMER NAME: ";
	gets(cust_name);
	int i=0;
		while(cust_name[i]!='\0')
		{
			 if(cust_name[i]<63||i>9)
			{
				  cout<<"\n\t  ENTER THE VALID CUSTOMER NAME(LESS THAN 10 CHARACTERS): ";
				 goto NAME;
			}
			i++;
		}

     PH:
	i=0;
	cout<<"\n\t\t\tENTER THE PHONE: ";
	gets(phone);
	len3=strlen(phone);
	while(phone[i]!='\0')
	{
	    if(phone[i]>63 ||i>9 ||len3!=10)
	    {
	      cout<<"\n\t  ENTER VALID PHONE NUMBER: ";
	      goto PH;
	    }
	      i++;
	}
  /* if(i!=11)
{
     cout<<"\n\t  ENTER VALID PHONE NUMBER: ";
	      goto PH;

}*/
	int ch,sum=0;
	char b[60];
	strcpy(order,"");
	do{
	
	cout<<"\n\t\t\tMENU: \n\t\t\t1.NOODLES\t\t\tRs.45 \n\t\t\t2.VEG THALI\t\t\tRs.110 \n\t\t\t3.MANCHURIAN\t\t\tRs.70 \n\t\t\t4.BIRYANI\t\t\tRs.80 \n\t\t\t5.DOSA\t\t\t\tRs.35 \n\t\t\t6.PANEER ROLL\t\t\tRs.55 \n\t\t\t7.PLACE ORDER";
	cout<<"\n\t\t\tENTER YOUR CHOICE: ";
	cin>>ch;
	switch(ch) {
		case 1: strcat(order,"Noodles");
			sum=sum+45;
			break;
		case 2: strcat(order,"Veg thali");
			sum=sum+110;
			break;
		case 3: strcat(order,"Manchurian");
			sum=sum+70;
			break;
		case 4: strcat(order,"Biryani");
			sum=sum+80;
			break;
		case 5: strcat(order,"Dosa");
			sum=sum+35;
			break;
		case 6: strcat(order,"Paneer roll");
			sum=sum+55;
			break;
		default:sprintf(amount_paid, "%d", sum);
			break;
		}
		if(ch<7){
			strcat(order,",");
		}
		else
			order[strlen(order)-1]=' ';
	}while(ch<7);


    ORD_TIME:
	cout<<"\n\t\t\tENTER THE ORDER TIME IN 24 HRS FORMAT: ";
	gets(order_time);
	len4=strlen(order_time);
	 i=0;
	while(order_time[i]!='\0')
	{
	    if(order_time[i]>63||i>4||order_time[2]!=58 ||len4!=5)
	    {
	      cout<<"\n\t  ENTER THE VALID ORDER TIME IN 24 HRS FORMAT: ";
	      goto ORD_TIME;
	    }
	      i++;
	}


     DEL_TIME:
	cout<<"\n\t\t\tENTER THE DELIVERY TIME IN 24 HRS FORMAT: ";
	gets(delivery_time);
	len5=strlen(delivery_time);
	   i=0;
	while(delivery_time[i]!='\0')
	{
	    if(delivery_time[i]>63||i>4||delivery_time[2]!=58 ||len5!=5)
	    {
	      cout<<"\n\t  ENTER THE VALID DELIVERY TIME IN 24 HRS FORMAT: ";
	      goto DEL_TIME;
	    }
	      i++;
	}

	pack();
	return;
}
void Order::pack()
{
	strcpy(buffer,order_no);		strcat(buffer,"|");
	strcat(buffer,restaurant_code);		strcat(buffer,"|");
	strcat(buffer,cust_name);		strcat(buffer,"|");
	strcat(buffer,phone);			strcat(buffer,"|");
	strcat(buffer,order_time);		strcat(buffer,"|");
	strcat(buffer,delivery_time);		strcat(buffer,"|");
	strcat(buffer,order);			strcat(buffer,"|");
	strcat(buffer,amount_paid);        	strcat(buffer,"|");     
}

int hash(char order_no[])
{
int i=0,sum=0,c,len;
len=strlen(order_no);
if(len%2==1)
    len++;
for(i=0;i<len;i+=2)
    sum=(sum+((order_no[i]+order_no[i+1])-96));
c=sum%max;
cout<<"\n\t\t    HASH KEY GENERATED IS: "<<c<<endl;
return c;
}

void store(int addr)
{
char dummy[10];
int flag=0,i;
file.open(datafile,ios::in|ios::out);
file.seekg(addr*recsize*bucketsize,ios::beg);
file.getline(dummy,5,'\n');
if(strcmp(dummy,"####")==0 ||strcmp(dummy,"$$$$")==0)
{
	file.seekp(addr*recsize*bucketsize,ios::beg);
	file<<buffer;
	clrscr();
	textcolor(YELLOW);
	textbackground(BLACK);
	cout<<"\n\n\n\n";
	for(int q=0;q<80;q++) cout<<'~';
	for(int w=0;w<80;w++) cout<<'*';
	cout<<"\n\n\t\t   FOOD ORDER RECORD INSERTED SUCCESSFULLY....!!\n\n";
	for(int e=0;e<80;e++) cout<<'*';
	cout<<"\n";
	for(int r=0;r<80;r++) cout<<'~';
	getch();
	flag=1;
}
file.seekg(addr*recsize*bucketsize+recsize,ios::beg);
file.getline(dummy,5,'\n');
if((strcmp(dummy,"####")==0||strcmp(dummy,"$$$$")==0) && flag!=1)
{
	file.seekp(addr*recsize*bucketsize+recsize,ios::beg);
	file<<buffer;clrscr();
	textcolor(YELLOW);
	textbackground(BLACK);
	cout<<"\n\n\n\n";
	for(int q=0;q<80;q++) cout<<'~';
	for(int w=0;w<80;w++) cout<<'*';
	cout<<"\n\n\t\t   FOOD ORDER RECORD INSERTED SUCCESSFULLY....!!\n\n";
	for(int e=0;e<80;e++) cout<<'*';
	cout<<"\n";
	for(int r=0;r<80;r++) cout<<'~';
	getch();
	flag=1;
}
file.seekg(addr*recsize*bucketsize+(2*recsize),ios::beg);
file.getline(dummy,5,'\n');
if((strcmp(dummy,"####")==0||strcmp(dummy,"$$$$")==0)&&flag!=1)
{
	file.seekp(addr*recsize*bucketsize+(2*recsize),ios::beg);
	file<<buffer;
	clrscr();
	textcolor(YELLOW);
	textbackground(BLACK);
	cout<<"\n\n\n\n";
	for(int q=0;q<80;q++) cout<<'~';
	for(int w=0;w<80;w++) cout<<'*';
	cout<<"\n\n\t\t   FOOD ORDER RECORD INSERTED SUCCESSFULLY....!!\n\n";
	for(int e=0;e<80;e++) cout<<'*';
	cout<<"\n";
	for(int r=0;r<80;r++) cout<<'~';
	getch();
	flag=1;
}
if(flag!=1)
{
for(i=addr+1;i!=addr;i++)
{
	if(i%max==0)
		i=0;
	file.seekg(i*recsize*bucketsize,ios::beg);
	file.getline(dummy,5,'\n');
	if(strcmp(dummy,"####")==0||strcmp(dummy,"$$$$")==0)
	{
		clrscr();
		textcolor(YELLOW);
		textbackground(BLACK);		
		cout<<"\n\n\n\n";
		for(int t=0;t<80;t++) cout<<'~';
		for(int y=0;y<80;y++) cout<<'*';
		cout<<"\n\n\t\t\tCOLLISION HAS OCCURED....!!\n\n";
		cout<<"\t      HOME ADDRESS IS: "<<addr<<" & ACTUAL ADDRESS IS: "<<i<<"\n";
		file.seekp(i*recsize*bucketsize,ios::beg);
		file<<buffer;
		textcolor(YELLOW);
		textbackground(BLACK);
		cout<<"\n\n\t\t   FOOD ORDER RECORD INSERTED SUCCESSFULLY....!!\n\n";
		for(int u=0;u<80;u++) cout<<'*';
		cout<<"\n";
		for(int f=0;f<80;f++) cout<<'~';
		getch();
		flag=1;
		break;
	}
	file.seekg(i*recsize*bucketsize+recsize,ios::beg);
	file.getline(dummy,5,'\n');
	if(strcmp(dummy,"####")==0||strcmp(dummy,"$$$$")==0)
	{
		clrscr();
		textcolor(YELLOW);
		textbackground(BLACK);	
		cout<<"\n\n\n\n";
		for(int t=0;t<80;t++) cout<<'~';
		for(int y=0;y<80;y++) cout<<'*';
		cout<<"\n\n\t\t\tCOLLISION HAS OCCURED....!!\n\n";
		cout<<"\t      HOME ADDRESS IS: "<<addr<<" & ACTUAL ADDRESS IS: "<<i<<"\n";
		file.seekp(i*recsize*bucketsize+recsize,ios::beg);
		file<<buffer;
		textcolor(YELLOW);
		textbackground(BLACK);
		cout<<"\n\n\t\t   FOOD ORDER RECORD INSERTED SUCCESSFULLY....!!\n\n";
		for(int u=0;u<80;u++) cout<<'*';
		cout<<"\n";
		for(int f=0;f<80;f++) cout<<'~';
		getch();
		flag=1;
		break;
	}
	file.seekg(i*recsize*bucketsize+(2*recsize),ios::beg);
	file.getline(dummy,5,'\n');
       if(strcmp(dummy,"####")==0||strcmp(dummy,"$$$$")==0)
	{
		clrscr();
		textcolor(YELLOW);
		textbackground(BLACK);	
		cout<<"\n\n\n\n";
		for(int t=0;t<80;t++) cout<<'~';
		for(int y=0;y<80;y++) cout<<'*';
		cout<<"\n\n\t\t\tCOLLISION HAS OCCURED....!!\n\n";
		cout<<"\t      HOME ADDRESS IS: "<<addr<<" & ACTUAL ADDRESS IS: "<<i<<"\n";
		file.seekp(i*recsize*bucketsize+(2*recsize),ios::beg);
		file<<buffer;
		textcolor(YELLOW);
		textbackground(BLACK);
		cout<<"\n\n\t\t   FOOD ORDER RECORD INSERTED SUCCESSFULLY....!!\n\n";
		for(int u=0;u<80;u++) cout<<'*';
		cout<<"\n";
		for(int f=0;f<80;f++) cout<<'~';
		getch();
		flag=1;
		break;
	}
}
}
if(i==addr && (flag!=1))
{
	clrscr();
	textcolor(YELLOW);
	textbackground(BLACK);
	cout<<"\n\n\n\n";
	for(int o=0;o<80;o++) cout<<'~';
	for(int a=0;a<80;a++) cout<<'*';
	cout<<setw(23)<<" "<<"\n\n\t*** HASH FILE IS FULL, FOOD ORDER RECORD CANT BE INSERTED ***\n\n";
	for(int s=0;s<80;s++) cout<<'*';
	for(int d=0;d<80;d++) cout<<'~';
	getch();
}
getch();
file.close();
return;
}

void Order::unpack()
{
	char dummy[100];
	file.getline(restaurant_code,10,'|');
	file.getline(cust_name,12,'|');
	file.getline(phone,12,'|');
	file.getline(order_time,7,'|');
	file.getline(delivery_time,7,'|');
	file.getline(order,60,'|');
	file.getline(amount_paid,5,'|');
	file.getline(dummy,100,'#');
}

int Order::retrieve(int addr,char k[],int l)  //search
{
int found=0,i;
char dummy[10];
i=addr;
file.open(datafile,ios::in);
cout<<setiosflags(ios::left);
//if(l==1)
do
{
	file.seekg(i*recsize*bucketsize,ios::beg);
	file.getline(dummy,5,'\n');
	if(strcmp(dummy,"####")==0)
		break;
    	file.seekg(i*recsize*bucketsize,ios::beg);
	file.getline(order_no,10,'|');
	if(strcmp(order_no,k)==0)
	{
		found=1;
		if(l==1)
		{
		unpack();
		clrscr();
		textcolor(15);
	      textbackground(YELLOW);		
	   cout<<endl;   
	   cout<<setw(18)<<" "<<"THE SEARCHED FOOD ORDER RECORD IS DISPLAYED"<<endl<<endl;
	   for(int f=0;f<80;f++)   cout<<'*';   cout<<endl;
	   cout<<setw(20)<<" "<<" ORDER ID               :  "<<setw(10)<<order_no<<endl<<endl;
	   cout<<setw(20)<<" "<<" RESTAURANT CODE        :  "<<setw(9)<<restaurant_code<<endl<<endl;
	   cout<<setw(20)<<" "<<" CUSTOMER NAME          :  "<<setw(10)<<cust_name<<endl<<endl;
	   cout<<setw(20)<<" "<<" PHONE NUMBER           :  "<<setw(10)<<phone<<endl<<endl;
	   cout<<setw(20)<<" "<<" YOUR ORDER             :  "<<setw(6)<<order<<endl<<endl;
	   cout<<setw(20)<<" "<<" ORDER TIME             :  "<<setw(5)<<order_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" DELIVERY TIME          :  "<<setw(5)<<delivery_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" AMOUNT PAID            :  "<<setw(5)<<amount_paid<<endl<<endl;
	   for(int g=0;g<80;g++) cout<<'*';
	   cout<<endl<<setw(25)<<" ";
	  			
	   cout<<"FOOD ORDER RECORD FOUND...!!"<<endl;		
		}
		break;
	}
	file.seekg(i*recsize*bucketsize+recsize,ios::beg);
	file.getline(order_no,10,'|');
	if(strcmp(order_no,"#########")==0)
	      break;
	if(strcmp(order_no,k)==0)
	{
		found=1;
		if(l==1)
		{
		unpack();
		clrscr();
		textcolor(15);
	      textbackground(YELLOW);	
	   cout<<endl;   
	   cout<<setw(18)<<" "<<"THE SEARCHED FOOD ORDER RECORD IS DISPLAYED"<<endl<<endl;
	   for(int f=0;f<80;f++)   cout<<'*';   cout<<endl;
	   cout<<setw(20)<<" "<<" ORDER ID               :  "<<setw(10)<<order_no<<endl<<endl;
	   cout<<setw(20)<<" "<<" RESTAURANT CODE        :  "<<setw(9)<<restaurant_code<<endl<<endl;
	   cout<<setw(20)<<" "<<" CUSTOMER NAME          :  "<<setw(10)<<cust_name<<endl<<endl;
	   cout<<setw(20)<<" "<<" PHONE NUMBER           :  "<<setw(10)<<phone<<endl<<endl;
	   cout<<setw(20)<<" "<<" YOUR ORDER             :  "<<setw(6)<<order<<endl<<endl;
	   cout<<setw(20)<<" "<<" ORDER TIME             :  "<<setw(5)<<order_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" DELIVERY TIME          :  "<<setw(5)<<delivery_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" AMOUNT PAID            :  "<<setw(5)<<amount_paid<<endl<<endl;
	   for(int g=0;g<80;g++) cout<<'*';
	   cout<<endl<<setw(25)<<" ";
	  			
	cout<<"FOOD ORDER RECORD FOUND...!!"<<endl;
		}		
		break;
	}

	file.seekg(i*recsize*bucketsize+(2*recsize),ios::beg);
	file.getline(order_no,10,'|');
	if(strcmp(order_no,"#########")==0)
	      break;
	if(strcmp(order_no,k)==0)
	{
		found=1;
		if(l==1)
		{
		unpack();
		clrscr();
		textcolor(15);
	      textbackground(YELLOW);	
	   cout<<endl;   
	   cout<<setw(18)<<" "<<"THE SEARCHED FOOD ORDER RECORD IS DISPLAYED"<<endl<<endl;
	   for(int f=0;f<80;f++)   cout<<'*';   cout<<endl;
	   cout<<setw(20)<<" "<<" ORDER ID               :  "<<setw(10)<<order_no<<endl<<endl;
	   cout<<setw(20)<<" "<<" RESTAURANT CODE        :  "<<setw(9)<<restaurant_code<<endl<<endl;
	   cout<<setw(20)<<" "<<" CUSTOMER NAME          :  "<<setw(10)<<cust_name<<endl<<endl;
	   cout<<setw(20)<<" "<<" PHONE NUMBER           :  "<<setw(10)<<phone<<endl<<endl;
	   cout<<setw(20)<<" "<<" YOUR ORDER             :  "<<setw(6)<<order<<endl<<endl;
	   cout<<setw(20)<<" "<<" ORDER TIME             :  "<<setw(5)<<order_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" DELIVERY TIME          :  "<<setw(5)<<delivery_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" AMOUNT PAID            :  "<<setw(5)<<amount_paid<<endl<<endl;
	   for(int g=0;g<80;g++) cout<<'*';
	   cout<<endl<<setw(25)<<" ";
	  			
	cout<<"FOOD ORDER RECORD FOUND...!!"<<endl;
		}		
		break;
	}
	else
	{
		i++;
		if(i%max==0)
			i=0;
	}
}while(i!=addr);
if(found==0 && l==1)
{
		textcolor(15);
	      textbackground(YELLOW);
	cout<<setw(23)<<" "<<"\n\n\t!!*** GIVEN FOOD ORDER RECORD DOESNOT EXISTS IN HASH FILE ***!!\n\n";
	for(int h=0;h<80;h++) cout<<'~';
	for(int j=0;j<80;j++) cout<<'~';
	getch();
}
	/*if(l==0)
	    cout<<found<<endl;*/

return found ;
}

void Order::datadisp()
{
	file.open(datafile,ios::in);
	int i=0;
	char dummy[10];
	cout<<setiosflags(ios::left);
	while(i<max)
	{
	 if(file.eof())
		break;
	file.seekg(i*recsize*bucketsize,ios::beg);
	file.getline(dummy,5,'\n');
	if(strcmp(dummy,"####")!=0 && strcmp(dummy,"$$$$")!=0)
	{
		file.seekg(i*recsize*bucketsize,ios::beg);
		file.getline(order_no,10,'|');
		unpack();
		clrscr();
	      //drawdoublelinebox(17,7,60,30);
	      textcolor(15);
	      textbackground(RED);	
	   cout<<endl<<endl<<endl;
	   cout<<setw(25)<<" "<<"DISPLAYING FOOD ORDER RECORDS"<<endl<<endl;
	   for(int i=0;i<80;i++)   cout<<'*';   cout<<endl;
	   cout<<setw(20)<<" "<<" ORDER ID               :  "<<setw(10)<<order_no<<endl<<endl;
	   cout<<setw(20)<<" "<<" RESTAURANT CODE        :  "<<setw(9)<<restaurant_code<<endl<<endl;
	   cout<<setw(20)<<" "<<" CUSTOMER NAME          :  "<<setw(10)<<cust_name<<endl<<endl;
	   cout<<setw(20)<<" "<<" PHONE NUMBER           :  "<<setw(10)<<phone<<endl<<endl;
	   cout<<setw(20)<<" "<<" YOUR ORDER             :  "<<setw(6)<<order<<endl<<endl;
	   cout<<setw(20)<<" "<<" ORDER TIME             :  "<<setw(5)<<order_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" DELIVERY TIME          :  "<<setw(5)<<delivery_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" AMOUNT PAID            :  "<<setw(5)<<amount_paid<<endl<<endl;
	   for(int k=0;k<80;k++) cout<<'*';
	   cout<<endl<<endl<<setw(20)<<" ";
	   getch();	

	}
	file.seekg(i*recsize*bucketsize+recsize,ios::beg);
	file.getline(dummy,5,'\n');
	if(strcmp(dummy,"####")!=0 && strcmp(dummy,"$$$$")!=0)
	{
		 file.seekg(i*recsize*bucketsize+recsize,ios::beg);
		file.getline(order_no,10,'|');
		unpack();
	     clrscr();
		textcolor(15);
	      textbackground(RED);		
	   cout<<endl<<endl<<endl;
	   cout<<setw(25)<<" "<<"DISPLAYING FOOD ORDER RECORDS"<<endl<<endl;
	   for(int i=0;i<80;i++)   cout<<'*';   cout<<endl;
	   cout<<setw(20)<<" "<<" ORDER ID               :  "<<setw(10)<<order_no<<endl<<endl;
	   cout<<setw(20)<<" "<<" RESTAURANT CODE        :  "<<setw(9)<<restaurant_code<<endl<<endl;
	   cout<<setw(20)<<" "<<" CUSTOMER NAME          :  "<<setw(10)<<cust_name<<endl<<endl;
	   cout<<setw(20)<<" "<<" PHONE NUMBER           :  "<<setw(10)<<phone<<endl<<endl;
	   cout<<setw(20)<<" "<<" YOUR ORDER             :  "<<setw(6)<<order<<endl<<endl;
	   cout<<setw(20)<<" "<<" ORDER TIME             :  "<<setw(5)<<order_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" DELIVERY TIME          :  "<<setw(5)<<delivery_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" AMOUNT PAID            :  "<<setw(5)<<amount_paid<<endl<<endl;
	   for(int k=0;k<80;k++) cout<<'*';
	   cout<<endl<<endl<<setw(20)<<" ";
	   getch();	


	}
	file.seekg(i*recsize*bucketsize+(2*recsize),ios::beg);
	file.getline(dummy,5,'\n');
	if(strcmp(dummy,"####")!=0 && strcmp(dummy,"$$$$")!=0)
	{
		file.seekg(i*recsize*bucketsize+(2*recsize),ios::beg);
		file.getline(order_no,10,'|');
		unpack();
		clrscr();
		textcolor(15);
	      textbackground(RED);		
	   cout<<endl<<endl<<endl;
	   cout<<setw(25)<<" "<<"DISPLAYING FOOD ORDER RECORDS"<<endl<<endl;
	   for(int i=0;i<80;i++)   cout<<'*';   cout<<endl;
	   cout<<setw(20)<<" "<<" ORDER ID               :  "<<setw(10)<<order_no<<endl<<endl;
	   cout<<setw(20)<<" "<<" RESTAURANT CODE        :  "<<setw(9)<<restaurant_code<<endl<<endl;
	   cout<<setw(20)<<" "<<" CUSTOMER NAME          :  "<<setw(10)<<cust_name<<endl<<endl;
	   cout<<setw(20)<<" "<<" PHONE NUMBER           :  "<<setw(10)<<phone<<endl<<endl;
	   cout<<setw(20)<<" "<<" YOUR ORDER             :  "<<setw(6)<<order<<endl<<endl;
	   cout<<setw(20)<<" "<<" ORDER TIME             :  "<<setw(5)<<order_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" DELIVERY TIME          :  "<<setw(5)<<delivery_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" AMOUNT PAID            :  "<<setw(5)<<amount_paid<<endl<<endl;
	   for(int k=0;k<80;k++) cout<<'*';
	   cout<<endl<<setw(20)<<" ";
	   getch();	
	}
	i++;
	}
	clrscr();
	textcolor(15);
	      textbackground(RED);
	cout<<"\n\n\n\n\n\n\n\n\n\t\t=============================================\n"<<endl;	
	cout<<"\t\t     !!*** NO RECORDS TO DISPLAY ***!!"<<endl;
	cout<<"\n\t\t=============================================\n"<<endl;	
	getch();
	file.close();
}



int Order::remove(int addr,char k[])
{
int found=0,i;
char dummy[10];
i=addr;
int s;

file.open(datafile,ios::in|ios::out);
cout<<setiosflags(ios::left);
do
{
	file.seekg(i*recsize*bucketsize,ios::beg);
	file.getline(dummy,5,'\n');
	if(strcmp(dummy,"####")==0)
		break;
        file.seekg(i*recsize*bucketsize,ios::beg);
	file.getline(order_no,10,'|');
	if(strcmp(order_no,"#########")==0)
	      break;
	if(strcmp(order_no,k)==0)
	{
		found=1;
		unpack();
		clrscr();
		textcolor(15);
	      textbackground(1);		
	   cout<<endl<<endl;
	   cout<<setw(26)<<" "<<"FOOD ORDER RECORD DISPLAYED"<<endl;
	   for(int l=0;l<80;l++)   cout<<'*';   cout<<endl;
	   cout<<setw(20)<<" "<<" ORDER ID               :  "<<setw(10)<<order_no<<endl<<endl;
	   cout<<setw(20)<<" "<<" RESTAURANT CODE        :  "<<setw(9)<<restaurant_code<<endl<<endl;
	   cout<<setw(20)<<" "<<" CUSTOMER NAME          :  "<<setw(10)<<cust_name<<endl<<endl;
	   cout<<setw(20)<<" "<<" PHONE NUMBER           :  "<<setw(10)<<phone<<endl<<endl;
	   cout<<setw(20)<<" "<<" YOUR ORDER             :  "<<setw(6)<<order<<endl<<endl;
	   cout<<setw(20)<<" "<<" ORDER TIME             :  "<<setw(5)<<order_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" DELIVERY TIME          :  "<<setw(5)<<delivery_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" AMOUNT PAID            :  "<<setw(5)<<amount_paid<<endl<<endl;
	   for(int z=0;z<80;z++) cout<<'*';
	   cout<<endl<<setw(20)<<" ";
	   	
		textcolor(15);
	      textbackground(1);	
		cout<<"FOOD ORDER RECORD TO BE DELETED IS FOUND"<<endl;
		file.seekp(i*recsize*bucketsize,ios::beg);
	      for(s=0;s<recsize;s++)
		   file.put('$');
		cout<<setw(25)<<" "<<"  RECORD IS DELETED... !!"<<endl;
		getch();
		file.close();
		break;
	}
	file.seekg(i*recsize*bucketsize+recsize,ios::beg);
	file.getline(order_no,10,'|');
		if(strcmp(order_no,"#########")==0)
		 break;
	if(strcmp(order_no,k)==0)
	{
		found=1;
		unpack();
		clrscr();
		textcolor(15);
	      textbackground(1);		
	   cout<<endl<<endl;   
	   cout<<setw(26)<<" "<<"FOOD ORDER RECORD DISPLAYED"<<endl;
	   for(int l=0;l<80;l++)   cout<<'*';   cout<<endl;
	   cout<<setw(20)<<" "<<" ORDER ID               :  "<<setw(10)<<order_no<<endl<<endl;
	   cout<<setw(20)<<" "<<" RESTAURANT CODE        :  "<<setw(9)<<restaurant_code<<endl<<endl;
	   cout<<setw(20)<<" "<<" CUSTOMER NAME          :  "<<setw(10)<<cust_name<<endl<<endl;
	   cout<<setw(20)<<" "<<" PHONE NUMBER           :  "<<setw(10)<<phone<<endl<<endl;
	   cout<<setw(20)<<" "<<" YOUR ORDER             :  "<<setw(6)<<order<<endl<<endl;
	   cout<<setw(20)<<" "<<" ORDER TIME             :  "<<setw(5)<<order_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" DELIVERY TIME          :  "<<setw(5)<<delivery_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" AMOUNT PAID            :  "<<setw(5)<<amount_paid<<endl<<endl;
	   for(int z=0;z<80;z++) cout<<'*';
	   cout<<endl<<setw(20)<<" ";
	  
		textcolor(15);
	      textbackground(1);					
	cout<<"FOOD ORDER RECORD TO BE DELETED IS FOUND"<<endl;
		file.seekp(i*recsize*bucketsize+recsize,ios::beg);
	      for(s=0;s<recsize;s++)
		   file.put('$');
		cout<<setw(25)<<" "<<"  RECORD IS DELETED... !!"<<endl;
		getch();
		break;
	}

	file.seekg(i*recsize*bucketsize+(2*recsize),ios::beg);
	file.getline(order_no,10,'|');
	if(strcmp(order_no,k)==0)
	{
		found=1;
		unpack();
		clrscr();
		textcolor(15);
	      textbackground(1);			
	   cout<<endl<<endl;
	   cout<<setw(26)<<" "<<"FOOD ORDER RECORD DISPLAYED"<<endl;
	   for(int l=0;l<80;l++)   cout<<'*';   cout<<endl;
	   cout<<setw(20)<<" "<<" ORDER ID               :  "<<setw(10)<<order_no<<endl<<endl;
	   cout<<setw(20)<<" "<<" RESTAURANT CODE        :  "<<setw(9)<<restaurant_code<<endl<<endl;
	   cout<<setw(20)<<" "<<" CUSTOMER NAME          :  "<<setw(10)<<cust_name<<endl<<endl;
	   cout<<setw(20)<<" "<<" PHONE NUMBER           :  "<<setw(10)<<phone<<endl<<endl;
	   cout<<setw(20)<<" "<<" YOUR ORDER             :  "<<setw(6)<<order<<endl<<endl;
	   cout<<setw(20)<<" "<<" ORDER TIME             :  "<<setw(5)<<order_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" DELIVERY TIME          :  "<<setw(5)<<delivery_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" AMOUNT PAID            :  "<<setw(5)<<amount_paid<<endl<<endl;
	   for(int z=0;z<80;z++) cout<<'*';
	   cout<<endl<<setw(20)<<" ";		

		textcolor(15);
	      textbackground(1);		
		cout<<"FOOD ORDER RECORD TO BE DELETED IS FOUND"<<endl;
		file.seekp(i*recsize*bucketsize+(2*recsize),ios::beg);
	       for(s=0;s<recsize-2;s++)
		   file.put('$');
		   file.put('\n');
		cout<<setw(25)<<" "<<"  RECORD IS DELETED... !!"<<endl;
		getch();
		break;
	}
	else
	{
		i++;
		if(i%max==0)
			i=0;
	}
}while(i!=addr);
if(found==0)
{
	textcolor(15);
	      textbackground(1);
	cout<<setw(23)<<" "<<"\n\n\t!!*** GIVEN FOOD ORDER RECORD DOESNOT EXISTS IN HASH FILE ***!!\n\n";
	for(int k=0;k<80;k++) cout<<'^';
	for(int j=0;j<80;j++) cout<<'^';
	getch();
}
return found;
}

void Order::modify(int addr, char k[])
{
int found=0,i;
char dummy[10];
i=addr;
int s;
int x;
char confirm;

file.open(datafile,ios::in|ios::out);
cout<<setiosflags(ios::left);
do
{
	file.seekg(i*recsize*bucketsize,ios::beg);
	file.getline(dummy,5,'\n');
	if(strcmp(dummy,"####")==0)
		break;
       file.seekg(i*recsize*bucketsize,ios::beg);
	file.getline(order_no,10,'|');
	if(strcmp(order_no,k)==0)
	{
		found=1;
		unpack();
		clrscr();
		textcolor(YELLOW);
	      textbackground(0);
	   cout<<endl;
	   cout<<setw(18)<<" "<<"FOOD ORDER RECORD TO BE MODIFIED IS DISPLAYED"<<endl<<endl;
	   for(int c=0;c<80;c++)   cout<<'*';   cout<<endl;
	   cout<<setw(20)<<" "<<" ORDER ID               :  "<<setw(10)<<order_no<<endl<<endl;
	   cout<<setw(20)<<" "<<" RESTAURANT CODE        :  "<<setw(9)<<restaurant_code<<endl<<endl;
	   cout<<setw(20)<<" "<<" CUSTOMER NAME          :  "<<setw(10)<<cust_name<<endl<<endl;
	   cout<<setw(20)<<" "<<" PHONE NUMBER           :  "<<setw(10)<<phone<<endl<<endl;
	   cout<<setw(20)<<" "<<" YOUR ORDER             :  "<<setw(6)<<order<<endl<<endl;
	   cout<<setw(20)<<" "<<" ORDER TIME             :  "<<setw(5)<<order_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" DELIVERY TIME          :  "<<setw(5)<<delivery_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" AMOUNT PAID            :  "<<setw(5)<<amount_paid<<endl<<endl;
	   for(int v=0;v<80;v++) cout<<'*';
	   cout<<endl<<setw(20)<<" ";

		textcolor(YELLOW);
	      textbackground(0);
		cout<<"FOOD ORDER RECORD TO BE MODIFIED IS FOUND"<<endl;
		cout<<"ARE YOU SURE YOU WANT TO MODIFY THE RECORD?(Y/N)";
		cin>>confirm;
		if(confirm=='Y' || confirm=='y')
		{
		getch();
		file.seekp(i*recsize*bucketsize+9,ios::beg);
		for(s=0;s<recsize-9;s++)
		   file.put('$');
		  cout<<"\t\t\tRECORD DELETED...!!"<<endl;
		  getch();
		   file.close();
		clrscr();
		textcolor(15);
	      textbackground(1);
		cout<<"\n";
		for(int b=0;b<80;b++) cout<<'~';
		for(int n=0;n<80;n++) cout<<'*';
		  cout<<"\t\t\tENTER THE NEW(MODIFIED) FOOD ORDER RECORD: \n";
		  getch();
		read();
		x=hash(order_no);
		store(x);
		cout<<"\t\t    RECORD IS MODIFIED SUCCESSFULLY...!!\n";
		getch();
		break;
		}
		else{
			getch();
			break;
		}
	}
	file.seekg(i*recsize*bucketsize+recsize,ios::beg);
	file.getline(order_no,10,'|');
	if(strcmp(order_no,"#########")==0)
	      break;

	if(strcmp(order_no,k)==0)
	{
		found=1;
		unpack();
		clrscr();
		textcolor(YELLOW);
	      textbackground(0);
	   cout<<endl;
	   cout<<setw(18)<<" "<<"FOOD ORDER RECORD TO BE MODIFIED IS DISPLAYED"<<endl<<endl;
	   for(int c=0;c<80;c++)   cout<<'*';   cout<<endl;
	   cout<<setw(20)<<" "<<" ORDER ID               :  "<<setw(10)<<order_no<<endl<<endl;
	   cout<<setw(20)<<" "<<" RESTAURANT CODE        :  "<<setw(9)<<restaurant_code<<endl<<endl;
	   cout<<setw(20)<<" "<<" CUSTOMER NAME          :  "<<setw(10)<<cust_name<<endl<<endl;
	   cout<<setw(20)<<" "<<" PHONE NUMBER           :  "<<setw(10)<<phone<<endl<<endl;
	   cout<<setw(20)<<" "<<" YOUR ORDER             :  "<<setw(6)<<order<<endl<<endl;
	   cout<<setw(20)<<" "<<" ORDER TIME             :  "<<setw(5)<<order_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" DELIVERY TIME          :  "<<setw(5)<<delivery_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" AMOUNT PAID            :  "<<setw(5)<<amount_paid<<endl<<endl;
	   for(int v=0;v<80;v++) cout<<'*';
	   cout<<endl<<setw(20)<<" ";

		 textcolor(YELLOW);
	      textbackground(0);
		cout<<"FOOD ORDER RECORD TO BE MODIFIED IS FOUND"<<endl;
		getch();
		file.seekp(i*recsize*bucketsize+recsize,ios::beg);
	      for(s=0;s<recsize;s++)
		   file.put('$');
		cout<<"\t\t\tRECORD DELETED...!!"<<endl;
		  getch();
		   file.close();
		clrscr();
		textcolor(15);
	      textbackground(1);
		cout<<"\n";
		for(int b=0;b<80;b++) cout<<'~';
		for(int n=0;n<80;n++) cout<<'*';
		  cout<<"\t\t\tENTER THE NEW(MODIFIED) FOOD ORDER RECORD: \n";
		  getch();
		read();
		x=hash(order_no);
		store(x);

		cout<<"\t\t    RECORD IS MODIFIED SUCCESSFULLY...!!\n";
		getch();
		break;
	}

	file.seekg(i*recsize*bucketsize+(2*recsize),ios::beg);
	file.getline(order_no,10,'|');
	if(strcmp(order_no,"#########")==0)
	      break;

	if(strcmp(order_no,k)==0)
	{
		found=1;
		unpack();
		clrscr();
		textcolor(YELLOW);
	      textbackground(0);
	   cout<<endl;
	   cout<<setw(18)<<" "<<"FOOD ORDER RECORD TO BE MODIFIED IS DISPLAYED"<<endl<<endl;
	   for(int c=0;c<80;c++)   cout<<'*';   cout<<endl;
	   cout<<setw(20)<<" "<<" ORDER ID               :  "<<setw(10)<<order_no<<endl<<endl;
	   cout<<setw(20)<<" "<<" RESTAURANT CODE        :  "<<setw(9)<<restaurant_code<<endl<<endl;
	   cout<<setw(20)<<" "<<" CUSTOMER NAME          :  "<<setw(10)<<cust_name<<endl<<endl;
	   cout<<setw(20)<<" "<<" PHONE NUMBER           :  "<<setw(10)<<phone<<endl<<endl;
	   cout<<setw(20)<<" "<<" YOUR ORDER             :  "<<setw(6)<<order<<endl<<endl;
	   cout<<setw(20)<<" "<<" ORDER TIME             :  "<<setw(5)<<order_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" DELIVERY TIME          :  "<<setw(5)<<delivery_time<<endl<<endl;
	   cout<<setw(20)<<" "<<" AMOUNT PAID            :  "<<setw(5)<<amount_paid<<endl<<endl;
	   for(int v=0;v<80;v++) cout<<'*';
	   cout<<endl<<setw(20)<<" ";

		textcolor(YELLOW);
	      textbackground(0);
		cout<<"FOOD ORDER RECORD TO BE MODIFIED IS FOUND"<<endl;
		getch();
		file.seekp(i*recsize*bucketsize+(2*recsize),ios::beg);
	       for(s=0;s<recsize-2;s++)
		   file.put('$');
		   file.put('\n');
		  cout<<"\t\t\tRECORD DELETED...!!"<<endl;
		  getch();
		   file.close();
		clrscr();
		textcolor(15);
	      textbackground(1);
		cout<<"\n";
		for(int b=0;b<80;b++) cout<<'~';
		for(int n=0;n<80;n++) cout<<'*';
		  cout<<"\t\t\tENTER THE NEW(MODIFIED) FOOD ORDER RECORD: \n";
		  getch();
		read();
		x=hash(order_no);
		store(x);

		cout<<"\t\t    RECORD IS MODIFIED SUCCESSFULLY...!!\n";
		getch();
		break;
	}
	else
	{
		i++;
		if(i%max==0)
			i=0;
	}
}while(i!=addr);
if(found==0)
{
	textcolor(15);
	      textbackground(1);
	cout<<setw(23)<<" "<<"\n\n\t!!*** GIVEN FOOD ORDER RECORD DOESNOT EXISTS IN HASH FILE ***!!\n\n";
	for(int k=0;k<80;k++) cout<<'=';
	for(int j=0;j<80;j++) cout<<'^';
	getch();
}
}



void Order::bill(int addr,char k[])
{
int found=0,i;
char dummy[10];
i=addr;
file.open(datafile,ios::in);
cout<<setiosflags(ios::left);
//if(l==1)
do
{
	file.seekg(i*recsize*bucketsize,ios::beg);
	file.getline(dummy,5,'\n');
	if(strcmp(dummy,"####")==0)
		break;
	file.seekg(i*recsize*bucketsize,ios::beg);
	file.getline(order_no,10,'|');
	if(strcmp(order_no,k)==0)
	{
		found=1;

		unpack();
		clrscr();
		textcolor(15);
	      textbackground(YELLOW);
	   cout<<endl;
	   cout<<setw(18)<<" "<<"THE BILL GENERATED FOR YOUR FOOD ORDER RECORD IS DISPLAYED"<<endl<<endl;
	   for(int f=0;f<80;f++)   cout<<'*';   cout<<endl;
	   cout<<setw(20)<<" "<<" ORDER ID               :  "<<setw(10)<<order_no<<endl<<endl;
	   cout<<setw(20)<<" "<<" CUSTOMER NAME          :  "<<setw(10)<<cust_name<<endl<<endl;
	   cout<<setw(20)<<" "<<" YOUR ORDER             :  "<<setw(6)<<order<<endl<<endl;
	   cout<<setw(20)<<" "<<" AMOUNT PAID            :  "<<setw(5)<<amount_paid<<endl<<endl;
	   for(int g=0;g<80;g++) cout<<'*';
	   cout<<endl<<setw(25)<<" ";

	   cout<<"FOOD ORDER RECORD FOUND...!!"<<endl;
		getch();
		break;
	}
	file.seekg(i*recsize*bucketsize+recsize,ios::beg);
	file.getline(order_no,10,'|');
	if(strcmp(order_no,"#########")==0)
	      break;
	if(strcmp(order_no,k)==0)
	{
		found=1;
		unpack();
		clrscr();
		textcolor(15);
	      textbackground(YELLOW);
	   cout<<endl;
	   cout<<setw(18)<<" "<<"THE BILL GENERATED FOR YOUR FOOD ORDER RECORD IS DISPLAYED"<<endl<<endl;
	   for(int f=0;f<80;f++)   cout<<'*';   cout<<endl;
	   cout<<setw(20)<<" "<<" ORDER ID               :  "<<setw(10)<<order_no<<endl<<endl;
	   cout<<setw(20)<<" "<<" CUSTOMER NAME          :  "<<setw(10)<<cust_name<<endl<<endl;
	   cout<<setw(20)<<" "<<" YOUR ORDER             :  "<<setw(6)<<order<<endl<<endl;
	   cout<<setw(20)<<" "<<" AMOUNT PAID            :  "<<setw(5)<<amount_paid<<endl<<endl;
	   for(int g=0;g<80;g++) cout<<'*';
	   cout<<endl<<setw(25)<<" ";

	cout<<"FOOD ORDER RECORD FOUND...!!"<<endl;
		getch();
		break;
	}

	file.seekg(i*recsize*bucketsize+(2*recsize),ios::beg);
	file.getline(order_no,10,'|');
	if(strcmp(order_no,"#########")==0)
	      break;
	if(strcmp(order_no,k)==0)
	{
		found=1;
		unpack();
		clrscr();
		textcolor(15);
	      textbackground(YELLOW);
	   cout<<endl;
	   cout<<setw(18)<<" "<<"THE BILL GENERATED FOR YOUR FOOD ORDER RECORD IS DISPLAYED"<<endl<<endl;
	   for(int f=0;f<80;f++)   cout<<'*';   cout<<endl;
	   cout<<setw(20)<<" "<<" ORDER ID               :  "<<setw(10)<<order_no<<endl<<endl;
	   cout<<setw(20)<<" "<<" CUSTOMER NAME          :  "<<setw(10)<<cust_name<<endl<<endl;
	   cout<<setw(20)<<" "<<" YOUR ORDER             :  "<<setw(6)<<order<<endl<<endl;
	   cout<<setw(20)<<" "<<" AMOUNT PAID            :  "<<setw(5)<<amount_paid<<endl<<endl;
	   for(int g=0;g<80;g++) cout<<'*';
	   cout<<endl<<setw(25)<<" ";

	cout<<"FOOD ORDER RECORD FOUND...!!"<<endl;
		getch();
		break;
	}
	else
	{
		i++;
		if(i%max==0)
			i=0;
	}
}while(i!=addr);
if(found==0 )
{
		textcolor(15);
	      textbackground(YELLOW);
	cout<<setw(23)<<" "<<"\n\n\t!!*** GIVEN FOOD ORDER RECORD DOESNOT EXISTS IN HASH FILE ***!!\n\n";
	for(int h=0;h<80;h++) cout<<'~';
	for(int j=0;j<80;j++) cout<<'~';
	getch();
}
	/*if(l==0)
	    cout<<found<<endl;*/

}





/*Douoblebox design*/

void drawdoublelinebox(int x,int y,int len,int bdt)
{
	gotoxy(x,y);
	cprintf("ษ");
		gotoxy(x,y+bdt);
	cprintf("ศ");
	gotoxy(x+len,y);
	cprintf("ป");
	gotoxy(x+len,y+bdt);
	cprintf("ผ");
	for(int i=x+1;i<x+len;i++)
	{
		gotoxy(i,y);
		cprintf("อ");
		gotoxy(i,y+bdt);
		cprintf("อ");
	}
	for(int j=y+1;j<y+bdt;j++)
	{
		gotoxy(x,j);
		cprintf("บ");
		gotoxy(x+len,j);
		cprintf("บ");
			}
}

/*Double box design*/

void drawsinglelinebox(int x,int y,int len,int bdt)
{                                            // (x,y) : co.ord. of 1st tip
	gotoxy(x,y);                         // len : length of box
	cprintf("ฺ");                        // bdt : breadth of box
	gotoxy(x,y+bdt);
	cprintf("ภ");
	gotoxy(x+len,y);
	cprintf("ฟ");
	gotoxy(x+len,y+bdt);
	cprintf("ู");

	for(int i=x+1;i<x+len;i++)
	{
		gotoxy(i,y);
		cprintf("ฤ");
		gotoxy(i,y+bdt);
		cprintf("ฤ");
	}
	for(int j=y+1;j<y+bdt;j++)
	{
		gotoxy(x,j);
		cprintf("ณ");
		gotoxy(x+len,j);
		cprintf("ณ");
	}
}
void normal()  // to obtain normal background
{
	textattr(WHITE); // textattr sets text attributes for text-window functions
	textcolor(WHITE);// textcolor selects a new character color in text mode
	textbackground(BLACK);//textbackground selects a new text background color
	_setcursortype(_NORMALCURSOR); /* sets cursor type to normal underscore type cursor  */
	//return;
}

void screen()
{
 int g=19,h=10;

     textbackground(GREEN);
     gotoxy(24,3);
     textcolor(YELLOW);
     cprintf("       FOOD ORDER MANAGEMENT       ");
     gotoxy(24,4);
     textcolor(15);
     cprintf(" อออออออออออออออออออออออออออออออออ ");
     drawdoublelinebox(17,7,44,14);
     gotoxy(28,7);
     textbackground(RED);
	 textcolor(YELLOW);
	 cprintf("Û  M A I N    M E N U Û" );
     normal();
     textcolor(15);
     gotoxy(g,h++);
     cout<<"	     þ FOOD ORDER RECORD";
     gotoxy(g,h++);
     cout<<"	     þ QUIT";
     gotoxy(g,h++);
     gotoxy(g,h++);
     gotoxy(g,h++);
     normal();
}


void main()
{
int ch,addr,l,choice, found=0;
char sord_no[10];
Order p;
p.initial();
clrscr();
do
{
	screen();
	//cout<<"\n============================"<<endl;
	//cout<<"1.read food order details\n2.search food order record\n3.display food order record\n4.delete food order record\n5.modify record\n6.exit\n";
      // cout<<"============================"<<endl;
	cout<<"PLEASE ENTER YOUR CHOICE [1-2] :  ";
	cin>>ch;
	switch(ch)
	{
	case 1:do
	      {
	      clrscr();
	      drawdoublelinebox(17,7,44,14);
	      textcolor(0);
	      textbackground(15);
		  clrscr();
		  textbackground(BLACK);
	      gotoxy(25,3);
	      textcolor(YELLOW);
	      cprintf("       FOOD ORDER RECORDS      ");
	      gotoxy(25,4);
	      cprintf(" อออออออออออออออออออออออออออออ ");
	      int z=19,q=10;
	      textcolor(YELLOW);
	      drawdoublelinebox(17,7,44,14);
	      gotoxy(28,7);
	      textbackground(RED);
	      textcolor(YELLOW);
	      cprintf("Û  M A I N    M E N U Û" );
	      normal();
	      textcolor(WHITE);
	      gotoxy(z,q++);
	      cout<<"     1.>  READ FOOD ORDER DETAILS";
	      gotoxy(z,q++);
	      cout<<"     2.>  SEARCH FOOD ORDER RECORD";
		  gotoxy(z,q++);
	      cout<<"     3.>  DISPLAY FOOD ORDER DETAILS";
	      gotoxy(z,q++);
	      cout<<"     4.>  DELETE FOOD ORDER RECORD";
	      gotoxy(z,q++);
	      cout<<"     5.>  MODIFY FOOD ORDER RECORD";
	      gotoxy(z,q++);
	      cout<<"     6.>  GENERATE BILL";
	      gotoxy(z,q++);
	      cout<<"     7.>  QUIT";
	      gotoxy(z,q++);
	      gotoxy(z,q++);
	      cout<<"    PLEASE ENTER YOUR CHOICE [1-7]  : ";
	      cin>>choice;
	 switch(choice)
	 {
	case 1:clrscr();
		cout<<"\n";
		for(int b=0;b<80;b++) cout<<'~';
		cout<<setw(24)<<" "<<"ENTER THE FOOD ORDER RECORD DETAILS \n";
		for(int h=0;h<80;h++) cout<<'~';
		cout<<"\n";
		getch();
	 LABEL: p.read();
		addr=hash(p.order_no);
		store(addr);
		getch();
		file.close();
		break;
	case 2:clrscr();
		textcolor(15);
	      textbackground(YELLOW);
		cout<<"\n\n\n\n";
		cout<<"\t\t\t  SEARCHING FOOD ORDER RECORD\n\n";
		for(int e=0;e<80;e++) cout<<'~';
		//cout<<"\n";
		for(int w=0;w<80;w++) cout<<'~';
		cout<<"\n";
		cout<<setw(20)<<" "<<"ENTER THE ORDER ID TO BE SEARCHED: "<<endl;
		cout<<setw(36)<<" ";
		cin>>sord_no;
		cout<<"\n";
		addr=hash(sord_no);
		p.retrieve(addr,sord_no,1);
		getch();
		file.close();
		break;
	case 3://cout<<"FOOD ORDER DETAILS:"<<endl;
		textcolor(15);
	      textbackground(RED);
	       p.datadisp();
	       file.close();
		break;
       case 4://p.del();
		clrscr();
		textcolor(15);
	      textbackground(1);
		cout<<"\n\n\n\n";
		cout<<"\t\t\t  DELETING FOOD ORDER RECORD\n\n";
		for(int x=0;x<80;x++) cout<<'^';
		//cout<<"\n";
		for(int y=0;y<80;y++) cout<<'^';
		cout<<"\n";
		cout<<setw(20)<<" "<<"ENTER THE ORDER ID TO BE DELETED: "<<endl;
		cout<<setw(36)<<" ";
		cin>>sord_no;
		cout<<"\n";
		addr=hash(sord_no);
		p.remove(addr,sord_no);
		getch();
		file.close();
		break;
	case 5:clrscr();
		textcolor(15);
	      textbackground(1);
		cout<<"\n\n\n\n";
		cout<<"\t\t\t  MODIFYING FOOD ORDER RECORD\n\n";
		for(int u=0;u<80;u++) cout<<'=';
		//cout<<"\n";
		for(int r=0;r<80;r++) cout<<'^';
		cout<<"\n";
		cout<<setw(20)<<" "<<"ENTER THE FOOD ORDER RECORD TO MODIFY: \n";
		cout<<setw(36)<<" ";
		cin>>sord_no;
	      addr=hash(sord_no);
		cout<<"\n";
	      p.modify(addr,sord_no);
		getch();
	     file.close();
	      break;
	case 6: clrscr();
		textcolor(15);
		textbackground(YELLOW);
		cout<<"\n\n\n\n";
		cout<<"\t\t\t\t  YOUR BILL \n\n";
		for(int t=0;t<80;t++) cout<<'~';
		//cout<<"\n";
		for(int z=0;z<80;z++) cout<<'~';
		cout<<"\n";
		BILL:
		cout<<setw(20)<<" "<<"ENTER THE ORDER ID TO GENERATE THE BILL: "<<endl;
		cout<<setw(36)<<" ";
		cin>>sord_no;
		if(strlen(sord_no)!=8){
			goto BILL;
		}
		cout<<"\n";
		addr=hash(sord_no);
		p.bill(addr,sord_no);
		getch();
		file.close();
		break;

	case 7:	clrscr();
		screen();
		break;
	default:sound(600);delay(300);nosound();
		break;
	}
	}while(choice!=7);   break;

case 2:exit(0);

default: sound(100);delay(500);nosound(); break;
}
}while(ch!=2);

       file.close();
}