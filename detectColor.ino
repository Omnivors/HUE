void readRGB(float colors[])
{
  uint16_t lux, red, green, blue;
  tcs.getRawData(&red, &green, &blue, &lux);

  // Figure out some basic hex code for visualization
  uint32_t sum = red;  sum += green;  sum += blue;  sum += lux;
  
  float r, g, b;
  r = red; r /= sum;  g = green; g /= sum;  b = blue; b /= sum;
  r *= 256; g *= 256; b *= 256;

  colors[0] = r;
  colors[1] = g;
  colors[2] = b;
  
  return;
}

byte readColor(float col [])
{ 
 byte  colorCode = 0;
 float red, grn, blu;
 String color = "NO_COLOR";
 red = col[0];
 grn = col[1];
 blu = col[2];

 byte redMin = 45;  byte redMax = 67;
 byte grnMin = 48; byte grnMax = 56;
 byte bluMin = 48; byte bluMax = 52;

 float ylwCoeff = 0.70;
 float magCoeff = 0.7;
 float redCoeff = 0.8;
 float bluCoeff = 0.85;
 float orgCoeff = 0.77;

  if (red > magCoeff*redMax && grn < (0.1+magCoeff)*grnMin && blu < bluMin )   {color = "MAGENTA"; colorCode = 4;}
  if (red < redMin && grn > grnMax && blu < bluMin)  { color = "GREEN"; colorCode = 2;}
  if (red < redMin/bluCoeff && grn < grnMin/bluCoeff && blu > bluMax)  { color = "BLUE"; colorCode = 3;}
  if (red > orgCoeff*redMax && grn > (orgCoeff-0.3)*grnMax && blu < bluMin*orgCoeff)   {color = "ORANGE"; colorCode = 6;}
  if (red > ylwCoeff*redMax && grn > ylwCoeff*redMax && blu < bluMin*ylwCoeff)   {color = "YELLOW"; colorCode = 5;}
  if (red > redMax && grn < grnMin && blu < bluMin*redCoeff)   {color = "RED"; colorCode = 1;}

  //Serial.println(color);
  
  return colorCode;
}

byte decideColor(byte colorCode, byte oldCodes [], byte Mem)
{ 
  byte color;
  color = 0;
  
  // Add the new color to the array
  for (int i=0; i<Mem-1; i++)
  {    
   oldCodes[i] = oldCodes[i+1];  //Serial.print((int)oldCodes[i]);  Serial.print("\t"); 
  }
  oldCodes[Mem-1] = colorCode;

  // If a color has been seen at least "Mem/2" times in the last "Mem" checks, set it as the current color.
  colorCode = maxColorPresence(oldCodes, Mem);
  
  return colorCode;
}


int maxColorPresence(byte arr [], byte N)
{
    byte freq[N];
    byte i, j, count, maxFreqIndex;
    byte detectedColor = 0;

    // Initialize the array "frequencies" to "-1"
   memset(freq,-1,N);


    for(i=0; i<N; i++)
    {
        count = 1;
        for(j=i+1; j<N; j++)
        {
            /* If duplicate element is found */
            if(arr[i]==arr[j])
            {
                count++;
                /* Make sure not to count frequency of same element again */
                freq[j] = 0;
            }
        }

        /* If frequency of current element is not counted */
        if(freq[i] != 0)
        {
            freq[i] = count;
        }
        //Serial.print((int)freq[i]); Serial.print(" ");
    }
    
    maxFreqIndex = findMaxFrequency(freq, N);
    detectedColor = arr[maxFreqIndex];

    return detectedColor;
}

int findMaxFrequency(byte arr [], byte N)
{
  int maximum, c, location = 0;

  maximum = arr[0];
 
  for (c = 1; c < N; c++)
  {
    if (arr[c] > maximum)
    {
       maximum  = arr[c];
       location = c;
    }
  }

  if (maximum < floor(N/2))
    location = 0;
 
  return location;
}
