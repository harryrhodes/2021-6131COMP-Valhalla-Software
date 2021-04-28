#include <Arduino.h>
#include <unity.h>
#include <User.h>
#include <RGB_LED.h>

void test_user_temp_min_and_max(void)
{
    User *user = NULL;
    user = new User(20, 21);
    unsigned short maxTemp = user->getMinTemp();
    unsigned short minTemp = user->getMaxTemp();

    TEST_ASSERT_EQUAL(maxTemp, 21);
    TEST_ASSERT_EQUAL(minTemp, 20);
    //clean up
    delete user;
}

void test_user_set_min_temp()
{
    User *user = NULL;
    user = new User(20, 21);

    user->setMinTemp(10);

    TEST_ASSERT_EQUAL(user->getMinTemp(), 10);
    //clean up
    delete user;
}

void test_user_set_too_low_min_temp()
{
    User *user = NULL;
    user = new User(20, 21);

    //set it to 5 which means it wont pass and leave it as 20
    user->setMinTemp(5);

    TEST_ASSERT_EQUAL(user->getMinTemp(), 20);
    //clean up
    delete user;
}

void test_user_set_min_temp_over_max_temp()
{
    User *user = NULL;
    user = new User(20, 21);

    //set it to 22 which means it wont pass and leave it as 20
    user->setMinTemp(22);

    TEST_ASSERT_EQUAL(user->getMinTemp(), 20);
    //clean up
    delete user;
}

void test_user_set_max_temp()
{
    User *user = NULL;
    user = new User(20, 21);

    user->setMinTemp(28);

    TEST_ASSERT_EQUAL(user->getMaxTemp(), 20);
    //clean up
    delete user;
}

void test_user_set_too_high_max_temp()
{
    User *user = NULL;
    user = new User(20, 21);

    //set it to 30 which means it wont pass and leave it as 21
    user->setMinTemp(30);

    TEST_ASSERT_EQUAL(user->getMaxTemp(), 21);
    //clean up
    delete user;
}

void test_user_set_max_temp_below_min_temp()
{
    User *user = NULL;
    user = new User(20, 21);

    //set it to 18 which means it wont pass and leave it as 21
    user->setMinTemp(18);

    TEST_ASSERT_EQUAL(user->getMaxTemp(), 21);
    //clean up
    delete user;
}

// void test_rgb_led(void)
// {
//     RGBLed *led = NULL;
//     led = new RGBLed(14, 12, 13, 0, 1, 2, 5000, 8);
//     led->init();

//     led->setRGBValue(0, 255, 0);
// }

void setup()
{
    UNITY_BEGIN();

    // testing the min and max temp settings
    RUN_TEST(test_user_temp_min_and_max);
    RUN_TEST(test_user_set_min_temp);
    RUN_TEST(test_user_set_too_low_min_temp);
    RUN_TEST(test_user_set_min_temp_over_max_temp);
    RUN_TEST(test_user_set_max_temp);
    RUN_TEST(test_user_set_too_high_max_temp);
    RUN_TEST(test_user_set_max_temp_below_min_temp);

    // testing the RGB LED
    // RUN_TEST(test_rgb_led);

    UNITY_END();
}

void loop()
{
}