#include <stdbool.h>
/*Returnera antalet valda resitorer med vilka kan orig_resistance ersättas
orig_resistance - det öriginella resitorer som ska ersättas (värde i Ohm)
res_array - värde med resulterande resistanser
*/
int e_resistance(float orig_resistance, float *res_array )
{
       //resistorserie e12  
       float  e12[12]={10.00,12.00,15.00,18.00,22.00,27.00, 33.00,39.00,47.00,56.00,68.00,82.00};
       //multiplicerare nponser av 10
	float mult10[6]={1.00,10.00,100.00,1000.00,10000.00,100000.00};
       //antalet valda resistorer 
       int antalValda=0;
       //räknare i loopar
       int i,j;
       //true=om intervallen hittat (var någonstans ligger resistors värde) 
       bool found=false;
       //om antalet valda är mindre än 3 och resistans som ska ersättas >= 10 ohm
      while(antalValda<3 && orig_resistance>=mult10[0]*e12[0])
      {     // om resistans >1 MegaOhm
	    if(orig_resistance>mult10[5]*e12[0] )
            {   //välj 1 megaohm och minska värde med 1 megaohm
           	*(res_array+antalValda)=mult10[5]*e12[0] ;
          	 antalValda++;
                 orig_resistance-=mult10[5]*e12[0];
            } 
      	    else 
            {
		for(j=0;j<6;j++)
      		{
           
            		 if(j<5)
            		{   	//kolla alla resistorer i e12 resistorserie 		
        			for (i=0;i<11;i++)
				{	//om resistor R som ska ersättas är mellan två resistorer i e12 resistorserie
                                        //  R1<R<R2  t.ex.   68<R<82
 					if(orig_resistance>=mult10[j]*e12[i] && orig_resistance<mult10[j]*e12[i+1])
					{
                                             //sätt på rätt plats(resulterande array) mindre värde (t.ex. 68)  och öka 						     //antalet valda 
                                             *(res_array+antalValda)=mult10[j]*e12[i];
					     antalValda++;
                                             //om resistor som ska ersättas är lika med något värde i e12 serie 
                                             if(orig_resistance==mult10[j]*e12[i])
                                             {
                                                 //returnera antalet valda resistorer  
                                                 return antalValda;
                                             }
                                             //minska värde på resistor som ska ersättas , bryt loppen 
					     orig_resistance-=mult10[j]*e12[i];
					     found=true;
                                             break;
						
					}
                                        /*om värde av resistorer som ska ersättas är störrre än sista värde         						med mindre   multiplikator och
                                        är större än första värde multiplicerat med större multiplikator t.ex 82< R <100*/ 
                                        else if(orig_resistance>=mult10[j]*e12[11] && orig_resistance<mult10[j+1]*e12[0])
					{
                                             //sätt på rätt plats mindre värde (t.ex 82)  och öka antalet valda
					     *(res_array+antalValda)=mult10[j]*e12[11];
					     antalValda++;
                                             //om resistor som ska ersättas är lika med mindre värde (e12 serie)
                                             if(orig_resistance==mult10[j]*e12[11])
                                             {
                                                 return antalValda;
                                             }
                                             //minska värde på resistor som ska ersättas , bryt loppen  
					     orig_resistance-=mult10[j]*e12[11];
					     found=true;
                                             break;
					}			
				}
       	               }
                      //om intervallen hittat
                       if(found)
                           break;
	       }
         } 
         found=false;
     }
   
     return antalValda; 
       	
}

