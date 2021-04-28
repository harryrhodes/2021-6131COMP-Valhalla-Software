#include <Arduino.h>
#include <unity.h>
#include <SD.h>

int chipSelect = 5;

void test_sd_initalised(void) {
    TEST_ASSERT_TRUE(SD.begin(chipSelect));
    SD.end();
}

void test_sd_write(void) {
    SD.begin(chipSelect);
    File testFile = SD.open("/testFile.txt", FILE_WRITE);
    TEST_ASSERT_TRUE(testFile);
    TEST_ASSERT_TRUE(testFile.println("test"));
    testFile.close();
    SD.end();
}

void test_sd_read(void) {
    SD.begin(chipSelect);
    File testFile = SD.open("/testFile.txt");
    TEST_ASSERT_TRUE(testFile);
    testFile.close();
    SD.end();
}

void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    UNITY_BEGIN();    // IMPORTANT LINE!
    RUN_TEST(test_sd_initalised);
    RUN_TEST(test_sd_read);
    RUN_TEST(test_sd_write);
    UNITY_END();

}

void loop() {

}