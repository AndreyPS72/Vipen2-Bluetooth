#ifndef _TYPES_VIPEN2_H
#define _TYPES_VIPEN2_H

#include <stdint.h>
//#include "Bluetooth_ViPen1.h"




const float VIPEN2_ADC_FREQUENCY = 25600.0f;	// Примерная частота ацепирования сигнала, реально = 25602.41 Hz
const uint32_t VIPEN2_WAVEFORM_COUNT = 8192UL;	// Макс отсчётов в сигнале
const uint32_t VIPEN2_SPECTRUM_COUNT = VIPEN2_WAVEFORM_COUNT * 100UL / 256UL + 1UL;	// Макс 3201 линий в спектре






// Команды, приходящие с Bluetooth
const uint32_t VIPEN2_BT_COMMAND_NONE = (0);	// Нет команды
const uint32_t VIPEN2_BT_COMMAND_START = (1);	// Старт чтения, в структуре - параметры чтения
const uint32_t VIPEN2_BT_COMMAND_STOP = (2);	// Стоп чтения
const uint32_t VIPEN2_BT_COMMAND_IDLE = (3);	// Пока не отключаться
const uint32_t VIPEN2_BT_COMMAND_OFF = (4);	// Выключиться

// Эти команды только для производителя. Пользователю их не показывать
const uint32_t VIPEN2_BT_COMMAND_CALIBRATION_0 = (0x10);	// Сброс калибровок (Scale = default)
const uint32_t VIPEN2_BT_COMMAND_CALIBRATION_45HZ = (0x11);	// Калибровка 45,0Гц 20мм/с, стандартный канал
const uint32_t VIPEN2_BT_COMMAND_CALIBRATION_10HZ = (0x12);	// Калибровка 10,0Гц 20мм/с, медленный канал
const uint32_t VIPEN2_BT_COMMAND_CALIBRATION_1KHZ = (0x20);	// Калибровка Envelope 1,0кГц 20м/с2
const uint32_t VIPEN2_BT_COMMAND_CALIBRATION_SAVE = (0x30);	// Сохранить калибровки

const uint32_t VIPEN2_BT_COMMAND_TEST = (0x80);	// Внутренний тест прибора

typedef uint32_t TBluetoothCommand;



// Что измеряем ?
#define MEAS_TYPE_SPECTRUM			(0)	// Спектр: Стандартный канал: Ускорение 10-10000 Гц; Скорость 10-1000 Гц; Перемещение 10-200 Гц; 
#define MEAS_TYPE_WAVEFORM			(1)	// Сигнал: Стандартный канал: Ускорение 10-10000 Гц; Скорость 10-1000 Гц; Перемещение 10-200 Гц; 
#define MEAS_TYPE_SPECTRUM_SLOW		(2)	// Спектр: Тихоходный канал: 0,5-50 Гц; Ускорение, Скорость, Перемещение
#define MEAS_TYPE_WAVEFORM_SLOW		(3)	// Сигнал: Тихоходный канал: 0,5-50 Гц; Ускорение, Скорость, Перемещение
#define MEAS_TYPE_SPECTRUM_ENV		(4)	// Спектр огибающей Ускорения 0,5-10 кГц; Только канал Ускорения
#define MEAS_TYPE_WAVEFORM_ENV		(5)	// Сигнал огибающей Ускорения 0,5-10 кГц; Только канал Ускорения
#define MEAS_TYPE_COUNT				(6)

#define MEAS_TYPE_SPECTRUM_MASK		(0)
#define MEAS_TYPE_WAVEFORM_MASK		(1)


#define MEAS_UNITS_ACCELERATION		(0)	// Ускорение
#define MEAS_UNITS_VELOCITY			(1) // Скорость
#define MEAS_UNITS_DISPLACEMENT		(2)	// Перемещение
#define MEAS_UNITS_COUNT			(3)




// Отсчётов в сигнале
#define SETUP_ALLX_256		(0)	// 256
#define SETUP_ALLX_1K		(1)	// 1024
#define SETUP_ALLX_2K		(2)	// 2048
#define SETUP_ALLX_8K		(3)	// 8192
#define SETUP_ALLX_COUNT	(4)

// Отсчётов в спектре
#define SETUP_ALLF_100		SETUP_ALLX_256	// 100 + 1
#define SETUP_ALLF_400		SETUP_ALLX_1K	// 400 + 1
#define SETUP_ALLF_800		SETUP_ALLX_2K	// 800 + 1
#define SETUP_ALLF_3200		SETUP_ALLX_8K	// 3200 + 1
#define SETUP_ALLF_COUNT	SETUP_ALLX_COUNT

// Частота семплирования в сигнале 
#define SETUP_DX_256_HZ		(0)	// 256 Гц
#define SETUP_DX_640_HZ		(1)	// 640 Гц
#define SETUP_DX_2560_HZ	(2)	// 2560 Гц
#define SETUP_DX_6400_HZ	(3)	// 6400 Гц
#define SETUP_DX_25600_HZ	(4)	// 25600 Гц
#define SETUP_DX_COUNT		(5)

// Верхняя частота в спектре
#define SETUP_FN_100_HZ		SETUP_DX_256_HZ		// 100 Гц
#define SETUP_FN_250_HZ		SETUP_DX_640_HZ		// 250 Гц
#define SETUP_FN_1000_HZ	SETUP_DX_2560_HZ	// 1000 Гц
#define SETUP_FN_2500_HZ	SETUP_DX_6400_HZ	// 2500 Гц
#define SETUP_FN_10000_HZ	SETUP_DX_25600_HZ	// 10000 Гц
#define SETUP_FN_COUNT		SETUP_DX_COUNT

const uint32_t SetupMeasAllX[SETUP_ALLX_COUNT] = { 256, 1024, 2048, 8192 };
const float SetupMeasdX[SETUP_DX_COUNT] = { 1.0f / 256.0f, 1.0f / 640.0f, 1.0f / 2560.0f, 1.0f / 6400.0f, 1.0f / 25600.0f };


// Усреднения для спектров
#define SETUP_AVG_NO		(0)	// Нет усреднений
#define SETUP_AVG_4_STOP	(1)	// Усреднить 4 спектра и остановить измерение
#define SETUP_AVG_10_STOP	(2)	// Усреднить 10 спектров и остановить измерение
#define SETUP_AVG_999		(3)	// Усреднять спектры всё время, пока не нажмут Стоп
#define SETUP_AVG_COUNT		(4)




#define SETUP_EXTERNAL_SENSOR	(0)	// Сигнал подаётся с датчика
#define SETUP_INTERNAL_DAC		(1)	// Сигнал подаётся с внутреннего ЦАП - используется для внутренних тестов

#define SETUP_READ_MODE			(0)	// Режим измерения
#define SETUP_CALIBRATION_MODE	(1)	// Режим калибровки - только для производителя



#pragma pack(4) // Структуры выровнены на 4 байта

// Текущие настройки чтения
typedef struct
{
	TBluetoothCommand Command;

	uint32_t MeasType;	// MEAS_TYPE_xxx
	uint32_t MeasUnits;	// MEAS_UNITS_xxx

	uint32_t AllX;		// SETUP_ALLX_xxx или SETUP_ALLF_xxx
	uint32_t dX;		// SETUP_DX_xxx или SETUP_FN_xxx

	uint32_t Avg;		// SETUP_AVG_xxx


	uint32_t InternalDAC; // Переключение канала: 0 - Вход с датчиков, 1 - с DAC
	uint32_t CalibrationMode; // 0 - Работа, 1 - Калибровка

	uint32_t Reserv[8];

} TVipen2MeasureSetup;
#define szTVipen2MeasureSetup sizeof(TVipen2MeasureSetup)
static_assert(szTVipen2MeasureSetup == 64, "");









#pragma pack(1) // Структуры выровнены на 1 байт



typedef struct stVipen2AdvertisingData
{
	uint8_t Len1;
	uint8_t Type1;
	uint8_t Flag1;

	uint8_t Len9;
	uint8_t Type9;
	char Name9[5];

	uint8_t LenFF;
	uint8_t TypeFF;
	uint16_t ManID;
	uint8_t Addr;	// ==0
	uint16_t DeviceNumber;	// Номер прибора
	uint32_t TimeStamp;	// Счётчик 1024 Гц для проверки, что появились новые данные
	int16_t Values[4];	// Velocity, Value, Excess, Temperature
	uint8_t Battery;	// В беконе хранится состояние батареи в процентах 0..100 %
						// Старший бит == 1 - прибор заряжается (зелёный светодиод)
	uint8_t Firmware;	// Версии FW процессоров:
						// Старшие 4 бита = для SAME70, младшие 4 бита = для CC2640 

} TVipen2AdvertisingData;
#define szTVipen2AdvertisingData sizeof(TVipen2AdvertisingData)
static_assert(szTVipen2AdvertisingData == 31, "");


#define GAP_ADTYPE_FLAGS                    1
#define GAP_ADTYPE_LOCAL_NAME_COMPLETE      9
#define GAP_ADTYPE_MANUFACTURER_SPECIFIC    0xFF

#define GAP_ADTYPE_FLAGS_GENERAL    0x02
#define GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED    0x04


#define TI_COMPANY_ID (0x000D) // Company Identifier: Texas Instruments Inc. (13)
//#define ViPen_Bt_ID   (0x4F5C) // Magic number



// бекон
static TVipen2AdvertisingData Vipen2AdvertisingData =
{
	0x02, GAP_ADTYPE_FLAGS, GAP_ADTYPE_FLAGS_GENERAL | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,
	0x06, GAP_ADTYPE_LOCAL_NAME_COMPLETE, 'V', 'i', 'P', '-', '2',
	0x14, GAP_ADTYPE_MANUFACTURER_SPECIFIC, TI_COMPANY_ID,
		0x00, 0x0001, 0x00000000, 0, 0, -200, 0, 0, 0
};

// В беконе в поле Battery хранится состояние батареи в процентах 0..100 %
// Старший бит == 1 - прибор заряжается (зелёный светодиод)
#define VIPEN2_CHARGE	(0x80) 




typedef struct
{
	uint8_t Addr;	// ==0
	uint16_t DeviceNumber;	// Номер прибора
	uint32_t TimeStamp;	// Счётчик 1024 Гц для проверки, что появились новые данные
	int16_t Values[4];	// Velocity, Value, Excess, Temperature
	uint8_t Battery;	// В беконе хранится состояние батареи в процентах 0..100 %
						// Старший бит == 1 - прибор заряжается (зелёный светодиод)
	uint8_t Firmware;	// Версии FW процессоров:
						// Старшие 4 бита = для SAME70, младшие 4 бита = для CC2640 

} TVipen2UserData;
#define szTVipen2UserData sizeof(TVipen2UserData)
static_assert(szTVipen2UserData == 17, "");




// Биты состояния, Read, Notify:
#define VIPEN_STATE_STOPED	(0<<0) // Прибор стоит
#define VIPEN_STATE_STARTED	(1<<0) // Прибор в режиме измерения
#define VIPEN_STATE_NODATA	(0<<1) // Данных нет (после инициализации)
#define VIPEN_STATE_DATA	(1<<1) // Есть данные








// Команда для запроса сигнала/спектра
#define VIPEN2_GET_DATA		(0x0010) // запросить сигнал/спектр
// Команда для запроса текстового лога
#define VIPEN2_GET_LOG		(0x0020) // запросить текстовый лог




#define VIPEN2_BLUETOOTH_PACKET_DATA_SIZE	(236)	// длина блока данных

#define VIPEN2_STAMPS_IN_BLOCK ((VIPEN2_BLUETOOTH_PACKET_DATA_SIZE-2)/2) // 117 отсчётов в блоке
#define VIPEN2_MAX_DATA_BLOCKS  (VIPEN2_WAVEFORM_COUNT / VIPEN2_STAMPS_IN_BLOCK + 1) // Длина сигнала = до 71 блока


typedef struct
{
	uint8_t ViPen2_Get_Data_Command;	// Команда
	uint8_t ViPen2_Get_Data_Block; 		// Номер блока
	uint8_t ViPen2_Get_Wave_ID; 		// Счётчик, позволяет проверить, что качаем тот-же замер
										// Увеличивается на 1, при запросе заголовка
	uint8_t ViPen2_Data_Blocks;			// Количество блоков с данными, до 72: 0 = Hdr; 1..71 = отсчёты

	uint32_t Timestamp;	//  Счётчик 1024 Гц, совпадает с User_Data. Timestamp
	float Coeff;	// Коэф перевода данных int16_t в float, 4 байта

	uint32_t DataType;	// MEAS_TYPE_xxx
	uint32_t DataUnits;	// MEAS_UNITS_xxx
	uint32_t DataLen; // Отсчётов
	float DataDX;	// Шаг между отсчётами, сек или Гц

	int32_t SpectrumAvg;
	int32_t SpectrumAvgMax;

    int16_t Values[4];	// Velocity, Value, Excess, Temperature

	uint8_t Reading;	// 1 - идёт измерение; 0 - стоит
//	uint8_t IsCharging;	// 1 - идёт зарядка; 0 - нет
//	uint8_t Battery;	// % зарядки аккумулятора = 0..100 %
	uint8_t Align1[3];
    
	uint8_t Reserv2[VIPEN2_BLUETOOTH_PACKET_DATA_SIZE - 12 * 4];
} TVipen2_Waveform_Header;
#define szTVipen2_Waveform_Header sizeof(TVipen2_Waveform_Header)
static_assert(szTVipen2_Waveform_Header == VIPEN2_BLUETOOTH_PACKET_DATA_SIZE, "");



typedef struct 
{
	uint8_t ViPen2_Get_Data_Block; 	// Номер блока, 0..71
	uint8_t ViPen2_Get_Wave_ID; 	// Счётчик, позволяет проверить, что качаем тот-же замер
	int16_t Wave[VIPEN2_STAMPS_IN_BLOCK];	// отсчёты = 2 байта знаковое * 117 отсчётов в блоке
} TVipen2_Waveform_Data;
#define szTVipen2_Waveform_Data sizeof(TVipen2_Waveform_Data)
static_assert(szTVipen2_Waveform_Data == VIPEN2_BLUETOOTH_PACKET_DATA_SIZE, "");






#define VIPEN2_LOG_NUMBER	(32)	// 32 блока в текстовом логе
#define VIPEN2_LOG_SIZE	((VIPEN2_BLUETOOTH_PACKET_DATA_SIZE-2)*VIPEN2_LOG_NUMBER)	// 32 * 234 = 7488 байт в текстовом логе


// Совпадает со структурой TVipen2_Waveform_Data
typedef struct 
{
	uint8_t ViPen2_GetLog_Block; 	// Номер блока
	uint8_t ViPen2_GetLog_ID; 	// Счётчик, позволяет проверить, что качаем тот-же лог
	int16_t LogData[VIPEN2_BLUETOOTH_PACKET_DATA_SIZE-2];	// 234 байта * 32 блока = 7488 байт
} TVipen2_Log_Data;













#pragma pack(4) // Структуры выровнены на 4 байта


#define DEVICE_OFF_TIME	(10 * 60 * RTC_FREQ) // Выключаться через 10 мин неактивности


// Текущие настройки для записи-загрузки из FRAM
typedef struct
{

	uint32_t NormIndex; // Индекс текущей нормы в массиве Norms[]
	uint32_t NormIndexW, NormIndexA; //  Индексы Норм для Yellow  и Red


	TVipen2MeasureSetup Meas; // Текущие настройки чтения

	uint32_t Reserv[12];

	uint16_t Align;
	uint16_t CRC;

} TVipen2Setup;
#define szTVipen2Setup sizeof(TVipen2Setup)


#pragma pack()


#endif

