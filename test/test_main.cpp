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

void test_user_set_min_temp(void)
{
    User *user = NULL;
    user = new User(20, 21);

    user->setMinTemp(10);

    TEST_ASSERT_EQUAL(user->getMinTemp(), 10);
    //clean up
    delete user;
}

void test_user_set_too_low_min_temp(void)
{
    User *user = NULL;
    user = new User(20, 21);

    //set it to 5 which means it wont pass and leave it as 20
    user->setMinTemp(5);

    TEST_ASSERT_EQUAL(user->getMinTemp(), 20);
    //clean up
    delete user;
}

void test_user_set_min_temp_over_max_temp(void)
{
    User *user = NULL;
    user = new User(20, 21);

    //set it to 22 which means it wont pass and leave it as 20
    user->setMinTemp(22);

    TEST_ASSERT_EQUAL(user->getMinTemp(), 20);
    //clean up
    delete user;
}

void test_user_set_max_temp(void)
{
    User *user = NULL;
    user = new User(20, 21);

    user->setMinTemp(28);

    TEST_ASSERT_EQUAL(user->getMaxTemp(), 20);
    //clean up
    delete user;
}

void test_user_set_too_high_max_temp(void)
{
    User *user = NULL;
    user = new User(20, 21);

    //set it to 30 which means it wont pass and leave it as 21
    user->setMinTemp(30);

    TEST_ASSERT_EQUAL(user->getMaxTemp(), 21);
    //clean up
    delete user;
}

void test_user_set_max_temp_below_min_temp(void)
{
    User *user = NULL;
    user = new User(20, 21);

    //set it to 18 which means it wont pass and leave it as 21
    user->setMinTemp(18);

    TEST_ASSERT_EQUAL(user->getMaxTemp(), 21);
    //clean up
    delete user;
}

void test_set_rgb_led_green(void)
{
    bool passed;
    RGBLed *led = NULL;
    led = new RGBLed(14, 12, 13, 0, 1, 2, 5000, 8);
    led->init();

    try
    {
        led->setRGBValue(0, 255, 0);
        passed = true;
    }
    catch (const std::exception &e)
    {
        passed = false;
    }

    TEST_ASSERT_TRUE(passed);

    delete led;
}

void test_set_rgb_led_red(void)
{
    bool passed;
    RGBLed *led = NULL;
    led = new RGBLed(14, 12, 13, 0, 1, 2, 5000, 8);
    led->init();

    try
    {
        led->setRGBValue(255, 0, 0);
        passed = true;
    }
    catch (const std::exception &e)
    {
        passed = false;
    }

    TEST_ASSERT_TRUE(passed);

    delete led;
}

void test_set_rgb_led_blue(void)
{
    bool passed;
    RGBLed *led = NULL;
    led = new RGBLed(14, 12, 13, 0, 1, 2, 5000, 8);
    led->init();

    try
    {
        led->setRGBValue(0, 0, 255);
        passed = true;
    }
    catch (const std::exception &e)
    {
        passed = false;
    }

    TEST_ASSERT_TRUE(passed);

    delete led;
}

void test_set_rgb_led_amber(void)
{
    bool passed;
    RGBLed *led = NULL;
    led = new RGBLed(14, 12, 13, 0, 1, 2, 5000, 8);
    led->init();

    try
    {
        led->setRGBValue(255, 135, 0);
        passed = true;
    }
    catch (const std::exception &e)
    {
        passed = false;
    }

    TEST_ASSERT_TRUE(passed);

    delete led;
}

void test_set_user_status_present(void)
{
    User *user = NULL;
    user = new User(20, 21);

    user->setStatus(UserState::PRESENT);

    TEST_ASSERT_EQUAL(UserState::PRESENT, user->getStatus());

    delete user;
}

void test_set_user_status_absent(void)
{
    User *user = NULL;
    user = new User(20, 21);

    user->setStatus(UserState::ABSENT);

    TEST_ASSERT_EQUAL(UserState::ABSENT, user->getStatus());

    delete user;
}

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

    // testing the RGB LED colour setting
    RUN_TEST(test_set_rgb_led_green);
    RUN_TEST(test_set_rgb_led_red);
    RUN_TEST(test_set_rgb_led_blue);
    RUN_TEST(test_set_rgb_led_amber);

    // testing setting user status
    RUN_TEST(test_set_user_status_present);
    RUN_TEST(test_set_user_status_absent);

    UNITY_END();
}

void loop()
{
}