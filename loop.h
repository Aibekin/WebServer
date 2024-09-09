void loop_func(int a, int b, bool isWork, int led, int work, int rest) {
  if (isWork) {
    a -= 1;
    digitalWrite(led, HIGH);
    Serial.println(a);
    if (a <= 0) {
      isWork = false;
      a = work;
    }
  } else {
    b -= 1;
    digitalWrite(led, LOW);
    Serial.println(b);
    if (b <= 0) {
      isWork = true;
      b = rest;
    }
  }
}

