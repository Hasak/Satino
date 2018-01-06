#include <DS3232RTC.h> 

//unsigned long  h0=8;
//unsigned long  m0=14;
//unsigned long  s0=30;
bool a[16],al[16];
tmElements_t tm;
void setup(){
  int c=0,i;
  for(i=2;i<18;i++){
    pinMode(i,OUTPUT);
    digitalWrite(i,0);
    al[i-2]=0;
  }
  pinMode(0,INPUT_PULLUP);
  pinMode(1,INPUT_PULLUP);
  setSyncProvider(RTC.get); 
  for(i=2;i<18;i++){
    digitalWrite(i,1);
    delay(100);
  }
  for(i=2;i<18;i++){
    digitalWrite(i,0);
    delay(100);
  }
  delay(500);
  //Serial.begin(9600);
  
  /*
  tm.Hour = 18;             //set the tm structure to 23h31m30s on 13Feb2009
  tm.Minute = 06;
  tm.Second = 25;
  tm.Day = 17;
  tm.Month = 10;
  tm.Year = 2019 - 1970;    //tmElements_t.Year is the offset from 1970
  RTC.write(tm);
  */
}
void loop(){
  unsigned long m,h,mts,mmts,c=0,s,i;
//  t=millis()/1000+s0;
  //t*=20;
  //h=(h0+t/3600)%12;
  //m=m0+(t%3600)/60;
  //s=t%60;
  //tmElements_t tm;
  RTC.read(tm);
  
  h=tm.Hour;
  m=tm.Minute;
  s=tm.Second;

  if(digitalRead(1))
    h++;
  h%=12;

  if(digitalRead(0)==LOW){
    if(tm.Minute==59){
      tm.Minute=0;
      tm.Hour++;
      tm.Hour%=24;
    }
    else tm.Minute++;
    RTC.write(tm);
    delay(250);
  }
  
  mts=m/5;
  mmts=m%5;
  
  for(i=0;i<16;i++){
    a[i]=0;
  }
  
  a[h]=1;
  if(s%2==0)
    a[mts]=1;
  else a[mts]=0;
  
  if(h%2==0)
     while(c<mmts){
        a[12+c]=true;
        c++;
     }
   else
     while(c<mmts){
        a[15-c]=true;
        c++;
     }
 /* for(i=0;i<12;i++){
    Serial.print(a[i]);
    Serial.print(" ");
  }
  Serial.print(" -  ");
  for(i=12;i<16;i++){
    Serial.print(a[i]);
    Serial.print(" ");
  }
  Serial.println();*/
  refresh();
}
void refresh(){
  //Serial.println(millis());
    int fad=0,kol,i;
    while(fad<99){
      kol=99-fad;
      for(i=0;i<16;i++)if(a[i]==1 && al[i]==0)
      digitalWrite(i+2,1);
      delayMicroseconds(fad*50);
      for(i=0;i<16;i++)if(a[i]==1 && al[i]==0)
      digitalWrite(i+2,0);
      delayMicroseconds(kol*50);
      fad++;
    }
    for(i=0;i<16;i++)if(a[i]==1 && al[i]==0){
      al[i]=1;
      digitalWrite(i+2,1);
    }

    fad=99;
      while(fad>=0){
      kol=99-fad;
      for(i=0;i<16;i++)if(a[i]==0 && al[i]==1)
      digitalWrite(i+2,1);
      delayMicroseconds(fad*50);
      for(i=0;i<16;i++)if(a[i]==0 && al[i]==1)
      digitalWrite(i+2,0);
      delayMicroseconds(kol*50);
      fad--;
    }
    for(i=0;i<16;i++)if(a[i]==0 && al[i]==1){
      al[i]=0;
      digitalWrite(i+2,0);
    }
  //Serial.println(millis());
}
