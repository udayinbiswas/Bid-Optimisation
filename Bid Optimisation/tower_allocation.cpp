#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <list>
#include <ctime>

#define max 10000
using namespace std;

struct node // bid node structure
{
	int cid; //company
	double price;
	int norc;  // no. of regions in one bid
	int region[max];
};

float tim;
int nor;
int nob;
int noc;

struct node lob[max]; // total no. of bids

bool com[max],reg[max]; //keeps record of which companies and regions can be seen in the remaining unprocessed bids
bool fbid[max]; // final bids


//function to take input - read from console by redirection
void readFile(char* inp)
{
	ifstream infile(inp);

	infile >> tim >> nor >> nob >> noc;

	string g;
	getline(infile,g);
	getline(infile,g);

	for(int i=0;i<nob;i++)
	{
		cout<<flush;
		string ch;
		getline(infile,ch);

		int t=0;int j=0;
		char ch1[max];
		while(ch[t]!=' ')
		{
			ch1[j]=ch[t];
			j++;t++;
		}

		ch1[j]='\0';
		lob[i].cid=atoi(ch1);

		ch1[0]='\0';j=0;t++;

		while(ch[t]!=' ')
		{
			ch1[j]=ch[t];
			j++;t++;
		}

		ch1[j]='\0';
		lob[i].price=strtod(ch1, NULL);
		t++;

		int x=0;
		int w=t;
		while(ch[t]!='#')
		{
			if(ch[t]==' ')
			{	x++;}
			t++;
		}
		lob[i].norc=x;
		t=w;
		for(int qq=0;qq<x;qq++)
		{
			ch1[0]='\0';j=0;
			while(ch[t]!=' ')
			{
				ch1[j]=ch[t];
				j++;t++;
			}
			t++;
			ch1[j]='\0';
			lob[i].region[qq] = atoi(ch1);
		}
		getline(infile,g);
	}
	srand ( time(NULL) );
}

// give us a number based on its availabilty in array
int retNum(int array[],int size,int p)
{
	while (array[p]==1)
   	{
   		p=(p+1)%size;
   	}
   	return p;
}

 //terminate when all the random restarts have been done with
bool fullArray(int array[],int size)
{    bool truth=true;
	for (int a=0;a<size;a++)
	{
		if (array[a]==0)
		{	truth=false;
        	break;
		}
 	}
    	return truth;
}




void fill(int);
bool checkReg(int);


void getRandom(char* outp) //modify this function to produce the best output(following the conditions mentioned in the assignment)
{
	int numb,i;
	int maxP=0; int fullA[nob]; int myBid[max];
	for (int y=0;y<nob;y++)
	{
		fullA[y]=0;
	}

	int resetfbid[max]; int resetcom[max]; int resetreg[max]; bool seen[max];

    while (!fullArray(fullA,nob))
    {
    	int count = 0;
    	for (int a=0;a<nob;a++)
    	{
    		fbid[a]=resetfbid[a];
    	}

    	for (int a=0;a<nob;a++)
    	{
    		com[a]=resetcom[a];
    	}

    	for (int a=0;a<nob;a++)
    	{
    		reg[a]=resetreg[a];
    	}

    	for (int a=0;a<nob;a++)
    	{
    		seen[a]=false;
    	}

    	numb=rand()%nob;

    	int num1=retNum(fullA,nob,numb);
    	fullA[num1]=1;

    	seen[num1] = true;
    	fill(num1);
    	bool added = true;
    	while(added)
    	{
    		added = false;
    		list<int> neighbours;

    		//listing the neighbours
    		for(int i=(num1+1)%nob;i!=num1;i=(i+1)%nob){

    			if(seen[i])
    				continue;
    			else if(com[lob[i].cid]){
    				seen[i] = true;
    				continue;
    			}
    			else if(checkReg(i)){
    				seen[i] = true;
    				continue;
    			}
    			neighbours.push_front(i);
    		}

    		double maxPrice = -1;
    		int max_neig = -1;
    		int secMax_neig = -1;
    		int thiMax_neig = -1;
    		//int beamK = neighbours.size();
    		// selecting the max neighbou

    		for(list<int>::iterator list_iter = neighbours.begin(); list_iter != neighbours.end(); list_iter++)
    		{

				if(maxPrice<lob[*list_iter].price){
					thiMax_neig = secMax_neig ;
					secMax_neig = max_neig;
					maxPrice = lob[*list_iter].price;
					max_neig = *list_iter;
				}

    		}
		//if it is -1 then there is no neighbour and added becomes false exiting out of loop
			if(max_neig!=-1){
				if(count<10000&&secMax_neig!=-1){
					int ran = rand()%3;
					if(ran==0){
						fill(thiMax_neig);
						seen[thiMax_neig] = true;
					}
					else if (ran==1){
						fill(secMax_neig);
						seen[secMax_neig] = true;
					}
					else{
						fill(max_neig);
						seen[max_neig] = true;
					}
					count++;
				}else if(count<10000&&secMax_neig!=-1){
					int ran = rand()%2;
					if(ran==0){
						fill(secMax_neig);
						seen[secMax_neig] = true;
					}
					else{
						fill(max_neig);
						seen[max_neig] = true;
					}
					count++;
				} else

				{
					fill(max_neig);
					seen[max_neig] = true;
				}
				added=true;
			}

		}
		double tPrice = 0;
			 //compare different restarts here based on the price and then equate
		for(i=0;i<nob;i++)
		{
			if(fbid[i])
			{	tPrice+=lob[i].price;
			}
		}

         if (tPrice>maxP)
         	{
         	  maxP=tPrice;
              for(i=0;i<nob;i++)
              {
            	  myBid[i]=fbid[i];
              }
         	}

    }

	ofstream myfile;
	myfile.open(outp);
    string outputStr="";

	double totalPrice = 0;
	//max bid's bids are being printed
	for(i=0;i<nob;i++)
	{
		if(myBid[i])
		{
			totalPrice+=lob[i].price;
            outputStr+=std::to_string(i)+" ";
		}
	}

	outputStr+="#";
	cout<<to_string(totalPrice)<<endl;
	myfile << outputStr;
    myfile.close();

}

//helper function of getRandom function
void fill(int bidno)
{
	com[lob[bidno].cid]=true;
	fbid[bidno]=true;
	for(int i=0;i<lob[bidno].norc;i++)
	{
		reg[lob[bidno].region[i]]=true;
	}
}

//helper function of getRandom function
bool checkReg(int bidno)
{
	for(int i=0;i<lob[bidno].norc;i++)
	{
		if(reg[lob[bidno].region[i]]==true)
			return true;
	}
	return false;
}

int main(int argc, char* argv[])
{
	readFile(argv[1]);
	getRandom(argv[2]);

	return 0;
}
