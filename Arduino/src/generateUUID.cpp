#include "generateUUID.h"

String generateUUID() {
    // Buffer to store UUID
    char uuid[37];
    
    // Seed random number generator with the current millis value
    randomSeed(millis());
    
    // Generate random values for UUID parts
    sprintf(uuid, "%04x%04x-%04x-%04x-%04x-%04x%04x%04x",
        random(0, 0xffff), random(0, 0xffff),    // Generates 8 hex digits
        random(0, 0xffff),                      // Generates 4 hex digits
        random(0, 0x0fff) | 0x4000,             // Generates 4 hex digits, but ensures the first digit is 4
        random(0, 0x3fff) | 0x8000,             // Generates 4 hex digits, but ensures the first digit is 8, 9, A, or B
        random(0, 0xffff), random(0, 0xffff), random(0, 0xffff)  // Generates 12 hex digits
    );
    
    // Return the generated UUID as a string
    return String(uuid);
}