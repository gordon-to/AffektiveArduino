const int GSR=1;
int threshold=0;

void setup_gsr(){
  long sum=0;
  delay(1000);
  
  for(int i=0;i<500;i++)
  {
  gsr_value=analogRead(GSR);
  sum += gsr_value;
  delay(5);
  }
  threshold = sum/500;
   Serial.print("threshold =");
   Serial.println(threshold);
}

void read_gsr(){
  gsr_value = 102400.00/analogRead(GSR);
}
