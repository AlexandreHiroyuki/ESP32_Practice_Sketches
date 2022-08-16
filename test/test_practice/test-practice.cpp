#include <unity.h>
#include <Arduino.h>

void test_practice(){
    TEST_ASSERT_EQUAL_INT(1, 1);
}

void setup(){
    delay(200);
    UNITY_BEGIN();
    RUN_TEST(test_practice);
    UNITY_END();
}

void loop(){
}
