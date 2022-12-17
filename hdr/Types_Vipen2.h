#ifndef _TYPES_VIPEN2_H
#define _TYPES_VIPEN2_H

#include <stdint.h>
//#include "Bluetooth_ViPen1.h"




const float VIPEN2_ADC_FREQUENCY = 25600.0f;	// ��������� ������� ������������ �������, ������� = 25602.41 Hz
const uint32_t VIPEN2_WAVEFORM_COUNT = 8192UL;	// ���� �������� � �������
const uint32_t VIPEN2_SPECTRUM_COUNT = VIPEN2_WAVEFORM_COUNT * 100UL / 256UL + 1UL;	// ���� 3201 ����� � �������






// �������, ���������� � Bluetooth
const uint32_t VIPEN2_BT_COMMAND_NONE = (0);	// ��� �������
const uint32_t VIPEN2_BT_COMMAND_START = (1);	// ����� ������, � ��������� - ��������� ������
const uint32_t VIPEN2_BT_COMMAND_STOP = (2);	// ���� ������
const uint32_t VIPEN2_BT_COMMAND_IDLE = (3);	// ���� �� �����������
const uint32_t VIPEN2_BT_COMMAND_OFF = (4);	// �����������

// ��� ������� ������ ��� �������������. ������������ �� �� ����������
const uint32_t VIPEN2_BT_COMMAND_CALIBRATION_0 = (0x10);	// ����� ���������� (Scale = default)
const uint32_t VIPEN2_BT_COMMAND_CALIBRATION_45HZ = (0x11);	// ���������� 45,0�� 20��/�, ����������� �����
const uint32_t VIPEN2_BT_COMMAND_CALIBRATION_10HZ = (0x12);	// ���������� 10,0�� 20��/�, ��������� �����
const uint32_t VIPEN2_BT_COMMAND_CALIBRATION_1KHZ = (0x20);	// ���������� Envelope 1,0��� 20�/�2
const uint32_t VIPEN2_BT_COMMAND_CALIBRATION_SAVE = (0x30);	// ��������� ����������

const uint32_t VIPEN2_BT_COMMAND_TEST = (0x80);	// ���������� ���� �������

typedef uint32_t TBluetoothCommand;



// ��� �������� ?
#define MEAS_TYPE_SPECTRUM			(0)	// ������: ����������� �����: ��������� 10-10000 ��; �������� 10-1000 ��; ����������� 10-200 ��; 
#define MEAS_TYPE_WAVEFORM			(1)	// ������: ����������� �����: ��������� 10-10000 ��; �������� 10-1000 ��; ����������� 10-200 ��; 
#define MEAS_TYPE_SPECTRUM_SLOW		(2)	// ������: ���������� �����: 0,5-50 ��; ���������, ��������, �����������
#define MEAS_TYPE_WAVEFORM_SLOW		(3)	// ������: ���������� �����: 0,5-50 ��; ���������, ��������, �����������
#define MEAS_TYPE_SPECTRUM_ENV		(4)	// ������ ��������� ��������� 0,5-10 ���; ������ ����� ���������
#define MEAS_TYPE_WAVEFORM_ENV		(5)	// ������ ��������� ��������� 0,5-10 ���; ������ ����� ���������
#define MEAS_TYPE_COUNT				(6)

#define MEAS_TYPE_SPECTRUM_MASK		(0)
#define MEAS_TYPE_WAVEFORM_MASK		(1)


#define MEAS_UNITS_ACCELERATION		(0)	// ���������
#define MEAS_UNITS_VELOCITY			(1) // ��������
#define MEAS_UNITS_DISPLACEMENT		(2)	// �����������
#define MEAS_UNITS_COUNT			(3)




// �������� � �������
#define SETUP_ALLX_256		(0)	// 256
#define SETUP_ALLX_1K		(1)	// 1024
#define SETUP_ALLX_2K		(2)	// 2048
#define SETUP_ALLX_8K		(3)	// 8192
#define SETUP_ALLX_COUNT	(4)

// �������� � �������
#define SETUP_ALLF_100		SETUP_ALLX_256	// 100 + 1
#define SETUP_ALLF_400		SETUP_ALLX_1K	// 400 + 1
#define SETUP_ALLF_800		SETUP_ALLX_2K	// 800 + 1
#define SETUP_ALLF_3200		SETUP_ALLX_8K	// 3200 + 1
#define SETUP_ALLF_COUNT	SETUP_ALLX_COUNT

// ������� ������������� � ������� 
#define SETUP_DX_256_HZ		(0)	// 256 ��
#define SETUP_DX_640_HZ		(1)	// 640 ��
#define SETUP_DX_2560_HZ	(2)	// 2560 ��
#define SETUP_DX_6400_HZ	(3)	// 6400 ��
#define SETUP_DX_25600_HZ	(4)	// 25600 ��
#define SETUP_DX_COUNT		(5)

// ������� ������� � �������
#define SETUP_FN_100_HZ		SETUP_DX_256_HZ		// 100 ��
#define SETUP_FN_250_HZ		SETUP_DX_640_HZ		// 250 ��
#define SETUP_FN_1000_HZ	SETUP_DX_2560_HZ	// 1000 ��
#define SETUP_FN_2500_HZ	SETUP_DX_6400_HZ	// 2500 ��
#define SETUP_FN_10000_HZ	SETUP_DX_25600_HZ	// 10000 ��
#define SETUP_FN_COUNT		SETUP_DX_COUNT

const uint32_t SetupMeasAllX[SETUP_ALLX_COUNT] = { 256, 1024, 2048, 8192 };
const float SetupMeasdX[SETUP_DX_COUNT] = { 1.0f / 256.0f, 1.0f / 640.0f, 1.0f / 2560.0f, 1.0f / 6400.0f, 1.0f / 25600.0f };


// ���������� ��� ��������
#define SETUP_AVG_NO		(0)	// ��� ����������
#define SETUP_AVG_4_STOP	(1)	// ��������� 4 ������� � ���������� ���������
#define SETUP_AVG_10_STOP	(2)	// ��������� 10 �������� � ���������� ���������
#define SETUP_AVG_999		(3)	// ��������� ������� �� �����, ���� �� ������ ����
#define SETUP_AVG_COUNT		(4)




#define SETUP_EXTERNAL_SENSOR	(0)	// ������ ������� � �������
#define SETUP_INTERNAL_DAC		(1)	// ������ ������� � ����������� ��� - ������������ ��� ���������� ������

#define SETUP_READ_MODE			(0)	// ����� ���������
#define SETUP_CALIBRATION_MODE	(1)	// ����� ���������� - ������ ��� �������������



#pragma pack(4) // ��������� ��������� �� 4 �����

// ������� ��������� ������
typedef struct
{
	TBluetoothCommand Command;

	uint32_t MeasType;	// MEAS_TYPE_xxx
	uint32_t MeasUnits;	// MEAS_UNITS_xxx

	uint32_t AllX;		// SETUP_ALLX_xxx ��� SETUP_ALLF_xxx
	uint32_t dX;		// SETUP_DX_xxx ��� SETUP_FN_xxx

	uint32_t Avg;		// SETUP_AVG_xxx


	uint32_t InternalDAC; // ������������ ������: 0 - ���� � ��������, 1 - � DAC
	uint32_t CalibrationMode; // 0 - ������, 1 - ����������

	uint32_t Reserv[8];

} TVipen2MeasureSetup;
#define szTVipen2MeasureSetup sizeof(TVipen2MeasureSetup)
static_assert(szTVipen2MeasureSetup == 64, "");









#pragma pack(1) // ��������� ��������� �� 1 ����



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
	uint16_t DeviceNumber;	// ����� �������
	uint32_t TimeStamp;	// ������� 1024 �� ��� ��������, ��� ��������� ����� ������
	int16_t Values[4];	// Velocity, Value, Excess, Temperature
	uint8_t Battery;	// � ������ �������� ��������� ������� � ��������� 0..100 %
						// ������� ��� == 1 - ������ ���������� (������ ���������)
	uint8_t Firmware;	// ������ FW �����������:
						// ������� 4 ���� = ��� SAME70, ������� 4 ���� = ��� CC2640 

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



// �����
static TVipen2AdvertisingData Vipen2AdvertisingData =
{
	0x02, GAP_ADTYPE_FLAGS, GAP_ADTYPE_FLAGS_GENERAL | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,
	0x06, GAP_ADTYPE_LOCAL_NAME_COMPLETE, 'V', 'i', 'P', '-', '2',
	0x14, GAP_ADTYPE_MANUFACTURER_SPECIFIC, TI_COMPANY_ID,
		0x00, 0x0001, 0x00000000, 0, 0, -200, 0, 0, 0
};

// � ������ � ���� Addr �������� ��������� ������� � ��������� 0..100 %
// ������� ��� == 1 - ������ ���������� (������ ���������)
#define VIPEN2_CHARGE	(0x80) 




typedef struct
{
	uint8_t Addr;	// ==0
	uint16_t DeviceNumber;	// ����� �������
	uint32_t TimeStamp;	// ������� 1024 �� ��� ��������, ��� ��������� ����� ������
	int16_t Values[4];	// Velocity, Value, Excess, Temperature
	uint8_t Battery;	// � ������ �������� ��������� ������� � ��������� 0..100 %
						// ������� ��� == 1 - ������ ���������� (������ ���������)
	uint8_t Firmware;	// ������ FW �����������:
						// ������� 4 ���� = ��� SAME70, ������� 4 ���� = ��� CC2640 

} TVipen2UserData;
#define szTVipen2UserData sizeof(TVipen2UserData)
static_assert(szTVipen2UserData == 17, "");




// ���� ���������, Read, Notify:
#define VIPEN_STATE_STOPED	(0<<0) // ������ �����
#define VIPEN_STATE_STARTED	(1<<0) // ������ � ������ ���������
#define VIPEN_STATE_NODATA	(0<<1) // ������ ��� (����� �������������)
#define VIPEN_STATE_DATA	(1<<1) // ���� ������








// ������� ��� ������� �������/�������
#define VIPEN2_GET_DATA		(0x0010) // ��������� ������/������
// ������� ��� ������� ���������� ����
#define VIPEN2_GET_LOG		(0x0020) // ��������� ��������� ���




#define VIPEN2_BLUETOOTH_PACKET_DATA_SIZE	(236)	// ����� ����� ������

#define VIPEN2_STAMPS_IN_BLOCK ((VIPEN2_BLUETOOTH_PACKET_DATA_SIZE-2)/2) // 117 �������� � �����
#define VIPEN2_MAX_DATA_BLOCKS  (VIPEN2_WAVEFORM_COUNT / VIPEN2_STAMPS_IN_BLOCK + 1) // ����� ������� = �� 71 �����


typedef struct
{
	uint8_t ViPen2_Get_Data_Command;	// �������
	uint8_t ViPen2_Get_Data_Block; 		// ����� �����
	uint8_t ViPen2_Get_Wave_ID; 		// �������, ��������� ���������, ��� ������ ���-�� �����
										// ������������� �� 1, ��� ������� ���������
	uint8_t ViPen2_Data_Blocks;			// ���������� ������ � �������, �� 72: 0 = Hdr; 1..71 = �������

	uint32_t Timestamp;	//  ������� 1024 ��, ��������� � User_Data. Timestamp
	float Coeff;	// ���� �������� ������ int16_t � float, 4 �����

	uint32_t DataType;	// MEAS_TYPE_xxx
	uint32_t DataUnits;	// MEAS_UNITS_xxx
	uint32_t DataLen; // ��������
	float DataDX;	// ��� ����� ���������, ��� ��� ��

	int32_t SpectrumAvg;
	int32_t SpectrumAvgMax;

    int16_t Values[4];	// Velocity, Value, Excess, Temperature

	uint8_t Reading;	// 1 - ��� ���������; 0 - �����
//	uint8_t IsCharging;	// 1 - ��� �������; 0 - ���
//	uint8_t Battery;	// % ������� ������������ = 0..100 %
	uint8_t Align1[3];
    
	uint8_t Reserv2[VIPEN2_BLUETOOTH_PACKET_DATA_SIZE - 12 * 4];
} TVipen2_Waveform_Header;
#define szTVipen2_Waveform_Header sizeof(TVipen2_Waveform_Header)
static_assert(szTVipen2_Waveform_Header == VIPEN2_BLUETOOTH_PACKET_DATA_SIZE, "");



typedef struct 
{
	uint8_t ViPen2_Get_Data_Block; 	// ����� �����, 0..71
	uint8_t ViPen2_Get_Wave_ID; 	// �������, ��������� ���������, ��� ������ ���-�� �����
	int16_t Wave[VIPEN2_STAMPS_IN_BLOCK];	// ������� = 2 ����� �������� * 117 �������� � �����
} TVipen2_Waveform_Data;
#define szTVipen2_Waveform_Data sizeof(TVipen2_Waveform_Data)
static_assert(szTVipen2_Waveform_Data == VIPEN2_BLUETOOTH_PACKET_DATA_SIZE, "");






#define VIPEN2_LOG_NUMBER	(32)	// 32 ����� � ��������� ����
#define VIPEN2_LOG_SIZE	((VIPEN2_BLUETOOTH_PACKET_DATA_SIZE-2)*VIPEN2_LOG_NUMBER)	// 32 * 234 = 7488 ���� � ��������� ����


// ��������� �� ���������� TVipen2_Waveform_Data
typedef struct 
{
	uint8_t ViPen2_GetLog_Block; 	// ����� �����
	uint8_t ViPen2_GetLog_ID; 	// �������, ��������� ���������, ��� ������ ���-�� ���
	int16_t LogData[VIPEN2_BLUETOOTH_PACKET_DATA_SIZE-2];	// 234 ����� * 32 ����� = 7488 ����
} TVipen2_Log_Data;













#pragma pack(4) // ��������� ��������� �� 4 �����


#define DEVICE_OFF_TIME	(10 * 60 * RTC_FREQ) // ����������� ����� 10 ��� ������������


// ������� ��������� ��� ������-�������� �� FRAM
typedef struct
{

	uint32_t NormIndex; // ������ ������� ����� � ������� Norms[]
	uint32_t NormIndexW, NormIndexA; //  ������� ���� ��� Yellow  � Red


	TVipen2MeasureSetup Meas; // ������� ��������� ������

	uint32_t Reserv[12];

	uint16_t Align;
	uint16_t CRC;

} TVipen2Setup;
#define szTVipen2Setup sizeof(TVipen2Setup)


#pragma pack()


#endif

