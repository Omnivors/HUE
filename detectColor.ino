void readRGB(float colors[])
{
  uint16_t lux, red, green, blue;
  tcs.getRawData(&red, &green, &blue, &lux);

  // Figure out some basic hex code for visualization
  uint32_t sum = red;  sum += green;  sum += blue;  sum += lux;
  
  float r, g, b;
  r = red; r /= sum;  g = green; g /= sum;  b = blue; b /= sum;
  r *= 256; g *= 256; b *= 256;
  colors[0] = r;  colors[1] = g;  colors[2] = b;  
  return;
}

byte readColor(float col [])
{ 
 byte  colorCode = 0;
 byte red, grn, blu;
 String color = "NO_COLOR";
 red = round(col[0]);
 grn = round(col[1]);
 blu = round(col[2]);

  // LL = lowlow, L = low, H = high, HH = highhigh
 byte redLL = 45; byte redL = 50; byte redH = 57; byte redHH = 68; 
 byte grnLL = 37; byte grnL = 44; byte grnH = 48; byte grnHH = 54;
 byte bluLL = 37; byte bluL = 48; byte bluH = 51;

  if (red > redHH && grn < grnL && blu < bluL)   {color = "RED"; colorCode = 1;}
  else if (red > redHH  && grn < grnL && blu > bluLL )   {color = "MAGENTA"; colorCode = 4;}
  else if (red > redL && red < redHH && grn > grnL && blu < bluH)   {color = "YELLOW"; colorCode = 5;}
  else if (red > redL &&  grn > grnLL && grn < grnHH && blu < bluL)   {color = "ORANGE"; colorCode = 6;}
  else if (red < redH && grn > grnHH && blu < bluH)  { color = "GREEN"; colorCode = 2;}
  else if (red < redH && grn < grnHH && blu > bluH)  { color = "BLUE"; colorCode = 3;}
  else { color = "NO COLOR"; colorCode = 0;}

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
