unsigned char buffer[64];
int count;
String employee_name;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) ;
  Serial1.begin(9600);
}

void loop() {
  delay(1000);

  if (Serial1.available())
  {
    while (Serial1.available())              
    {
      buffer[count++] = Serial1.read();      
      if (count == 64)break;
    }
    Serial.write(buffer, count);
    Serial.println();    
    clearBufferArray();             
    count = 0;       
  }
}

void clearBufferArray()  
{
  for (int i = 0; i < count; i++)
  {
    buffer[i] = NULL;
  }
}
