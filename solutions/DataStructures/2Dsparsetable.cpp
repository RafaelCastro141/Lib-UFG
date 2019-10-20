int a[N][N];
int tab[K][N][K][N];
int n,m;
int rlen,kr,pr,clen,kc,pc,m1,m2; 

int query(int x1,int y1,int x2,int y2)
{
    rlen=x2-x1+1;  //2
    kr=log2(rlen);  //1
    pr=(1<<kr);      //2
  // int rem_r=rlen-pr;  //0
   
     clen=y2-y1+1;   //2
     kc=log2(clen);  //1
     pc=(1<<kc);     //2
   //int rem_c=clen-pc;  //0

   //cout<<kr<<" "<<x1<<endl;
     m1=max(tab[kr][x1][kc][y1],tab[kr][x1][kc][y1+clen-pc]);
   //cout<<m1<<endl;
     m2=max(tab[kr][x1+rlen-pr][kc][y1],tab[kr][x1+rlen-pr][kc][y1+clen-pc]);
   //cout<<m2<<endl;
   return max(m1,m2);
}


void sparse()
{
  int rlog=log2(n)+1;
  int clog=log2(m)+1;
  int q;
   for(int rj=0;rj<rlog;rj++)
   {
      if(rj>0)
      {
         q=(1<<rj-1);
      }
     for(int ri=0;ri<n;ri++)
     {

    if(rj==0){

        for(int i=0;i<m;i++){
        	tab[rj][ri][0][i]=a[ri][i];
      	}
	      	for (int j=1;j<clog;j++){
	            int p=(1<<j-1);
	            for(int i=0;i<m;i++)
	            {
	                if(i+p<m)
	                {
	                  tab[rj][ri][j][i]=max(tab[rj][ri][j-1][i],tab[rj][ri][j-1][i+p]);
	                }
	            }
	          }
      }
      else
      {
          // if(ri+2*q-1<n)
           //{
            for(int j=0;j<clog;j++)
           {
               for(int i=0;i<m;i++)
               {
                  tab[rj][ri][j][i]=max(tab[rj-1][ri][j][i],tab[rj-1][ri+q][j][i]);
               }
           }
           //}
      }      

     }
   }
}


	void check()
	{

	   for(int rj=0;rj<log2(n)+1;rj++)
	   {
	     for(int ri=0;ri+(1<<rj)-1<n;ri++)
	     {
	       //cout<<"row->"<<ri<<"  utpo->"<<(ri-1+(1<<rj))<<endl;
	       cout<<rj<<" "<<ri<<endl;
	       for(int cj=0;cj<log2(m)+1;cj++)
	       {
	        cout<<cj<<"-> ";
	         for(int ci=0;ci<m;ci++)
	         {
	           cout<<tab[rj][ri][cj][ci]<<" ";
	         }
	         cout<<endl;
	       }
	       cout<<endl;cout<<endl;cout<<endl;
	     }
	   }
	}
