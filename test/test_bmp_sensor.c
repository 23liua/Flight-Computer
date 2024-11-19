/*#include "test/unity/unity.h";
#include "Core/Inc/bmp_sensor.h";

void setUp(void)
{
    // set stuff up here
}

void tearDown(void)
{
    // clean stuff up here
}

#include "unity.h"
#include "gps.h"
#include "mock_sdCard.h"
#include "mock_usart.h" // assuming USART functions need mocking
#include <string.h>
#include <stdlib.h>

Neo6M_GpsData NEO_GPS;

// Mock implementations for CDC_Transmit_FS and HAL_UART_Transmit_DMA
void CDC_Transmit_FS(uint8_t *data, uint16_t length)
{
    // Mock implementation: for Unity to test if it was called with expected data
}

void HAL_UART_Transmit_DMA(UART_HandleTypeDef *huart, uint8_t *data, uint16_t length)
{
    // Mock implementation for Unity to check if called
}

void setUp(void)
{
    memset(&NEO_GPS, 0, sizeof(NEO_GPS)); // Reset data before each test
}

void tearDown(void)
{
    // Clean up code if needed after each test
}

void test_Gps_Data_Parse_ValidData(void)
{
    UART_HandleTypeDef huart;      // Mocked UART handler
    uint32_t lastTransmitTime = 0; // Mocked timestamp

    // Sample valid GPGGA data for parsing
    strcpy((char *)NEO_GPS.data, "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\n");

    // Expected output string after parsing and formatting
    char expected_output[100];
    sprintf(expected_output, "Time: %.5lf Latitude: %.5lf Longitude: %.5lf\r\n", 123519.0, 4807.038, 1131.000);

    // Set expectations for mock functions
    sd_write_Expect(expected_output); // Expect SD card write
    CDC_Transmit_FS_ExpectWithArray((uint8_t *)expected_output, strlen(expected_output), strlen(expected_output));
    HAL_UART_Transmit_DMA_Expect(&huart, (uint8_t *)expected_output, 100);

    // Run the function with valid data
    Gps_Data_Parse(&huart, lastTransmitTime);

    // Validate parsed values
    TEST_ASSERT_EQUAL_FLOAT(123519.0, NEO_GPS.time);
    TEST_ASSERT_EQUAL_FLOAT(4807.038, NEO_GPS.latitude);
    TEST_ASSERT_EQUAL_CHAR('N', NEO_GPS.N_OR_S);
    TEST_ASSERT_EQUAL_FLOAT(1131.000, NEO_GPS.longitude);
    TEST_ASSERT_EQUAL_CHAR('E', NEO_GPS.E_OR_W);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_Gps_Data_Parse_ValidData);
    return UNITY_END();
}
*/