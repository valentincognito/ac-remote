void setup() {
  // put your setup code here, to run once:
  printf ("setup complete");
}

void loop() {
  // Calculate based on max input size expected for one command
  #define INPUT_SIZE 30

  // Get next command from Serial (add 1 for final 0)
  char input[INPUT_SIZE + 1];
  byte size = Serial.readBytes(input, INPUT_SIZE);
  // Add the final 0 to end the C string
  input[size] = 0;
  
  // Read each command pair 
  char* command = strtok(input, "&");
  while (command != 0)
  {
      // Split the command in two values
      char* separator = strchr(command, ':');
      if (separator != 0)
      {
          // Actually split the string in 2: replace ':' with 0
          *separator = 0;
          int power = atoi(command);
          ++separator;
          int temperature = atoi(separator);
  
          // Do something with servoId and position
          
      }
      // Find the next command in input string
      command = strtok(0, "&");


//      char str[] ="- This, a sample string.";
//      char * pch;
//      printf ("Splitting string \"%s\" into tokens:\n",str);
//      pch = strtok (str," ,.-");
//      while (pch != NULL)
//      {
//        printf ("%s\n",pch);
//        pch = strtok (NULL, " ,.-");
//      }
//      return 0;
  }

}
