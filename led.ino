
String myh;
String myb;
String mys;
float h;
float b;
float s;
uint32_t currbri;
uint32_t r;
uint32_t g;
uint32_t bl;

void setPixelColor( uint16_t n,  uint16_t brightness) {
  pixels.setPixelColor(n, (brightness * r / 255) , (brightness * g / 255), (brightness * bl / 255));
}

uint32_t HSVColor(float h, float s, float v) {

  h = constrain(h, 0, 360);
  s = constrain(s, 0, 1);
  v = constrain(v, 0, 1);

  int i, b, p, q, t;
  float f;

  h /= 60.0;  // sector 0 to 5
  i = floor( h );
  f = h - i;  // factorial part of h

  b = v * 255;
  p = v * ( 1 - s ) * 255;
  q = v * ( 1 - s * f ) * 255;
  t = v * ( 1 - s * ( 1 - f ) ) * 255;
  switch ( i ) {
    case 0:
      r = b; g = t; bl = p;
      return pixels.Color(b, t, p);
    case 1:
      r = q; g = b; bl = p;
      return pixels.Color(q, b, p);
    case 2:
      r = p; g = b; bl = t;
      return pixels.Color(p, b, t);
    case 3:
      r = p; g = q; bl = b;
      return pixels.Color(p, q, b);
    case 4:
      r = t; g = p; bl = b;
      return pixels.Color(t, p, b);
    default:
      r = b; g = p; bl = q;
      return pixels.Color(b, p, q);
  }
}
void CallBrightness(uint32_t i) {
  HSVColor(h, s, b);
  for (int j = 0; j < NUMPIXELS; j++) {
    setPixelColor(j, i * 2.5);
  }
  pixels.show();
  /*
    pixels.setBrightness(i);
    for (int i = 0; i < NUMPIXELS; i++) {
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(150, 0, 0)); // Moderately bright green color.
    }
    pixels.show(); // This sends the updated pixel color to the hardware.
  */
}

void hueSet() {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, HSVColor(h, s, b));
  }
  pixels.show();
}


void extractLEDDetails(String payload) {

  //payload[length] = '\0';
  //String input = (char*)payload;
  String input = payload;
  Serial.println(input);
  if (input.indexOf("LIGHTON") > -1) {
    b = currbri;
  }
  if (input.indexOf("LIGHTOFF") > -1) {
    currbri = b;
    b = 0;
  }

  int count = input.length();
  if (input.indexOf("brightness") > -1) {
    myb = input.substring(10, count);
    b = myb.toInt();
  }
  if (input.indexOf("hue") > -1) {
    myh = input.substring(3, count);
    h = myh.toInt();

  }
  if (input.indexOf("saturation") > -1) {
    mys = input.substring(10, count);
    s = mys.toInt();
  }

  //hueSet();
  Serial.println("Hue: " + myh);
  Serial.println("Brightness: " + myb);
  Serial.println("Sat: " + mys);

  CallBrightness(b);

}

