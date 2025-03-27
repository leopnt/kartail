#include "exception.h"
#include "global_logger.h"

void Raise(String message, uint8_t ledPin)
{
    Serial.println("FATAL: " + message);
    while (true)
    {
        digitalWrite(ledPin, LOW);
        delay(200);
        digitalWrite(ledPin, HIGH);
        delay(200);
    }
}
